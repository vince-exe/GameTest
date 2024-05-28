#include "MainGameWindow.h"

void MainGameWindow::init(const std::string nickname, std::shared_ptr<Client> client) {
    this->client = client;

    this->windowPtr = std::make_shared<sf::RenderWindow>();
    this->windowPtr->create(sf::VideoMode(900, 600), "SkyFall Showdown", sf::Style::Close);
    this->windowPtr->setFramerateLimit(60);
   
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
    while (windowPtr->isOpen() && displayWindow) {
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->quitGame();
                return;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                ;
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

    float xPosition = xPositionGenerator(rng), yPosition = yPositionGenerator(rng);
    /* set the player to a random position*/
    youPlayer->setPosition(xPosition, yPosition);

    float playerPosition[2] = { xPosition, yPosition };
    /* send the position*/
    if (!NetUtils::write_(*this->client->getSocket(), NetPacket(NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(playerPosition), sizeof(playerPosition)))) {
        std::cerr << "\nError in sending position to the server";
    }   
    
    /* get the enemy position */
    NetPacket p = NetUtils::read_(*this->client->getSocket());
    if (p.getMsgType() == NetMessages::PLAYER_POSITION) {
        this->enemyPlayer->setPosition(p.getFloatVec()[0], p.getFloatVec()[1]);
        return true;
    }

    return false;
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
    const float speed = 10.f;
    sf::Vector2f velocity(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        velocity.y -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        velocity.y += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += speed;
    }

    if (velocity.x != 0.f || velocity.y != 0.f) {
        float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity /= length;
        velocity *= speed;
    }

    sf::Vector2f newPos = youPlayer->getPosition() + velocity;
    if (newPos.x < 0.f) newPos.x = 0.f;
    if (newPos.x > 900.f - 70.f) newPos.x = 900.f - 70.f; 
    if (newPos.y < 0.f) newPos.y = 0.f;
    if (newPos.y > 540.f - 70.f) newPos.y = 540.f - 70.f;

    youPlayer->move(newPos - youPlayer->getPosition());
    float sendPosition[2] = {youPlayer->getPosition().x, youPlayer->getPosition().y};

    NetUtils::write_(*this->client->getSocket(), NetPacket(NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(sendPosition), sizeof(sendPosition)));
}

void MainGameWindow::quitGame() {
    this->client->close();
    this->displayWindow = false;
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

    youPlayer = std::make_shared<Player>(sf::Vector2f(70.f, 70.f), sf::Color(2, 35, 89), sf::Color(31, 110, 2), 8.f);
    enemyPlayer = std::make_shared<Player>(sf::Vector2f(70.f, 70.f), sf::Color(2, 35, 89), sf::Color(110, 6, 2), 8.f);
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