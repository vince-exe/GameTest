#include "MainMenu.h"

MainMenu::MainMenu() {
    this->windowPtr = std::make_shared<sf::RenderWindow>();

    this->backgroundMusicPtr = std::make_shared<Music>();
    this->notificationSound = std::make_shared<Sound>();

	this->windowPtr->create(sf::VideoMode::getDesktopMode(), "SkyFall Showdown", sf::Style::Close);
    this->windowPtr->setFramerateLimit(60);
}

bool MainMenu::init() {
    if (!loadMouse()) {
        std::cout << "\n[ Error ]: Failed to load the cursor ( MainMenu )\n";
        return false;
    }
    if (!loadMusicSound()) {
        std::cout << "\n[ Error ]: Failed to load the music/sound ( MainMenu )\n";
        return false;
    }
    setMusicSound();

    /* Menu Variables */
    displayText = false;
    exitRequested = false;
    displayGameWindow = false;
    /* for matchmaking */
    inMatchmaking.store(false);

    this->client = std::make_shared<Client>();

    setTextures();
    initSprites();
    sf::Event event;

    while (windowPtr->isOpen() && !exitRequested) {
        if (displayGameWindow) {
            this->windowPtr->setVisible(false);

            MainGameWindow mainGameWindow;
            mainGameWindow.init(this->nickname, this->client);

            displayGameWindow = false;
            displayText = false;
            this->windowPtr->setVisible(true);
        }
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowPtr->close();
            }

            handleKeyBoard(event);
            handleMouseCursor(event);
            handleButtonClicks(event);
        }
        renderWindow();
    }

    backgroundMusicPtr->getMusic().stop();
    return (exitRequested == false);
}

void MainMenu::setTextures() {
    background.setTexture(MainMenuTextureManager::background);
    playBtn.setTexture(MainMenuTextureManager::playBtn);
    settingsBtn.setTexture(MainMenuTextureManager::settingsBtn);
    exitBtn.setTexture(MainMenuTextureManager::exitBtn);
    undoMatchBtn.setTexture(MainMenuTextureManager::undoMatchBtn);

    for (int i = 0; i < 4; i++) {
        menuMsgs[i].setTexture(MainMenuTextureManager::menuMsg[i]);
    }
}

bool MainMenu::loadMouse() {
    if (!defaultCursor.loadFromPixels(MainMenuTextureManager::cursorTextureGrab.copyToImage().getPixelsPtr(), MainMenuTextureManager::cursorTextureGrab.getSize(), { 0, 0 }) ||
        !pointCursor.loadFromPixels(MainMenuTextureManager::cursorTexturePoint.copyToImage().getPixelsPtr(), MainMenuTextureManager::cursorTexturePoint.getSize(), { 0, 0 })) {
        return false;
    }

    windowPtr->setMouseCursor(defaultCursor);
    return true;
}

bool MainMenu::loadMusicSound() {
    return backgroundMusicPtr->openFromFile("assets/Music-Sound/Background_Menu_Music.ogg") &&
        notificationSound->openFromFile("assets/Music-Sound/notification.ogg");
}

void MainMenu::setMusicSound() {
    backgroundMusicPtr->setVolume(SettingsManager::getValue("VolumeMenu").GetInt());
    backgroundMusicPtr->play();
    backgroundMusicPtr->loop(true);

    notificationSound->setVolume(70.f);
}

void MainMenu::initSprites() {
    float windowXSize = windowPtr->getSize().x;

    playBtn.getSprite().setPosition((windowXSize - playBtn.getTexture().getSize().x) / 2, 345.f);
    undoMatchBtn.getSprite().setPosition((windowXSize - playBtn.getTexture().getSize().x) / 2, 345.f);

    settingsBtn.getSprite().setPosition((windowXSize - settingsBtn.getTexture().getSize().x) / 2, 465.f);
    exitBtn.getSprite().setPosition((windowXSize - exitBtn.getTexture().getSize().x) / 2, 585.f);
}

