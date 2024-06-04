#include "MainMenu.h"

MainMenu::MainMenu() {
    this->windowPtr = std::make_shared<sf::RenderWindow>();

    this->backgroundMusicPtr = std::make_shared<Music>();
    this->notificationSound = std::make_shared<Sound>();

	this->windowPtr->create(sf::VideoMode(650, 600), "SkyFall Showdown", sf::Style::Close);
    this->windowPtr->setFramerateLimit(60);
}

bool MainMenu::init() {
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
            handleButtonClicks(event);
        }
        draw();
    }

    backgroundMusicPtr->getMusic().stop();
    return (exitRequested == false);
}

void MainMenu::setTextures() {
    matchText.setTexture(MainMenuTextureManager::matchText);
    settings2Text.setTexture(MainMenuTextureManager::settings2Text);
    settingsText.setTexture(MainMenuTextureManager::settingsText);
    undoMatchText.setTexture(MainMenuTextureManager::undoMatchText);
    mainText.setTexture(MainMenuTextureManager::mainLobbyText);
    quitText.setTexture(MainMenuTextureManager::quitText);

    for (int i = 0; i < 4; i++) {
        menuMsgs[i].setTexture(MainMenuTextureManager::menuMsg[i]);
    }
}

void MainMenu::initSprites() {
    float windowXSize = windowPtr->getSize().x;

    matchText.getSprite().setPosition((windowXSize - matchText.getTexture().getSize().x) / 2, 130.f);
    undoMatchText.getSprite().setPosition((windowXSize - undoMatchText.getTexture().getSize().x) / 2, 130.f);

    settingsText.getSprite().setPosition((windowXSize - settingsText.getTexture().getSize().x) / 2, 250.f);
    settings2Text.getSprite().setPosition((windowXSize - settings2Text.getTexture().getSize().x) / 2, 250.f);

    quitText.getSprite().setPosition((windowXSize - quitText.getTexture().getSize().x) / 2, 370.f);

    mainText.getSprite().setPosition(20.f, windowPtr->getSize().y - mainText.getSprite().getGlobalBounds().height - 10);
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

void MainMenu::draw() {
    windowPtr->clear();

    windowPtr->draw(mainText);
    windowPtr->draw(quitText);

    if (inMatchmaking.load()) {
        windowPtr->draw(settings2Text);
        windowPtr->draw(undoMatchText);
    }
    else {
        windowPtr->draw(settingsText);
        windowPtr->draw(matchText);
    }

    /* display the notification message from the server */
    if (displayText) {
        windowPtr->draw(*msgToDisplay);
    }
    
    windowPtr->display();
}

void MainMenu::handleKeyBoard(sf::Event& event) {
    /* OPEN THE EXIT MENU */
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        MenuConfirmationExit menuConfirmationExit;
        PopupReturnValues checker{};

        menuConfirmationExit.init(windowPtr, checker);
        if (checker == PopupReturnValues::EXIT) {
            this->exitRequested = true;
        }
    }
}

void MainMenu::handleButtonClicks(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

        /* UNDO MATCHMAKING */
        if (inMatchmaking.load() && undoMatchText.isInside(position)) {
            undoMatchmaking();
            return;
        }
        /* OPEN THE NICKNAME MENU */
        if (matchText.isInside(position) && !inMatchmaking.load()) {
             displayText = false;
             NicknameMenu nicknameMenu;
             PopupReturnValues checker{};

             std::string nick = nicknameMenu.init(windowPtr, checker);
             if (checker == PopupReturnValues::DONE) {
                /* OPEN THE CONNECT MENU */
                IpPortMenu ipPortMenu;
                std::pair<std::string, int> ipPort = ipPortMenu.init(windowPtr, notificationSound, checker);

                if (checker == PopupReturnValues::DONE) {
                    /* start the connection thread */
                    std::thread t(&MainMenu::handleClientConnection, this, nick, ipPort.first, ipPort.second);
                    t.detach();
                }
             }
        }
        /* SETTINGS MENU */
        else if (settingsText.isInside(position) && !inMatchmaking.load()) {
            OptionsMainMenu optionsMainMenu;
            PopupReturnValues checker{};

            optionsMainMenu.init(windowPtr, backgroundMusicPtr, checker);
        }
        /* EXIT MENU */
        else if (quitText.isInside(position)) {
            MenuConfirmationExit menuConfirmationExit;
            PopupReturnValues checker{};

            menuConfirmationExit.init(windowPtr, checker);
            if (checker == PopupReturnValues::EXIT) {
                undoMatchmaking();
                this->exitRequested = true;
            }
        }
    }
}

