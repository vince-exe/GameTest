#include "MainGameWindow.h"

void MainGameWindow::init(const std::string nickname, std::shared_ptr<Client> client) {
    this->client = client;

    this->windowPtr = std::make_shared<sf::RenderWindow>();
    this->windowPtr->create(sf::VideoMode(1200, 800), "SkyFall Showdown", sf::Style::Close);
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

    sf::Clock clock;

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
            handleMouseClick(event);
        }
        update(clock.restart());
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
    } while (this->youPlayer->intersect(this->enemyPlayer->getRect()));

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

void MainGameWindow::checkPlayerWindowBorders() {
    sf::FloatRect playerBounds = this->youPlayer->getGlobalBounds();
    sf::Vector2f position = this->youPlayer->getPosition();

    if (playerBounds.left < 0.f) {
        this->youPlayer->setPosition(playerBounds.width / 2, position.y);
        this->youPlayer->stopMove();
    }
    else if ((playerBounds.left + playerBounds.width) > 1200.f) {
        this->youPlayer->setPosition(1200.f - playerBounds.width, position.y);
        this->youPlayer->stopMove();
    }
    if (playerBounds.top < 0.f) {
        this->youPlayer->setPosition(position.x, playerBounds.height / 2);
        this->youPlayer->stopMove();
    }
    else if ((playerBounds.top + playerBounds.height) > 800.f) {
        this->youPlayer->setPosition(position.x, 800.f - playerBounds.height);
        this->youPlayer->stopMove();
    }
}

void MainGameWindow::handlePlayerMovement(sf::Event& event) {
    const sf::Vector2i mousePosition{ sf::Mouse::getPosition(*this->windowPtr) };
    const sf::Vector2f mouseCoord{ this->windowPtr->mapPixelToCoords(mousePosition) };

    this->youPlayer->setTarget(mouseCoord);
}

void MainGameWindow::handleMouseClick(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            handlePlayerMovement(event);
        }
    }
}

void MainGameWindow::quitGame() {
    this->client->close();
    this->displayWindow = false;

    if (this->inGameSettings) {
        this->closeSettingsWindowFlag.store(true);
    }
}

void MainGameWindow::update(sf::Time deltaTime) {
    this->youPlayer->update(deltaTime);

    if (this->youPlayer->isMoving()) {
        checkPlayerWindowBorders();
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

    youPlayer = std::make_shared<Player>(sf::Vector2f(70.f, 70.f), sf::Color(2, 35, 89), sf::Color(31, 110, 2), 8.0f, 200.f);
    enemyPlayer = std::make_shared<Player>(sf::Vector2f(70.f, 70.f), sf::Color(2, 35, 89), sf::Color(110, 6, 2), 8.0f, 200.f);
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