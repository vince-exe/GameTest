#include "MainMenu.h"

MainMenu::MainMenu() {
    m_Window = std::make_shared<sf::RenderWindow>();

    m_backgroundMusicPtr = std::make_shared<Music>();
    m_notificationSound = std::make_shared<Sound>();

	m_Window->create(sf::VideoMode(650, 600), "SkyFall Showdown", sf::Style::Close);
    m_Window->setFramerateLimit(60);
}

bool MainMenu::init() {
    if (!loadMusicSound()) {
        std::cout << "\n[ Error ]: Failed to load the music/sound ( MainMenu )\n";
        return false;
    }
    setMusicSound();
    
    /* Menu Variables */
    m_displayText = false;
    m_exitRequested = false;
    m_displayGameWindow = false;
    /* for matchmaking */
    m_inMatchmaking.store(false);

    m_Client = std::make_shared<Client>();

    setTextures();
    initSprites();
    sf::Event event;

    while (m_Window->isOpen() && !m_exitRequested) {
        if (m_displayGameWindow) {
            m_Window->setVisible(false);
            
            MainGameWindow mainGameWindow;
            mainGameWindow.init(m_Nickname, m_Client);

            m_displayGameWindow = false;
            m_displayText = false;
            m_Window->setVisible(true);
        }
        while (m_Window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_Window->close();
            }
            handleKeyBoard(event);
            handleButtonClicks(event);
        }
        draw();
    }

    m_backgroundMusicPtr->getMusic().stop();
    return (m_exitRequested == false);
}

void MainMenu::setTextures() {
    m_matchText.setTexture(MainMenuTextureManager::matchText);
    m_settings2Text.setTexture(MainMenuTextureManager::settings2Text);
    m_settingsText.setTexture(MainMenuTextureManager::settingsText);
    m_undoMatchText.setTexture(MainMenuTextureManager::undoMatchText);
    m_mainText.setTexture(MainMenuTextureManager::mainLobbyText);
    m_quitText.setTexture(MainMenuTextureManager::quitText);

    for (int i = 0; i < 4; i++) {
        m_menuMsgs[i].setTexture(MainMenuTextureManager::menuMsg[i]);
    }
}

void MainMenu::initSprites() {
    float windowXSize = m_Window->getSize().x;

    m_matchText.getSprite().setPosition((windowXSize - m_matchText.getTexture().getSize().x) / 2, 130.f);
    m_undoMatchText.getSprite().setPosition((windowXSize - m_undoMatchText.getTexture().getSize().x) / 2, 130.f);

    m_settingsText.getSprite().setPosition((windowXSize - m_settingsText.getTexture().getSize().x) / 2, 250.f);
    m_settings2Text.getSprite().setPosition((windowXSize - m_settings2Text.getTexture().getSize().x) / 2, 250.f);

    m_quitText.getSprite().setPosition((windowXSize - m_quitText.getTexture().getSize().x) / 2, 370.f);

    m_mainText.getSprite().setPosition(20.f, m_Window->getSize().y - m_mainText.getSprite().getGlobalBounds().height - 10);
}

bool MainMenu::loadMusicSound() {
    return m_backgroundMusicPtr->openFromFile("assets/Music-Sound/Background_Menu_Music.ogg") &&
           m_notificationSound->openFromFile("assets/Music-Sound/notification.ogg");
}

void MainMenu::setMusicSound() {
    m_backgroundMusicPtr->setVolume(SettingsManager::getValue("VolumeMenu").GetInt());
    m_backgroundMusicPtr->play();
    m_backgroundMusicPtr->loop(true);

    m_notificationSound->setVolume(70.f);
}

