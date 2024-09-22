#include "MainMenu.h"

MainMenu::MainMenu() {
	m_Window.create(sf::VideoMode(650, 600), "SkyFall Showdown", sf::Style::Close);
    m_Window.setFramerateLimit(60);
}

bool MainMenu::init() {
    setMusicAndSound();
    
    /* Menu Variables */
    m_displayText = false;
    m_exitRequested = false;
    m_displayGameWindow = false;
    /* for matchmaking */
    m_inMatchmaking.store(false);

    setTextures();
    initSprites();
    sf::Event event;

    while (m_Window.isOpen() && !m_exitRequested) {
        if (m_displayGameWindow) {
            m_Window.setVisible(false);
            g_aSingleton.getBackgroundMusic().stop();

            MainGameWindow mainGameWindow;
            mainGameWindow.init(m_Nickname, m_Client);

            g_aSingleton.getBackgroundMusic().setVolume(g_sSingleton.getValue(SkyfallUtils::Settings::MUSIC_VOLUME).GetInt());
            g_aSingleton.getBackgroundMusic().play();
            g_aSingleton.getBackgroundMusic().loop(true);
            m_displayGameWindow = false;
            m_displayText = false;
            m_Window.setVisible(true);
        }
        while (m_Window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_Window.close();
            }
            handleKeyBoard(event);
            handleButtonClicks(event);
        }
        draw();
    }

    g_aSingleton.getBackgroundMusic().stop();
    return (m_exitRequested == false);
}

void MainMenu::setTextures() {
    m_matchText.setTexture(g_tSingleton.getMatchBtnTexture());
    m_settings2Text.setTexture(g_tSingleton.getSettings2BtnTexture());
    m_settingsText.setTexture(g_tSingleton.getSettingsBtnTexture());
    m_undoMatchText.setTexture(g_tSingleton.getUndoMatchTexture());
    m_mainText.setTexture(g_tSingleton.getTextImage(4));
    m_quitText.setTexture(g_tSingleton.getQuitBtnTexture());

    for (int i = 0; i < 4; i++) {
        m_menuMsgs[i].setTexture(g_tSingleton.getTextImage(i));
    }
}

void MainMenu::initSprites() {
    float windowXSize = m_Window.getSize().x;

    m_matchText.getSprite().setPosition((windowXSize - m_matchText.getTexture().getSize().x) / 2, 130.f);
    m_undoMatchText.getSprite().setPosition((windowXSize - m_undoMatchText.getTexture().getSize().x) / 2, 130.f);

    m_settingsText.getSprite().setPosition((windowXSize - m_settingsText.getTexture().getSize().x) / 2, 250.f);
    m_settings2Text.getSprite().setPosition((windowXSize - m_settings2Text.getTexture().getSize().x) / 2, 250.f);

    m_quitText.getSprite().setPosition((windowXSize - m_quitText.getTexture().getSize().x) / 2, 370.f);
   
    m_mainText.getSprite().setPosition(20.f, m_Window.getSize().y - m_mainText.getSprite().getGlobalBounds().height - 10);
}

void MainMenu::setMusicAndSound() {
    Music& backMusic = g_aSingleton.getBackgroundMusic();
    backMusic.setVolume(g_sSingleton.getValue(SkyfallUtils::Settings::MUSIC_VOLUME).GetInt());
    backMusic.play();
    backMusic.loop(true);

    g_aSingleton.setSoundEffectsVolume(g_sSingleton.getValue(SkyfallUtils::Settings::SOUND_EFFECTS_VOLUME).GetInt());
}

void MainMenu::draw() {
    m_Window.clear();

    m_Window.draw(m_mainText);
    m_Window.draw(m_quitText);

    if (m_inMatchmaking.load()) {
        m_Window.draw(m_settings2Text);
        m_Window.draw(m_undoMatchText);
    }
    else {
        m_Window.draw(m_settingsText);
        m_Window.draw(m_matchText);
    }

    /* display the notification message from the server */
    if (m_displayText) {
        m_Window.draw(*m_msgToDisplay);
    }
    
    m_Window.display();
}

