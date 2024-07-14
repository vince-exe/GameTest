#include "MainGameWindow.h"

void MainGameWindow::init(const std::string nickname, std::shared_ptr<Client> client, TextureManager& textureManager, FontManager& fontManager) {
    m_Client = client;

    m_Window.create(sf::VideoMode(1200, 800), "SkyFall Showdown", sf::Style::Close);
    m_Window.setFramerateLimit(60);

    m_closeSettingsWindowFlag.store(false);
    m_inGameSettings = false;
    m_Game.setBlockActions(true);

    /* get the enemy nickname */
    if (!handleEnemyNickname()) {
        quitGame();
        return;
    }
    m_myNickname.setString(nickname);

    initSprites(fontManager);

    /* try to get the default position of the player and enemy player*/
    if (!initPlayerAndEnemyPosition()) {
        quitGame();
        return;
    }

    /* start the thread to listen for game messages */
    std::thread t(&MainGameWindow::handleMessages, this);
    t.detach();

    sf::Event event;
    m_displayWindow = true;
    sf::Clock sprintClock;

    resolvePlayerSprint();
    //DEBUG
    std::cout << "\nPlayer: " << nickname;
    while (m_displayWindow) {
        while (m_Window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                quitGame();
                return;
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                m_inGameSettings = true;
                if (m_gameSettingsMenu.init(m_Window, textureManager, &m_closeSettingsWindowFlag)) {
                    return;
                }
                m_inGameSettings = false;
            }
            handleMouseClick(event);
            handleKeyBoards(event);
        }
        if (m_Game.getGameState() == Game::GameStates::RUNNING) {
            update(sprintClock.restart());
        }
        else if (m_Game.getGameState() == Game::GameStates::END) {
            if (!m_Game.hasEnemyQuit()) {
                NetUtils::write_(*m_Client->getSocket(), NetPacket(NetPacket::NetMessages::GAME_END, nullptr, 0));
            }
            m_closeSettingsWindowFlag.store(true);

            m_endGameWindow.init(m_Window, m_Game, textureManager, fontManager, m_myNickname, m_vsText, m_enemyNickname);
            m_Window.close();
            return;
        }
        draw();
    }
}

bool MainGameWindow::initPlayerAndEnemyPosition() {
    NetPacket packet = NetUtils::read_(*m_Client->getSocket());
    if (packet.getMsgType() == NetPacket::NetMessages::PLAYER_POSITION) {
        sf::Vector2f pos = NetGameUtils::getSfvector2f(packet);

        m_Game.setPlayerStartPosition(pos);
        m_youPlayer->setPosition(pos);
    }

    packet = NetUtils::read_(*m_Client->getSocket());
    if (packet.getMsgType() == NetPacket::NetMessages::PLAYER_POSITION) {
        m_enemyPlayer->setPosition(NetGameUtils::getSfvector2f(packet));

        return true;
    }
    return false;
}

void MainGameWindow::handleMessages() {
    NetPacket packet;
    while (true) {
        try {
            packet = NetUtils::read_(*m_Client->getSocket());

            switch (packet.getMsgType()) {
            case NetPacket::NetMessages::GAME_END:
                m_Client->close();
                return;
                /* if the enemy quit */
            case NetPacket::NetMessages::QUIT_GAME:
                m_Client->close();
                m_Game.handleEnemyQuit();
                return;

            case NetPacket::NetMessages::PLAYER_POSITION:
                m_enemyPlayer->setPosition(NetGameUtils::getSfvector2f(packet));
                break;

            case NetPacket::NetMessages::ENEMY_COLLISION:
                m_youPlayer->handleEnemyCollision((Player::CollisionSide)packet.getData()[0]);
                break;

            case NetPacket::NetMessages::GAME_STARTED:
                m_Game.waitRound(m_waitRoundText);
                m_Game.setDamageAreasCords(NetGameUtils::getDamageAreasCoordinates(packet));
                /* set the vector of damages areas */
                m_Game.startGame(m_damageAreasVector);
                m_Game.startTimer(m_gameTimer);
                break;

            case NetPacket::NetMessages::ENEMY_COLLISION_W_DAMAGE_AREA:
                m_youPlayer->stopMove();
                m_Game.waitRound(m_waitRoundText);
                m_Game.handleNewRound(Game::GameEntities::ENEMY);

                /* set the player to the start position and send the position to the enemy */
                m_youPlayer->setPosition(m_Game.getStartPlayerPosition());
                float p[] = { m_youPlayer->getPosition().x, m_youPlayer->getPosition().y };
                NetUtils::write_(*m_Client->getSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(p), sizeof(p)));
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
    m_rechargeBarProgress = m_youPlayer->getClock().getElapsedTime().asSeconds() / m_youPlayer->getSprintTimeout();

    if (m_rechargeBarProgress > 1.0f) {
        m_rechargeBarProgress = 1.0f;
    }
    m_rechargeBar.setSize(sf::Vector2f(static_cast<float>(170 * m_rechargeBarProgress), 30.f));
}

void MainGameWindow::resolvePlayerSprint() {
    /*
       this piece of code ensure that when the player will sprint for the first time, it will all works fine
       without this the first sprint of the player won't work as aspected ( i can't figure out why. )
    */
    sf::Vector2f pos = m_youPlayer->getPosition();
    m_youPlayer->setTarget(sf::Vector2f(1000, 1000));
    m_youPlayer->startSprint(false);
    m_youPlayer->setPosition(pos);
}

void MainGameWindow::checkPlayerWindowBorders() {
    sf::FloatRect playerBounds = m_youPlayer->getGlobalBounds();
    sf::Vector2f position = m_youPlayer->getPosition();

    if (playerBounds.left < 0.f) {
        m_youPlayer->setPosition(playerBounds.width / 2, position.y);
        m_youPlayer->stopMove();
    }
    else if ((playerBounds.left + playerBounds.width) > 1200.f) {
        m_youPlayer->setPosition(1200.f - playerBounds.width, position.y);
        m_youPlayer->stopMove();
    }
    else if (playerBounds.top < 110.f) {
        m_youPlayer->setPosition(position.x, 110);
        m_youPlayer->stopMove();
    }
    else if ((playerBounds.top + playerBounds.height) > 800.f) {
        m_youPlayer->setPosition(position.x, 800.f - playerBounds.height);
        m_youPlayer->stopMove();
    }
}

void MainGameWindow::handleMouseClick(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            m_Game.handlePlayerMovement(*m_youPlayer, m_Window, false);
        }
        else if (event.mouseButton.button == sf::Mouse::Right) {
            m_Game.handlePlayerMovement(*m_youPlayer, m_Window, true);
        }
    }
}

