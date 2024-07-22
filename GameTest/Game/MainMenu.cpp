#include "MainMenu.h"

MainMenu::MainMenu() {
	m_Window.create(sf::VideoMode(650, 600), "SkyFall Showdown", sf::Style::Close);
    m_Window.setFramerateLimit(60);
}

bool MainMenu::init(TextureManager& textureManager, FontManager& fontManager, SettingsManager& settingsManager, AudioManager& audioManager) {
    setMusicAndSound(settingsManager, audioManager);
    
    /* Menu Variables */
    m_displayText = false;
    m_exitRequested = false;
    m_displayGameWindow = false;
    /* for matchmaking */
    m_inMatchmaking.store(false);

    setTextures(textureManager);
    initSprites();
    sf::Event event;

    while (m_Window.isOpen() && !m_exitRequested) {
        if (m_displayGameWindow) {
            m_Window.setVisible(false);
            audioManager.getBackgroundMusic().stop();

            MainGameWindow mainGameWindow;
            mainGameWindow.init(m_Nickname, m_Client, textureManager, fontManager, settingsManager, audioManager);

            audioManager.getBackgroundMusic().play();
            audioManager.getBackgroundMusic().loop(true);
            m_displayGameWindow = false;
            m_displayText = false;
            m_Window.setVisible(true);
        }
        while (m_Window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_Window.close();
            }
            handleKeyBoard(event, textureManager, audioManager);
            handleButtonClicks(event, textureManager, fontManager, settingsManager, audioManager);
        }
        draw();
    }

    audioManager.getBackgroundMusic().stop();
    return (m_exitRequested == false);
}

void MainMenu::setTextures(TextureManager& textureManager) {
    m_matchText.setTexture(textureManager.getMatchBtnTexture());
    m_settings2Text.setTexture(textureManager.getSettings2BtnTexture());
    m_settingsText.setTexture(textureManager.getSettingsBtnTexture());
    m_undoMatchText.setTexture(textureManager.getUndoMatchTexture());
    m_mainText.setTexture(textureManager.getTextImage(4));
    m_quitText.setTexture(textureManager.getQuitBtnTexture());

    for (int i = 0; i < 4; i++) {
        m_menuMsgs[i].setTexture(textureManager.getTextImage(i));
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

void MainMenu::setMusicAndSound(SettingsManager& settingsManager, AudioManager& audioManager) {
    Music& backMusic = audioManager.getBackgroundMusic();
    backMusic.setVolume(settingsManager.getValue(SkyfallUtils::Settings::MUSIC_VOLUME).GetInt());
    backMusic.play();
    backMusic.loop(true);

    audioManager.setSoundEffectsVolume(settingsManager.getValue(SkyfallUtils::Settings::SOUND_EFFECTS_VOLUME).GetInt());
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

void MainMenu::handleKeyBoard(sf::Event& event, TextureManager& textureManager, AudioManager& audioManager) {
    /* OPEN THE EXIT MENU */
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        MenuConfirmationExit menuConfirmationExit;
        SkyfallUtils::WindowsReturnValues checker{};

        menuConfirmationExit.init(m_Window, textureManager, audioManager, checker);
        if (checker == SkyfallUtils::WindowsReturnValues::EXIT) {
            m_exitRequested = true;
        }
    }
}

void MainMenu::handleButtonClicks(sf::Event& event, TextureManager& textureManager, FontManager& fontManager, SettingsManager& settingsManager, AudioManager& audioManager) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f position = m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Window));

        /* UNDO MATCHMAKING */
        if (m_inMatchmaking.load() && m_undoMatchText.isInside(position)) {
            audioManager.getButtonClickSound().play();
            undoMatchmaking();
            return;
        }
        /* OPEN THE NICKNAME MENU */
        if (m_matchText.isInside(position) && !m_inMatchmaking.load()) {
             audioManager.getButtonClickSound().play();
             m_displayText = false;
             NicknameMenu m_NicknameMenu;
             SkyfallUtils::WindowsReturnValues checker{};

             std::string nick = m_NicknameMenu.init(m_Window, textureManager, fontManager, audioManager, checker);
             if (checker == SkyfallUtils::WindowsReturnValues::DONE) {
                /* OPEN THE CONNECT MENU */
                IpPortMenu ipPortMenu;
                std::pair<std::string, int> ipPort = ipPortMenu.init(m_Window, textureManager, fontManager, settingsManager, audioManager, checker);
                std::cout << "\nIp: " << ipPort.first << "   Port: " << ipPort.second; // debug

                if (checker == SkyfallUtils::WindowsReturnValues::DONE) {
                    /* start the connection thread */
                    std::thread t(&MainMenu::handleClientConnection, this, nick, ipPort.first, ipPort.second, std::ref(audioManager));
                    t.detach();
                }
             }
        }
        /* SETTINGS MENU */
        else if (m_settingsText.isInside(position) && !m_inMatchmaking.load()) {
            audioManager.getButtonClickSound().play();
            OptionsMainMenu optionsMainMenu;
            SkyfallUtils::WindowsReturnValues checker{};

            optionsMainMenu.init(m_Window, textureManager, settingsManager, fontManager, audioManager, checker);
        }
        /* EXIT MENU */
        else if (m_quitText.isInside(position)) {
            audioManager.getButtonClickSound().play();
            MenuConfirmationExit menuConfirmationExit;
            SkyfallUtils::WindowsReturnValues checker{};

            menuConfirmationExit.init(m_Window, textureManager, audioManager, checker);
            if (checker == SkyfallUtils::WindowsReturnValues::EXIT) {
                undoMatchmaking();
                m_exitRequested = true;
            }
        }
    }
}

