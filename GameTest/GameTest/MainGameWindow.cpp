#include "MainGameWindow.h"

void MainGameWindow::init(const std::string nickname, std::shared_ptr<Client> client) {
    this->client = client;

    this->windowPtr = std::make_shared<sf::RenderWindow>();
    this->windowPtr->create(sf::VideoMode(900, 600), "SkyFall Showdown", sf::Style::Close);
    this->windowPtr->setFramerateLimit(60);

    this->closeSettingsWindowFlag.store(false);
    this->inGameSettings = false;

    /* get the enemy nickname */
    if (!handleEnemyNickname()) {
        this->quitGame();
        return;
    }
    this->myNickname.setString(nickname);

    initSprites();
    
    if(!initPlayerAndEnemyPosition()) {
        this->quitGame();
        return;
    }
    /* start the thread to listen for game messages */
    std::thread t(&MainGameWindow::handleMessages, this);
    t.detach();

    sf::Event event;
    displayWindow = true;
    while (displayWindow) {
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->quitGame();
                return;
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                this->inGameSettings = true;
                if (gameSettingsMenu.init(windowPtr, &closeSettingsWindowFlag)) {
                    return;
                }
                this->inGameSettings = false;
            }
            handlePlayerMovement(event);
        }
        draw();
    }
}

bool MainGameWindow::initPlayerAndEnemyPosition() {
    std::random_device dev;
    std::mt19937 rng(dev());
    /* generate the X and Y*/
    std::uniform_int_distribution<std::mt19937::result_type> xPositionGenerator(100, 800);
    std::uniform_int_distribution<std::mt19937::result_type> yPositionGenerator(100, 500);

    float playerPosition[2];
    NetPacket p;
    /* check if they spawned at the same position */
    do {
        playerPosition[0] = xPositionGenerator(rng);
        playerPosition[1] = yPositionGenerator(rng);

        /* send the position*/
        if (!NetUtils::write_(*this->client->getSocket(), NetPacket(NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(playerPosition), sizeof(playerPosition)))) {
            std::cerr << "\nError in sending position to the server";
            return false;
        }
        /* get the enemy position */
        p = NetUtils::read_(*this->client->getSocket());
        if (p.getMsgType() == NetMessages::PLAYER_POSITION) {
            /* check if the position is the same */
            this->youPlayer->setPosition(sf::Vector2f(playerPosition[0], playerPosition[1]));
            this->enemyPlayer->setPosition(sf::Vector2f(p.getFloatVec()[0], p.getFloatVec()[1]));
        }
        else {
            return false;
        }
    } while (this->youPlayer->interscect(this->enemyPlayer->getRect()));

    return true;
}

void MainGameWindow::handleMessages() {
    NetPacket packet;

    while (true) {
        try {
            packet = NetUtils::read_(*this->client->getSocket());

            if (packet.getMsgType() == NetMessages::QUIT_GAME) {
                std::cout << "\nMatch end becasue [ " << this->enemyNickname.getString().toAnsiString() << " ] quit.";
               
                this->quitGame();
                return;
            }
            if (packet.getMsgType() == NetMessages::PLAYER_POSITION) {
                sf::Vector2f newPos(packet.getFloatVec()[0], packet.getFloatVec()[1]);
                this->enemyPlayer->move(newPos - this->enemyPlayer->getPosition());
            }
        }
        catch (const boost::system::system_error& ex) {
            std::cerr << "\nError in handleMessages() | " << ex.what();
            return;
        }
    }
}

void MainGameWindow::handlePlayerMovement(sf::Event& event) {
    sf::Vector2f coords(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        coords.y -= this->youPlayer->getVelocity();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        coords.x -= this->youPlayer->getVelocity();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        coords.y += this->youPlayer->getVelocity();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        coords.x += this->youPlayer->getVelocity();
    }
    
    sf::Vector2f proposedNewPos = youPlayer->getPosition() + coords;

    // Check window's borders
    if (proposedNewPos.x < 0.f) proposedNewPos.x = 0.f;
    else if (proposedNewPos.x > 900.f - 70.f) proposedNewPos.x = 900.f - 70.f;

    if (proposedNewPos.y < 0.f) proposedNewPos.y = 0.f;
    else if (proposedNewPos.y > 540.f - 70.f) proposedNewPos.y = 540.f - 70.f;

    // Create a rectangle of the proposed shape
    sf::RectangleShape proposedShape = this->youPlayer->getRect();
    proposedShape.setPosition(proposedNewPos);

    // Verifica della collisione con l'altro giocatore
    if (!proposedShape.getGlobalBounds().intersects(this->enemyPlayer->getRect().getGlobalBounds())) {
        youPlayer->move(proposedNewPos - youPlayer->getPosition());
        float sendPosition[2] = { proposedNewPos.x, proposedNewPos.y };

        NetUtils::write_(*this->client->getSocket(), NetPacket(NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(sendPosition), sizeof(sendPosition)));
    }
}

void MainGameWindow::quitGame() {
    this->client->close();
    this->displayWindow = false;

    if (this->inGameSettings) {
        this->closeSettingsWindowFlag.store(true);
    }
}

void MainGameWindow::draw() {
    this->windowPtr->clear();

    this->windowPtr->draw(myNickname);
    this->windowPtr->draw(enemyNickname);
    this->windowPtr->draw(*youPlayer);
    this->windowPtr->draw(*enemyPlayer);

    this->windowPtr->display();
}

void MainGameWindow::initSprites() {
    myNickname.setFont(FontManager::fredokaOne);
    myNickname.setCharacterSize(35);
    myNickname.setPosition(20, (windowPtr->getSize().y - 60));
    myNickname.setFillColor(sf::Color(31, 110, 2));
   
    enemyNickname.setFont(FontManager::fredokaOne);
    enemyNickname.setCharacterSize(35);
    enemyNickname.setPosition(((windowPtr->getSize().x - enemyNickname.getGlobalBounds().width) - 20), (windowPtr->getSize().y - 60));
    enemyNickname.setFillColor(sf::Color(110, 6, 2));

    youPlayer = std::make_shared<Player>(sf::Vector2f(70.f, 70.f), sf::Color(2, 35, 89), sf::Color(31, 110, 2), 8.0f, 12.f);
    enemyPlayer = std::make_shared<Player>(sf::Vector2f(70.f, 70.f), sf::Color(2, 35, 89), sf::Color(110, 6, 2), 8.0f, 12.f);
}

bool MainGameWindow::handleEnemyNickname() {
    try {
        NetPacket p = NetUtils::read_(*this->client->getSocket());
        this->enemyNickname.setString(p.getStr());
        return true;
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in handle enemy nickname | " << e.what() << "\n";
        return false;
    }
}