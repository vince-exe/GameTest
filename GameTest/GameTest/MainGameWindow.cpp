#include "MainGameWindow.h"

void MainGameWindow::init(const std::string nickname, std::shared_ptr<Client> client) {
    this->client = client;
    
    this->windowPtr = std::make_shared<sf::RenderWindow>();

    this->windowPtr->create(sf::VideoMode(1200, 800), "SkyFall Showdown", sf::Style::Close);
    this->windowPtr->setFramerateLimit(60);

    this->closeSettingsWindowFlag.store(false);
    this->inGameSettings = false;
    m_Game.setBlockActions(true);

    /* get the enemy nickname */
    if (!handleEnemyNickname()) {
        this->quitGame();
        return;
    }
    this->myNickname.setString(nickname);

    m_Game.initSprites(*this->windowPtr);
    initSprites();
    
    /* try to get the default position of the player and enemy player*/
    if (!initPlayerAndEnemyPosition()) {
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
    NetPacket packet = NetUtils::read_(*this->client->getSocket());
    if (packet.getMsgType() == NetPacket::NetMessages::PLAYER_POSITION) {
        sf::Vector2f pos = NetGameUtils::getSfvector2f(packet);

        m_Game.setPlayerStartPosition(pos);
        this->youPlayer->setPosition(pos);
    }

    packet = NetUtils::read_(*this->client->getSocket());
    if (packet.getMsgType() == NetPacket::NetMessages::PLAYER_POSITION) {
        this->enemyPlayer->setPosition(NetGameUtils::getSfvector2f(packet));

        return true;
    }
    return false;
}

void MainGameWindow::handleMessages() {
    NetPacket packet;
    while (true) {
        try {
            packet = NetUtils::read_(*this->client->getSocket());

            switch (packet.getMsgType()) {
            case NetPacket::NetMessages::QUIT_GAME:
                std::cout << "\nMatch end becasue [ " << this->enemyNickname.getString().toAnsiString() << " ] quit.";

                this->quitGame();
                return;

            case NetPacket::NetMessages::PLAYER_POSITION:
                this->enemyPlayer->setPosition(NetGameUtils::getSfvector2f(packet));
                break;

            case NetPacket::NetMessages::ENEMY_COLLISION:
                this->youPlayer->handleEnemyCollision((Player::CollisionSide)packet.getData()[0]);
                break;

            case NetPacket::NetMessages::GAME_STARTED:
                m_Game.waitRound();
                m_Game.setDamageAreasCords(NetGameUtils::getDamageAreasCoordinates(packet));
                m_Game.startGame();
                break;
            
            case NetPacket::NetMessages::ENEMY_COLLISION_W_DAMAGE_AREA:
                m_Game.waitRound();
                m_Game.reduceEnemyLife();
                this->youPlayer->setPosition(m_Game.getStartPlayerPosition());
                this->youPlayer->stopMove();

                float p[] = { this->youPlayer->getPosition().x, this->youPlayer->getPosition().y };
                NetUtils::write_(*this->client->getSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(p), sizeof(p)));
                break;
            }
        }
        catch (const boost::system::system_error& ex) {
            std::cerr << "\nError in handleMessages() | " << ex.what();
            return;
        }
    }
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

void MainGameWindow::handleMouseClick(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            /* REFACTORING: THE GAME CLASS SHOULD HAVE THE PROPERTY OF THE PLAYER OBJECT */
            m_Game.handlePlayerMovement(event, *this->youPlayer, *this->windowPtr, false);
        }
        else if (event.mouseButton.button == sf::Mouse::Right) {
            m_Game.handlePlayerMovement(event, *this->youPlayer, *this->windowPtr, true);
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
        /* send the position */
        float p[2] = { this->youPlayer->getPosition().x, this->youPlayer->getPosition().y };
        NetUtils::write_(*this->client->getSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(p), sizeof(p)));

        /* check if the player collided with a damage area */
        if (m_Game.checkCollision(*this->youPlayer)) {
            this->youPlayer->stopMove();
            m_Game.waitRound();
            
            NetUtils::write_(*this->client->getSocket(), NetPacket(NetPacket::NetMessages::ENEMY_COLLISION_W_DAMAGE_AREA, nullptr, 0));
            m_Game.reducePlayerLife();
            
            this->youPlayer->setPosition(m_Game.getStartPlayerPosition());
            float p[] = { this->youPlayer->getPosition().x, this->youPlayer->getPosition().y };

            NetUtils::write_(*this->client->getSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(p), sizeof(p)));            
            return;
        }
        if (this->youPlayer->isSprinting() && this->youPlayer->isEnemyHit()) {
            Player::CollisionSide cL = this->youPlayer->getCollidedSide();

            NetUtils::write_(*this->client->getSocket(), NetPacket(NetPacket::NetMessages::ENEMY_COLLISION, (uint8_t*)&cL, sizeof(cL)));
            this->youPlayer->stopMove();
            this->youPlayer->resetEnemyHit();
        }
    }
}

void MainGameWindow::draw() {
    this->windowPtr->clear();

    if (m_Game.areActionsBlocked()) {
        m_Game.drawWaitRoundText(*this->windowPtr);
    }

    this->windowPtr->draw(myNickname);
    this->windowPtr->draw(enemyNickname);
    this->windowPtr->draw(*youPlayer);
    this->windowPtr->draw(*enemyPlayer);
    this->windowPtr->draw(rechargeBarBorder);
    this->windowPtr->draw(rechargeBar);
    this->windowPtr->draw(vsText);
    
    for (int i = 0; i < m_Game.getPlayerLife(); i++) {
        this->windowPtr->draw(youHealth[i]);
    }
    for (int i = 0; i < m_Game.getEnemyLife(); i++) {
        this->windowPtr->draw(enemyHealth[i]);
    }
    
    if (m_Game.isGameStarted()) {
        m_Game.drawDamageAreasShapes(*this->windowPtr);
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
        this->enemyNickname.setString(NetGameUtils::getString(p));
        return true;
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in handle enemy nickname | " << e.what() << "\n";
        return false;
    }
}