void MainMenu::renderWindow() {
    windowPtr->clear();
    
    windowPtr->draw(background);
    windowPtr->draw(playBtn);
    windowPtr->draw(settingsBtn);
    windowPtr->draw(exitBtn);

    if (inMatchmaking.load()) {
        windowPtr->draw(undoMatchBtn);
    }
    else {
        windowPtr->draw(playBtn);
    }

    /* display the notification message from the server */
    if (displayText) {
        windowPtr->draw(*msgToDisplay);
    }
    
    windowPtr->display();
}

void MainMenu::handleMouseCursor(sf::Event& event) {
    /* change the mouse cursor from handle to point if it is on a button */
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

        if (playBtn.isInside(position) || settingsBtn.isInside(position) || exitBtn.isInside(position)) {
            windowPtr->setMouseCursor(pointCursor);
        }
        else {
            windowPtr->setMouseCursor(defaultCursor);
        }
    }
}

void MainMenu::handleKeyBoard(sf::Event& event) {
    /* OPEN THE EXIT MENU */
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        MenuConfirmationExit menuConfirmationExit;
        PopupReturnValues checker{};

        menuConfirmationExit.init(windowPtr, background, checker, defaultCursor, pointCursor);
        if (checker == PopupReturnValues::EXIT) {
            this->exitRequested = true;
        }
    }
}

void MainMenu::handleButtonClicks(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

        /* UNDO MATCHMAKING */
        if (inMatchmaking.load() && undoMatchBtn.isInside(position)) {
            undoMatchmaking();
            return;
        }
        /* OPEN THE NICKNAME MENU */
        if (playBtn.isInside(position) && !inMatchmaking.load()) {
             displayText = false;
             NicknameMenu nicknameMenu;
             PopupReturnValues checker{};

             std::string nick = nicknameMenu.init(windowPtr, background, checker, defaultCursor, pointCursor);
             if (checker == PopupReturnValues::DONE) {
                std::cout << "\nNickname: " << nick << "\n";
                /* OPEN THE CONNECT MENU */
                IpPortMenu ipPortMenu;

                std::pair<std::string, int> ipPort = ipPortMenu.init(windowPtr, notificationSound, background, checker, defaultCursor, pointCursor);

                if (checker == PopupReturnValues::DONE) {
                    std::cout << "\nIp Address: " << ipPort.first << "\tPort: " << ipPort.second << "\n";
                    /* start the connection thread */
                    std::thread t(&MainMenu::handleClientConnection, this, nick, ipPort.first, ipPort.second);
                    t.detach();
                }
             }
        }
        /* SETTINGS MENU */
        else if (settingsBtn.isInside(position)) {
            OptionsMainMenu optionsMainMenu;
            PopupReturnValues checker{};

            optionsMainMenu.init(windowPtr, backgroundMusicPtr, background, checker);
        }
        /* EXIT MENU */
        else if (exitBtn.isInside(position)) {
            exitMenu();
        }
    }
}

void MainMenu::handleMatchmakingResponse(const NetMessages& msg, std::string nickname) {
    if (msg == NetMessages::WAIT_FOR_MATCH) {
        /* in queue for a match text */
        notificationSound->play();
        displayTextFunc(menuMsgs[3]);

        /* start a thread to listen if the matchmaking is requested */
        std::thread t(&MainMenu::listenForMatchmaking, this, nickname);
        t.detach();
    }
    /* start the game window */
    else if (msg == NetMessages::MATCH_FOUND) {
        std::cout << "\nmatch found\n";
        this->matchFound(nickname);
    }
}