void MainMenu::handleClientConnection(std::string nick, std::string ip, int port) {
    try {
        if (!this->client->connect(ip, port)) {
            /* "Server Down" message */
            notificationSound->play();
            displayTextThread(menuMsgs[0], 7);
        }
        else {
            NetMessages msg = NetUtils::read_(*this->client->getSocket()).getMsgType();

            if (msg == NetMessages::SERVER_FULL) {
                /* display the "Server Full" message */
                notificationSound->play();
                displayTextThread(menuMsgs[2], 7);
            }
            else {
                /* send the nickname */
                NetUtils::write_(*this->client->getSocket(), NetPacket(NetMessages::IDLE, reinterpret_cast<const uint8_t*>(nick.c_str()), nick.size()));

                /* check the nickname */
                if (NetUtils::read_(*this->client->getSocket()).getMsgType() == NetMessages::NICK_EXITS) {
                    /* Nick exists text */
                    notificationSound->play();
                    this->client->getSocket()->close();

                    displayTextThread(menuMsgs[1], 7);
                }
                /* send the matchmaking request */
                else {
                    NetUtils::write_(*this->client->getSocket(), NetPacket(NetMessages::MATCHMAKING_REQUEST, nullptr, 0));
                    handleMatchmakingClient(NetUtils::read_(*this->client->getSocket()).getMsgType(), nick);
                }
            }
        }
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in handle client connection: " << e.what() << std::endl;
        /* "Server Down" message */
        notificationSound->play();
        displayTextThread(menuMsgs[0], 7);
    }
}

void MainMenu::handleMatchmakingClient(const NetMessages& msg, std::string nickname) {
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
            p = NetUtils::read_(*this->client->getSocket());

            if (p.getMsgType() == NetMessages::MATCH_FOUND) {
                std::cout << "\nStop listening match found\n";
                NetUtils::write_(*client->getSocket(), NetPacket(NetMessages::MATCH_FOUND, nullptr, 0));

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
}

void MainMenu::undoMatchmaking() {
    std::cout << "\nundo matchmaking.\n";
    try {
        inMatchmaking.store(false);
        NetUtils::write_(*this->client->getSocket(), NetPacket(NetMessages::UNDO_MATCHMAKING, nullptr, 0));
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError in undo matchmaking " << e.what() << std::endl;
        
    }
    this->client->close();
}

void MainMenu::exitMenu() {
    MenuConfirmationExit menuConfirmationExit;
    PopupReturnValues checker{};

    menuConfirmationExit.init(windowPtr, checker);
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

void MainMenu::displayTextFunc(Entity& entity) {
    msgToDisplay = &entity;
    msgToDisplay->getSprite().setPosition((windowPtr->getSize().x - entity.getTexture().getSize().x) / 2, 50.f);
    displayText = true;
}

void MainMenu::displayTextThread(Entity& entity, int seconds) {
    std::thread t([this, &entity, seconds]() {
        using namespace std::chrono_literals;
        
        this->msgToDisplay = &entity;
        this->msgToDisplay->getSprite().setPosition((windowPtr->getSize().x - entity.getTexture().getSize().x) / 2, 50);
        this->displayText = true;

        for (int i = 0; i < seconds; i++) {
            std::this_thread::sleep_for(1s);
        }
        displayText = false;
    });
    t.detach();
}