void MainMenu::handleKeyBoard(sf::Event& event) {
    /* OPEN THE EXIT MENU */
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        MenuConfirmationExit menuConfirmationExit;
        SkyfallUtils::WindowsReturnValues checker{};

        menuConfirmationExit.init(m_Window, checker);
        if (checker == SkyfallUtils::WindowsReturnValues::EXIT) {
            m_exitRequested = true;
        }
    }
}

void MainMenu::handleButtonClicks(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f position = m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Window));

        /* UNDO MATCHMAKING */
        if (m_inMatchmaking.load() && m_undoMatchText.isInside(position)) {
            g_aSingleton.getButtonClickSound().play();
            undoMatchmaking();
            return;
        }
        /* OPEN THE NICKNAME MENU */
        if (m_matchText.isInside(position) && !m_inMatchmaking.load()) {
             g_aSingleton.getButtonClickSound().play();
             m_displayText = false;
             NicknameMenu m_NicknameMenu;
             SkyfallUtils::WindowsReturnValues checker{};

             std::string nick = m_NicknameMenu.init(m_Window, checker);
             if (checker == SkyfallUtils::WindowsReturnValues::DONE) {
                /* OPEN THE CONNECT MENU */
                IpPortMenu ipPortMenu;
                std::pair<std::string, int> ipPort = ipPortMenu.init(m_Window, checker);
                std::cout << "\nIp: " << ipPort.first << "   Port: " << ipPort.second; // debug

                if (checker == SkyfallUtils::WindowsReturnValues::DONE) {
                    /* start the connection thread */
                    std::thread t(&MainMenu::handleClientConnection, this, nick, ipPort.first, ipPort.second);
                    t.detach();
                }
             }
        }
        /* SETTINGS MENU */
        else if (m_settingsText.isInside(position) && !m_inMatchmaking.load()) {
            g_aSingleton.getButtonClickSound().play();
            OptionsMainMenu optionsMainMenu;
            SkyfallUtils::WindowsReturnValues checker{};

            optionsMainMenu.init(m_Window, checker);
        }
        /* EXIT MENU */
        else if (m_quitText.isInside(position)) {
            g_aSingleton.getButtonClickSound().play();
            MenuConfirmationExit menuConfirmationExit;
            SkyfallUtils::WindowsReturnValues checker{};

            menuConfirmationExit.init(m_Window, checker);
            if (checker == SkyfallUtils::WindowsReturnValues::EXIT) {
                undoMatchmaking();
                m_exitRequested = true;
            }
        }
    }
}

void MainMenu::handleClientConnection(std::string nick, std::string ip, int port) {
    try {
        if (!m_Client.connect(ip, port)) {
            g_aSingleton.getErrorSound().play();    
            m_Client.close();
            displayTextThread(m_menuMsgs[0], 7); // "Server Down" message.
        } 
        else {
            // initialize the udp endpoint
            if (!m_Client.resolveUdpEndpoint(ip, port + 1)) {
                g_aSingleton.getErrorSound().play();
                m_Client.close();
                displayTextThread(m_menuMsgs[0], 7);
                return;
            }
            NetPacket::NetMessages msg = NetUtils::Tcp::read_(*m_Client.getSocket()).getMsgType();

            if (msg == NetPacket::NetMessages::SERVER_FULL) {
                g_aSingleton.getErrorSound().play();
                displayTextThread(m_menuMsgs[2], 7); // display the "Server Full" message
            }
            else {
                /* send the m_Nickname */
                NetUtils::Tcp::write_(*m_Client.getSocket(), NetPacket(NetPacket::NetMessages::IDLE, reinterpret_cast<const uint8_t*>(nick.c_str()), nick.size()));

                /* check the m_Nickname */
                if (NetUtils::Tcp::read_(*m_Client.getSocket()).getMsgType() == NetPacket::NetMessages::NICK_EXITS) {
                    g_aSingleton.getErrorSound().play();
                    m_Client.getSocket()->close();
                    displayTextThread(m_menuMsgs[1], 7); // Nick exists text
                    
                }
                /* send the udp connection and after the matchmaking request */
                else {
                    m_Client.openUdpSocket();
                    NetUtils::Udp::write_(*m_Client.getUdpSocket(), m_Client.getUdpEndpoint(), NetUdpPacket(nick, NetUdpPacket::UdpMessages::CONNECTION_MESSAGE, boost::uuids::uuid(), NetPacket::NetMessages::IDLE, nullptr, 0, 0));

                    /* check if the udp connection went fine */
                    if (NetUtils::Tcp::read_(*m_Client.getSocket()).getMsgType() != NetPacket::NetMessages::UDP_CONNECTION_SUCCESS) {
                        g_aSingleton.getErrorSound().play();
                        m_Client.getSocket()->close();
                        displayTextThread(m_menuMsgs[0], 7);
                        return;
                    }

                    NetUtils::Tcp::write_(*m_Client.getSocket(), NetPacket(NetPacket::NetMessages::MATCHMAKING_REQUEST, nullptr, 0));
                    handleMatchmakingClient(NetUtils::Tcp::read_(*m_Client.getSocket()).getMsgType(), nick);
                }
            }
        }
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in handle m_Client connection: " << e.what() << std::endl;
        /* "Server Down" message */
        displayTextThread(m_menuMsgs[0], 7);
        m_Client.close();
    }
}

