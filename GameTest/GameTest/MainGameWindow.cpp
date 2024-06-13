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
    this->displayWindow = true;

    sf::Clock sprintClock;
    
    // Debug ( Temporary )
    std::cout << "\nPlayer: " << myNickname.getString().toAnsiString();

    while (this->displayWindow) {
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
            handleKeyBoards(event);
        }
        update(sprintClock.restart());
        draw();
    }
}

bool MainGameWindow::initPlayerAndEnemyPosition() {
    std::random_device dev;
    std::mt19937 rng(dev());
    /* generate the X and Y*/
    std::uniform_int_distribution<std::mt19937::result_type> xPositionGenerator(100, 1000);
    std::uniform_int_distribution<std::mt19937::result_type> yPositionGenerator(200, 700);

    float playerPosition[2];
    NetPacket p;
    /* check if they spawned at the same position */
    do {
        playerPosition[0] = xPositionGenerator(rng);
        playerPosition[1] = yPositionGenerator(rng);

        /* send the position*/
        if (!NetUtils::write_(*this->client->getSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(playerPosition), sizeof(playerPosition)))) {
            std::cerr << "\nError in sending position to the server";
            return false;
        }
        /* get the enemy position */
        p = NetUtils::read_(*this->client->getSocket());
        if (p.getMsgType() == NetPacket::NetMessages::PLAYER_POSITION) {
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
    Player::CollisionSide cL;

    while (true) {
        try {
            packet = NetUtils::read_(*this->client->getSocket());

            switch (packet.getMsgType()) {
            case NetPacket::NetMessages::QUIT_GAME:
                std::cout << "\nMatch end becasue [ " << this->enemyNickname.getString().toAnsiString() << " ] quit.";

                this->quitGame();
                return;

            case NetPacket::NetMessages::PLAYER_POSITION:
                this->enemyPlayer->setPosition(packet.getFloatVec()[0], packet.getFloatVec()[1]);
                break;

            case NetPacket::NetMessages::ENEMY_COLLISION:
                /* DEBUG ( TEMPORARY ) */
                cL = (Player::CollisionSide)packet.getData()[0];
                if (cL == Player::CollisionSide::Top) {
                    std::cout << "\nSono stato colpito dall'alto";
                }
                else if (cL == Player::CollisionSide::Bottom) {
                    std::cout << "\nSono stato colpito dal basso";
                }
                else if (cL == Player::CollisionSide::Left) {
                    std::cout << "\nSono stato colpito da sinistra";
                }
                else {
                    std::cout << "\nSono stato colpito da destra";
                }
                break;
            }
        }
        catch (const boost::system::system_error& ex) {
            std::cerr << "\nError in handleMessages() | " << ex.what();
            return;
        }
    }
    std::cout << "\n sono uscito";
}

void MainGameWindow::updateRechargeBar() {
    this->rechargeBarProgress = this->youPlayer->getClock().getElapsedTime().asSeconds() / this->youPlayer->getSprintTimeout();

    if (this->rechargeBarProgress > 1.0f) {
        this->rechargeBarProgress = 1.0f;
    }
    this->rechargeBar.setSize(sf::Vector2f(static_cast<float>(170 * this->rechargeBarProgress), 30.f));
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
    else if (playerBounds.top < 110.f) {
        this->youPlayer->setPosition(position.x, 110);
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
        if (!this->youPlayer->isSprinting()) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                handlePlayerMovement(event);
            }
            else if (event.mouseButton.button == sf::Mouse::Right) {
                if (this->youPlayer->canSprint()) {
                    this->youPlayer->startSprint();
                    handlePlayerMovement(event);
                }
            }
        }
    }
}