void MainGameWindow::handleKeyBoards(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            m_youPlayer->stopMove();
        }
    }
}

void MainGameWindow::quitGame() {
    m_Client->close();
    m_displayWindow = false;

    if (m_inGameSettings) {
        m_closeSettingsWindowFlag.store(true);
    }
}

void MainGameWindow::update(sf::Time deltaTime) {
    m_youPlayer->update(deltaTime, m_enemyPlayer->getRect());

    updateRechargeBar();

    if (m_youPlayer->isMoving()) {
        checkPlayerWindowBorders();
        /* send the position */
        float p[2] = { m_youPlayer->getPosition().x, m_youPlayer->getPosition().y };
        NetUtils::write_(*m_Client->getSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(p), sizeof(p)));

        if (m_youPlayer->isSprinting() && m_youPlayer->isEnemyHit()) {
            Player::CollisionSide cL = m_youPlayer->getCollidedSide();

            NetUtils::write_(*m_Client->getSocket(), NetPacket(NetPacket::NetMessages::ENEMY_COLLISION, (uint8_t*)&cL, sizeof(cL)));
            m_youPlayer->stopMove();
            m_youPlayer->resetEnemyHit();
        }

        /* check if the player collided with a damage area */
        if (m_Game.checkCollision(m_damageAreasVector.at(m_Game.getCurrentRound()), *m_youPlayer)) {
            m_youPlayer->stopMove();
            m_Game.waitRound(m_waitRoundText);
            m_Game.handleNewRound(Game::GameEntities::PLAYER);

            NetUtils::write_(*m_Client->getSocket(), NetPacket(NetPacket::NetMessages::ENEMY_COLLISION_W_DAMAGE_AREA, nullptr, 0));

            m_youPlayer->setPosition(m_Game.getStartPlayerPosition());
            float p[] = { m_youPlayer->getPosition().x, m_youPlayer->getPosition().y };

            NetUtils::write_(*m_Client->getSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(p), sizeof(p)));
        }
    }
}

void MainGameWindow::draw() {
    m_Window.clear();

    m_Window.draw(m_myNickname);
    m_Window.draw(m_enemyNickname);
    m_Window.draw(*m_youPlayer);
    m_Window.draw(*m_enemyPlayer);
    m_Window.draw(m_rechargeBarBorder);
    m_Window.draw(m_rechargeBar);
    m_Window.draw(m_vsText);

    for (int i = 0; i < m_Game.getPlayerLife(); i++) {
        m_Window.draw(m_youHealth[i]);
    }
    for (int i = 0; i < m_Game.getEnemyLife(); i++) {
        m_Window.draw(m_enemyHealth[i]);
    }
    if (m_Game.getGameState() == Game::GameStates::RUNNING) {
        m_Window.draw(m_gameTimer);

        for (sf::CircleShape& shape : m_damageAreasVector.at(m_Game.getCurrentRound())) {
            m_Window.draw(shape);
        }
    }
    /* draw the 3..2..1 text */
    if (m_Game.areActionsBlocked()) {
        m_Window.draw(m_waitRoundText);
    }
    m_Window.display();
}