void MainMenu::handleMatchmakingClient(const NetPacket::NetMessages& msg, std::string m_Nickname) {
    if (msg == NetPacket::NetMessages::WAIT_FOR_MATCH) {
        g_aSingleton.getMatchmakingSound().play(); // "in queue for a match" text
        displayTextFunc(m_menuMsgs[3]);

        /* start a thread to listen if the matchmaking is requested */
        std::thread t(&MainMenu::listenMatchmaking, this, m_Nickname);
        t.detach();
    }
    /* start the game window */
    else if (msg == NetPacket::NetMessages::MATCH_FOUND) {
        matchFound(m_Nickname);
    }
}

void MainMenu::listenMatchmaking(const std::string nick) {
    using namespace std::chrono_literals;

    m_inMatchmaking.store(true);
    try {
        NetPacket packet = NetUtils::Tcp::read_(*m_Client.getSocket());
        if (packet.getMsgType() == NetPacket::NetMessages::MATCH_FOUND) {
            NetUtils::Tcp::write_(*m_Client.getSocket(), packet);
            matchFound(nick);
        }
    }
    catch (boost::system::error_code& e) {
        std::cout << "\nerror in listening for matchmaking";
    }
    catch (std::runtime_error& e) {
        std::cout << "\nruntime error: " << e.what();
    }
    m_displayText = false;
    m_inMatchmaking.store(false);
}

void MainMenu::undoMatchmaking() {
    try {
        if (!m_displayGameWindow) {

            m_inMatchmaking.store(false);
            m_Client.close();
        }
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in undo matchmaking " << e.what() << std::endl;
        
    }
}

void MainMenu::exitMenu() {
    MenuConfirmationExit menuConfirmationExit;
    SkyfallUtils::WindowsReturnValues checker{};

    menuConfirmationExit.init(m_Window, checker);
    if (checker == SkyfallUtils::WindowsReturnValues::EXIT) {
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
    m_msgToDisplay->getSprite().setPosition((m_Window.getSize().x - entity.getTexture().getSize().x) / 2, 50.f);
    m_displayText = true;
}

void MainMenu::displayTextThread(Entity& entity, int seconds) {
    std::thread t([this, &entity, seconds]() {
        using namespace std::chrono_literals;
        
        m_msgToDisplay = &entity;
        m_msgToDisplay->getSprite().setPosition((m_Window.getSize().x - entity.getTexture().getSize().x) / 2, 50);
        m_displayText = true;

        for (int i = 0; i < seconds; i++) {
            std::this_thread::sleep_for(1s);
        }
        m_displayText = false;
    });
    t.detach();
}