void MainMenu::listenForMatchmaking(std::string nickname) {
    using namespace std::chrono_literals;

    NetPacket p;
    inMatchmaking.store(true);
   
    this->client->getSocket()->non_blocking(true);

    while (inMatchmaking.load()) {
        try {
            std::this_thread::sleep_for(500ms);

            std::cout << "\nlistening for matchmaking..";
            
            p = NetUtils::read_(*this->client->getSocket());

            if (p.getMsgType() == NetMessages::MATCH_FOUND) {
                std::cout << "\nStop listening match found\n";
                NetUtils::send_(*client->getSocket(), NetPacket(NetMessages::MATCH_FOUND, nullptr, 0));

                this->client->getSocket()->non_blocking(false);
                this->matchFound(nickname);
                return;
            }
        }
        catch (const boost::system::system_error& e) {
            if (e.code() != boost::asio::error::would_block) {
                std::cerr << "\nCatch in listen for matchmaking: " << e.what() << std::endl;

                this->client->getSocket()->close();
                inMatchmaking.store(false);
                this->displayText = false;
            }
        }
    }   
    std::cout << "\nstop listening for matchmaking.\n";
}

void MainMenu::handleClientConnection(std::string nick, std::string ip, int port) {
    try {
        if (!this->client->connect(ip, port)) {
            /* "Server Down" message */
            notificationSound->play();
            displayTextFuncTime(menuMsgs[0], 7);
        }
        else {
            NetMessages msg = NetUtils::read_(*this->client->getSocket()).getMsgType();

            if (msg == NetMessages::SERVER_FULL) {
                /* display the "Server Full" message */
                notificationSound->play();
                displayTextFuncTime(menuMsgs[2], 7);
                return;
            }
            else {
                /* send the nickname */
                NetUtils::send_(*this->client->getSocket(), NetPacket(NetMessages::IDLE, reinterpret_cast<const uint8_t*>(nick.c_str()), nick.size()));

                /* check the nickname */
                if (NetUtils::read_(*this->client->getSocket()).getMsgType() == NetMessages::NICK_EXITS) {
                    /* Nick exists text */
                    notificationSound->play();
                    displayTextFuncTime(menuMsgs[1], 7);
                }
                /* send the matchmaking request */
                else {
                    NetUtils::send_(*this->client->getSocket(), NetPacket(NetMessages::MATCHMAKING_REQUEST, nullptr, 0));
                    handleMatchmakingResponse(NetUtils::read_(*this->client->getSocket()).getMsgType(), nick);
                }
            }
        }
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in handle client connection: " << e.what() << std::endl;
        /* "Server Down" message */
        notificationSound->play();
        displayTextFuncTime(menuMsgs[0], 7);
    }
    std::cout << "\nexit thread handle client connection...\n";
}

void MainMenu::displayTextFunc(Entity& entity) {
    msgToDisplay = &entity;
    msgToDisplay->getSprite().setPosition(15.f, 480.f);
    displayText = true;
}

void MainMenu::undoMatchmaking() {
    std::cout << "\nundo matchmaking.\n";
    try {
        inMatchmaking.store(false);
        NetUtils::send_(*this->client->getSocket(), NetPacket(NetMessages::UNDO_MATCHMAKING, nullptr, 0));
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in undo matchmaking " << e.what() << std::endl;
        
    }
    this->client->close();
}

void MainMenu::exitMenu() {
    MenuConfirmationExit menuConfirmationExit;
    PopupReturnValues checker{};

    menuConfirmationExit.init(windowPtr, background, checker, defaultCursor, pointCursor);
    if (checker == PopupReturnValues::EXIT) {
        /* if is in matchmaking */
        if (inMatchmaking.load()) {
            undoMatchmaking();
        }
        this->exitRequested = true;
    }
}

void MainMenu::matchFound(std::string nickname) {
    this->inMatchmaking.store(false);
    this->displayGameWindow = true;
    this->nickname = nickname;
}

void MainMenu::displayTextFuncTime(Entity& entity, int seconds) {
    std::thread t([this, &entity, seconds]() {
        using namespace std::chrono_literals;
        
        this->msgToDisplay = &entity;
        this->msgToDisplay->getSprite().setPosition(15.f, 480.f);
        this->displayText = true;

        for (int i = 0; i < seconds; i++) {
            std::this_thread::sleep_for(1s);
        }
        displayText = false;
    });
    t.detach();
}