void MainMenu::handleClientConnection(std::string nick, std::string ip, int port, AudioManager& audioManager) {
    try {
        if (!m_Client.connect(ip, port)) {
            audioManager.getErrorSound().play();    
            displayTextThread(m_menuMsgs[0], 7); // "Server Down" message.
        } 
        else {
            NetPacket::NetMessages msg = NetUtils::Tcp::read_(*m_Client.getSocket()).getMsgType();

            if (msg == NetPacket::NetMessages::SERVER_FULL) {
                audioManager.getErrorSound().play();
                displayTextThread(m_menuMsgs[2], 7); // display the "Server Full" message
            }
            else {
                /* send the m_Nickname */
                NetUtils::Tcp::write_(*m_Client.getSocket(), NetPacket(NetPacket::NetMessages::IDLE, reinterpret_cast<const uint8_t*>(nick.c_str()), nick.size()));

                /* check the m_Nickname */
                if (NetUtils::Tcp::read_(*m_Client.getSocket()).getMsgType() == NetPacket::NetMessages::NICK_EXITS) {
                    audioManager.getErrorSound().play();
                    m_Client.getSocket()->close();
                    displayTextThread(m_menuMsgs[1], 7); // Nick exists text
                    
                }
                /* send the matchmaking request */
                else {
                    NetUtils::Tcp::write_(*m_Client.getSocket(), NetPacket(NetPacket::NetMessages::MATCHMAKING_REQUEST, nullptr, 0));
                    handleMatchmakingClient(NetUtils::Tcp::read_(*m_Client.getSocket()).getMsgType(), audioManager, nick);
                }
            }
        }
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in handle m_Client connection: " << e.what() << std::endl;
        /* "Server Down" message */
        displayTextThread(m_menuMsgs[0], 7);
    }
}

void MainMenu::handleMatchmakingClient(const NetPacket::NetMessages& msg, AudioManager& audioManager, std::string m_Nickname) {
    if (msg == NetPacket::NetMessages::WAIT_FOR_MATCH) {
        audioManager.getMatchmakingSound().play(); // "in queue for a match" text
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
   
    m_Client.getSocket()->non_blocking(true);

    while (m_inMatchmaking.load()) {
        try {
            std::this_thread::sleep_for(500ms);            
            p = NetUtils::Tcp::read_(*m_Client.getSocket());

            if (p.getMsgType() == NetPacket::NetMessages::MATCH_FOUND) {
                NetUtils::Tcp::write_(*m_Client.getSocket(), NetPacket(NetPacket::NetMessages::MATCH_FOUND, nullptr, 0));

                m_Client.getSocket()->non_blocking(false);
                matchFound(m_Nickname);
                return;
            }
        }
        catch (const boost::system::system_error& e) {
            if (e.code() != boost::asio::error::would_block) {
                std::cerr << "\nCatch in listen for matchmaking: " << e.what() << std::endl;

                m_Client.getSocket()->close();
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
        NetUtils::Tcp::write_(*m_Client.getSocket(), NetPacket(NetPacket::NetMessages::UNDO_MATCHMAKING, nullptr, 0));
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in undo matchmaking " << e.what() << std::endl;
        
    }
    m_Client.close();
}

void MainMenu::exitMenu(TextureManager& textureManager, AudioManager& audioManager) {
    MenuConfirmationExit menuConfirmationExit;
    SkyfallUtils::WindowsReturnValues checker{};

    menuConfirmationExit.init(m_Window, textureManager, audioManager, checker);
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