void MainMenu::draw() {
    m_Window->clear();

    m_Window->draw(m_mainText);
    m_Window->draw(m_quitText);

    if (m_inMatchmaking.load()) {
        m_Window->draw(m_settings2Text);
        m_Window->draw(m_undoMatchText);
    }
    else {
        m_Window->draw(m_settingsText);
        m_Window->draw(m_matchText);
    }

    /* display the notification message from the server */
    if (m_displayText) {
        m_Window->draw(*m_msgToDisplay);
    }
    
    m_Window->display();
}

void MainMenu::handleKeyBoard(sf::Event& event) {
    /* OPEN THE EXIT MENU */
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        MenuConfirmationExit menuConfirmationExit;
        UiUtils::WindowsReturnValues checker{};

        menuConfirmationExit.init(m_Window, checker);
        if (checker == UiUtils::WindowsReturnValues::EXIT) {
            m_exitRequested = true;
        }
    }
}

void MainMenu::handleButtonClicks(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f position = m_Window->mapPixelToCoords(sf::Mouse::getPosition(*m_Window));

        /* UNDO MATCHMAKING */
        if (m_inMatchmaking.load() && m_undoMatchText.isInside(position)) {
            undoMatchmaking();
            return;
        }
        /* OPEN THE NICKNAME MENU */
        if (m_matchText.isInside(position) && !m_inMatchmaking.load()) {
             m_displayText = false;
             NicknameMenu m_NicknameMenu;
             UiUtils::WindowsReturnValues checker{};

             std::string nick = m_NicknameMenu.init(m_Window, checker);
             if (checker == UiUtils::WindowsReturnValues::DONE) {
                /* OPEN THE CONNECT MENU */
                IpPortMenu ipPortMenu;
                std::pair<std::string, int> ipPort = ipPortMenu.init(m_Window, m_notificationSound, checker);

                if (checker == UiUtils::WindowsReturnValues::DONE) {
                    /* start the connection thread */
                    std::thread t(&MainMenu::handleClientConnection, this, nick, ipPort.first, ipPort.second);
                    t.detach();
                }
             }
        }
        /* SETTINGS MENU */
        else if (m_settingsText.isInside(position) && !m_inMatchmaking.load()) {
            OptionsMainMenu optionsMainMenu;
            UiUtils::WindowsReturnValues checker{};

            optionsMainMenu.init(m_Window, m_backgroundMusicPtr, checker);
        }
        /* EXIT MENU */
        else if (m_quitText.isInside(position)) {
            MenuConfirmationExit menuConfirmationExit;
            UiUtils::WindowsReturnValues checker{};

            menuConfirmationExit.init(m_Window, checker);
            if (checker == UiUtils::WindowsReturnValues::EXIT) {
                undoMatchmaking();
                m_exitRequested = true;
            }
        }
    }
}

void MainMenu::handleClientConnection(std::string nick, std::string ip, int port) {
    try {
        if (!m_Client->connect(ip, port)) {
            /* "Server Down" message */
            m_notificationSound->play();
            displayTextThread(m_menuMsgs[0], 7);
        }
        else {
            NetPacket::NetMessages msg = NetUtils::read_(*m_Client->getSocket()).getMsgType();

            if (msg == NetPacket::NetMessages::SERVER_FULL) {
                /* display the "Server Full" message */
                m_notificationSound->play();
                displayTextThread(m_menuMsgs[2], 7);
            }
            else {
                /* send the m_Nickname */
                NetUtils::write_(*m_Client->getSocket(), NetPacket(NetPacket::NetMessages::IDLE, reinterpret_cast<const uint8_t*>(nick.c_str()), nick.size()));

                /* check the m_Nickname */
                if (NetUtils::read_(*m_Client->getSocket()).getMsgType() == NetPacket::NetMessages::NICK_EXITS) {
                    /* Nick exists text */
                    m_notificationSound->play();
                    m_Client->getSocket()->close();

                    displayTextThread(m_menuMsgs[1], 7);
                }
                /* send the matchmaking request */
                else {
                    NetUtils::write_(*m_Client->getSocket(), NetPacket(NetPacket::NetMessages::MATCHMAKING_REQUEST, nullptr, 0));
                    handleMatchmakingClient(NetUtils::read_(*m_Client->getSocket()).getMsgType(), nick);
                }
            }
        }
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in handle m_Client connection: " << e.what() << std::endl;
        /* "Server Down" message */
        m_notificationSound->play();
        displayTextThread(m_menuMsgs[0], 7);
    }
}