void MainGameWindow::handleKeyBoards(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            this->youPlayer->stopMove();
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
    this->youPlayer->update(deltaTime, this->enemyPlayer->getRect());
    
    updateRechargeBar();

    if (this->youPlayer->isMoving()) {
        checkPlayerWindowBorders();

        float p[2] = { this->youPlayer->getPosition().x, this->youPlayer->getPosition().y };
        NetUtils::write_(*this->client->getSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(p), sizeof(p)));
    }
    if (this->youPlayer->isSprinting() && this->youPlayer->isEnemyHit()) {
        Player::CollisionSide cL = this->youPlayer->getCollidedSide();

        NetUtils::write_(*this->client->getSocket(), NetPacket(NetPacket::NetMessages::ENEMY_COLLISION, (uint8_t*)&cL, sizeof(cL)));
        this->youPlayer->resetEnemyHit();
    }
}

void MainGameWindow::draw() {
    this->windowPtr->clear();

    this->windowPtr->draw(myNickname);
    this->windowPtr->draw(enemyNickname);
    this->windowPtr->draw(*youPlayer);
    this->windowPtr->draw(*enemyPlayer);
    this->windowPtr->draw(rechargeBarBorder);
    this->windowPtr->draw(rechargeBar);
    this->windowPtr->draw(vsText);

    for (int i = 0; i < 3; i++) {
        this->windowPtr->draw(youHealth[i]);
        this->windowPtr->draw(enemyHealth[i]);
    }

    this->windowPtr->display();
}

void MainGameWindow::initSprites() {
    rechargeBarBorder.setSize(sf::Vector2f(170.f, 30.f));
    rechargeBarBorder.setPosition(1000.f, 30.f);
    rechargeBarBorder.setFillColor(sf::Color::Transparent);
    rechargeBarBorder.setOutlineThickness(3);
    rechargeBarBorder.setOutlineColor(sf::Color(128, 103, 36));

    rechargeBar.setSize(sf::Vector2f(0.f, 30.f));
    rechargeBar.setPosition(1000.f, 30.f);
    rechargeBar.setFillColor(sf::Color(196, 154, 39));

    myNickname.setFont(FontManager::fredokaOne);
    myNickname.setCharacterSize(35);
    myNickname.setPosition(20.f, 22.f);
    myNickname.setFillColor(sf::Color(31, 110, 2));
   
    vsText.setFont(FontManager::fredokaOne);
    vsText.setCharacterSize(30);
    vsText.setPosition(myNickname.getGlobalBounds().left + myNickname.getGlobalBounds().width + vsText.getGlobalBounds().width + 20.f, 24.f);
    vsText.setFillColor(sf::Color(219, 219, 219));
    vsText.setString("vs");

    enemyNickname.setFont(FontManager::fredokaOne);
    enemyNickname.setCharacterSize(35);
    enemyNickname.setPosition(vsText.getGlobalBounds().left + vsText.getGlobalBounds().width + 20.f, 22.f);
    enemyNickname.setFillColor(sf::Color(110, 6, 2));

    youPlayer = std::make_shared<Player>(sf::Vector2f(70.f, 70.f), sf::Color(2, 35, 89), sf::Color(31, 110, 2), 8.0f, 200.f, 1000.f, 4.f);
    enemyPlayer = std::make_shared<Player>(sf::Vector2f(70.f, 70.f), sf::Color(2, 35, 89), sf::Color(110, 6, 2), 8.0f, 200.f, 1000.f, 4.f);


    float youHealthPosX = 850.f;
    float enemyHealthPosX = 720.f;

    for (int i = 0; i < 3; i++) {
        youHealth[i].setSize(sf::Vector2f(18.f, 18.f));
        youHealth[i].setFillColor(sf::Color(31, 110, 2));
        youHealth[i].setPosition(youHealthPosX, youHealth[i].getGlobalBounds().height + rechargeBarBorder.getGlobalBounds().height / 2);

        enemyHealth[i].setSize(sf::Vector2f(18.f, 18.f));
        enemyHealth[i].setPosition(enemyHealthPosX, enemyHealth[i].getGlobalBounds().height + rechargeBarBorder.getGlobalBounds().height / 2);
        enemyHealth[i].setFillColor(sf::Color(110, 6, 2));
        
        youHealthPosX += 30.f;
        enemyHealthPosX += 30.f;
    }
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