void MainGameWindow::initSprites(FontManager& fontManager) {
    m_rechargeBarBorder.setSize(sf::Vector2f(170.f, 30.f));
    m_rechargeBarBorder.setPosition(1000.f, 30.f);
    m_rechargeBarBorder.setFillColor(sf::Color::Transparent);
    m_rechargeBarBorder.setOutlineThickness(3);
    m_rechargeBarBorder.setOutlineColor(sf::Color(128, 103, 36));

    m_rechargeBar.setSize(sf::Vector2f(0.f, 30.f));
    m_rechargeBar.setPosition(1000.f, 30.f);
    m_rechargeBar.setFillColor(sf::Color(196, 154, 39));

    m_myNickname.setFont(fontManager.getFredokaOne());
    m_myNickname.setCharacterSize(35);
    m_myNickname.setPosition(20.f, 22.f);
    m_myNickname.setFillColor(sf::Color(31, 110, 2));

    m_vsText.setFont(fontManager.getFredokaOne());
    m_vsText.setCharacterSize(30);
    m_vsText.setPosition(m_myNickname.getGlobalBounds().left + m_myNickname.getGlobalBounds().width + m_vsText.getGlobalBounds().width + 20.f, 24.f);
    m_vsText.setFillColor(sf::Color(219, 219, 219));
    m_vsText.setString("vs");

    m_enemyNickname.setFont(fontManager.getFredokaOne());
    m_enemyNickname.setCharacterSize(35);
    m_enemyNickname.setPosition(m_vsText.getGlobalBounds().left + m_vsText.getGlobalBounds().width + 20.f, 22.f);
    m_enemyNickname.setFillColor(sf::Color(110, 6, 2));

    m_gameTimer.setFont(fontManager.getFredokaOne());
    m_gameTimer.setCharacterSize(30);
    m_gameTimer.setPosition(600.f, m_gameTimer.getGlobalBounds().height + (m_rechargeBarBorder.getGlobalBounds().height / 2) + 4.f);
    m_gameTimer.setFillColor(sf::Color(219, 219, 219));

    m_waitRoundText.setFont(fontManager.getFredokaOne());
    m_waitRoundText.setCharacterSize(80);
    m_waitRoundText.setFillColor(sf::Color(255, 255, 255));
    m_waitRoundText.setPosition((m_Window.getSize().x / 2.f) - (m_waitRoundText.getGlobalBounds().width / 2.f), (m_Window.getSize().y / 3.f) - (m_waitRoundText.getGlobalBounds().height / 2.f));

    m_youPlayer = std::make_shared<Player>(sf::Vector2f(70.f, 70.f), sf::Color(2, 35, 89), sf::Color(31, 110, 2), 8.0f, 200.f, 1000.f, 4.f);
    m_enemyPlayer = std::make_shared<Player>(sf::Vector2f(70.f, 70.f), sf::Color(2, 35, 89), sf::Color(110, 6, 2), 8.0f, 200.f, 1000.f, 4.f);

    float youHealthPosX = 850.f;
    float enemyHealthPosX = 720.f;

    for (int i = 0; i < 3; i++) {
        m_youHealth[i].setSize(sf::Vector2f(18.f, 18.f));
        m_youHealth[i].setFillColor(sf::Color(31, 110, 2));
        m_youHealth[i].setPosition(youHealthPosX, m_youHealth[i].getGlobalBounds().height + m_rechargeBarBorder.getGlobalBounds().height / 2);

        m_enemyHealth[i].setSize(sf::Vector2f(18.f, 18.f));
        m_enemyHealth[i].setPosition(enemyHealthPosX, m_enemyHealth[i].getGlobalBounds().height + m_rechargeBarBorder.getGlobalBounds().height / 2);
        m_enemyHealth[i].setFillColor(sf::Color(110, 6, 2));

        youHealthPosX += 30.f;
        enemyHealthPosX += 30.f;
    }

    for (int i = 0; i < 3; i++) {
        std::vector<sf::CircleShape> vec;
        for (int j = 0; j < 6; j++) {
            vec.push_back(sf::CircleShape());

            vec.at(j).setRadius(60.f);
            vec.at(j).setOutlineThickness(8.f);
            vec.at(j).setFillColor(sf::Color(120, 36, 14));
            vec.at(j).setOutlineColor(sf::Color(82, 20, 5));
        }
        m_damageAreasVector.push_back(vec);
    }
}

bool MainGameWindow::handleEnemyNickname() {
    try {
        NetPacket p = NetUtils::read_(*m_Client->getSocket());
        m_enemyNickname.setString(NetGameUtils::getString(p));
        return true;
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in handle enemy nickname | " << e.what() << "\n";
        return false;
    }
}