void MainMenu::handleMatchmakingClient(const NetPacket::NetMessages& msg, std::string m_Nickname) {
    if (msg == NetPacket::NetMessages::WAIT_FOR_MATCH) {
        /* in queue for a match text */
        m_notificationSound->play();
        displayTextFunc(m_menuMsgs[3]);

        /* start a thread to listen if the matchmaking is requested */
        std::thread t(&MainMenu::listenForMatchmaking, this, m_Nickname);
        t.detach();
    }
    /* start the game window */
    else if (msg == NetPacket::NetMessages::MATCH_FOUND) {
        matchFound(m_Nickname);
    }
}

void MainMenu::listenForMatchmaking(std::string m_Nickname) {
    using namespace std::chrono_literals;

    NetPacket p;
    m_inMatchmaking.store(true);
   
    m_Client->getSocket()->non_blocking(true);

    while (m_inMatchmaking.load()) {
        try {
            std::this_thread::sleep_for(500ms);            
            p = NetUtils::read_(*m_Client->getSocket());

            if (p.getMsgType() == NetPacket::NetMessages::MATCH_FOUND) {
                NetUtils::write_(*m_Client->getSocket(), NetPacket(NetPacket::NetMessages::MATCH_FOUND, nullptr, 0));

                m_Client->getSocket()->non_blocking(false);
                matchFound(m_Nickname);
                return;
            }
        }
        catch (const boost::system::system_error& e) {
            if (e.code() != boost::asio::error::would_block) {
                std::cerr << "\nCatch in listen for matchmaking: " << e.what() << std::endl;

                m_Client->getSocket()->close();
                m_inMatchmaking.store(false);
                m_displayText = false;
            }
        }
    }   
}

void MainMenu::undoMatchmaking() {
    std::cout << "\nundo matchmaking.\n";
    try {
        m_inMatchmaking.store(false);
        NetUtils::write_(*m_Client->getSocket(), NetPacket(NetPacket::NetMessages::UNDO_MATCHMAKING, nullptr, 0));
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in undo matchmaking " << e.what() << std::endl;
        
    }
    m_Client->close();
}

void MainMenu::exitMenu() {
    MenuConfirmationExit menuConfirmationExit;
    UiUtils::WindowsReturnValues checker{};

    menuConfirmationExit.init(m_Window, checker);
    if (checker == UiUtils::WindowsReturnValues::EXIT) {
        /* if is in matchmaking */
        if (m_inMatchmaking.load()) {
            undoMatchmaking();
        }
        m_exitRequested = true;
    }
}

void MainMenu::matchFound(std::string nick) {
    m_inMatchmaking.store(false);
    m_displayGameWindow = true;
    m_Nickname = nick;
}

void MainMenu::displayTextFunc(Entity& entity) {
    m_msgToDisplay = &entity;
    m_msgToDisplay->getSprite().setPosition((m_Window->getSize().x - entity.getTexture().getSize().x) / 2, 50.f);
    m_displayText = true;
}

void MainMenu::displayTextThread(Entity& entity, int seconds) {
    std::thread t([this, &entity, seconds]() {
        using namespace std::chrono_literals;
        
        m_msgToDisplay = &entity;
        m_msgToDisplay->getSprite().setPosition((m_Window->getSize().x - entity.getTexture().getSize().x) / 2, 50);
        m_displayText = true;

        for (int i = 0; i < seconds; i++) {
            std::this_thread::sleep_for(1s);
        }
        m_displayText = false;
    });
    t.detach();
}