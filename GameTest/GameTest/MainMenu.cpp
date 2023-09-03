#include "MainMenu.h"

MainMenu::MainMenu() {
    this->windowPtr = std::make_shared<sf::RenderWindow>();

    this->backgroundMusicPtr = std::make_shared<Music>();
    this->notificationSound = std::make_shared<Sound>();

	this->windowPtr->create(sf::VideoMode::getDesktopMode(), "SkyFall Showdown", sf::Style::None);
    this->windowPtr->setFramerateLimit(60);
}

void MainMenu::setTextures() {
    background.setTexture(MainMenuTextureManager::background);
    playBtn.setTexture(MainMenuTextureManager::playBtn);
    settingsBtn.setTexture(MainMenuTextureManager::settingsBtn);
    exitBtn.setTexture(MainMenuTextureManager::exitBtn);

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
    backgroundMusicPtr->setVolume(30.f);
    backgroundMusicPtr->play();
    backgroundMusicPtr->loop(true);

    notificationSound->setVolume(70.f);
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

    /* if true, a message from the server will be displayed on the screen */
    displayText = false;

    setTextures();
    initSprites();
    sf::Event event;

    bool exitRequested = false;
    while (windowPtr->isOpen() && !exitRequested) {
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowPtr->close();
            }

            handleKeyBoard(event, exitRequested);
            handleMouseCursor(event);
            handleButtonClicks(event, exitRequested);
        }
        renderWindow();
    }

    backgroundMusicPtr->getMusic().stop();
    return (exitRequested == false);
}

void MainMenu::initSprites() {
    float windowXSize = windowPtr->getSize().x;

    playBtn.getSprite().setPosition((windowXSize - playBtn.getTexture().getSize().x) / 2, 345.f);
    settingsBtn.getSprite().setPosition((windowXSize - settingsBtn.getTexture().getSize().x) / 2, 465.f);
    exitBtn.getSprite().setPosition((windowXSize - exitBtn.getTexture().getSize().x) / 2, 585.f);
}

void MainMenu::renderWindow() {
    windowPtr->clear();
    
    windowPtr->draw(background);
    windowPtr->draw(playBtn);
    windowPtr->draw(settingsBtn);
    windowPtr->draw(exitBtn);

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

void MainMenu::handleKeyBoard(sf::Event& event, bool& exitRequested) {
    /* OPEN THE EXIT MENU */
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        MenuConfirmationExit menuConfirmationExit;
        PopupReturnValues checker{};

        menuConfirmationExit.init(windowPtr, background, checker, defaultCursor, pointCursor);
        if (checker == PopupReturnValues::EXIT) {
            exitRequested = true;
        }
    }
}

void MainMenu::handleButtonClicks(sf::Event& event, bool& exitRequested) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

        /* OPEN THE NICKNAME MENU (then the match is requested if all is correct)*/
        if (playBtn.isInside(position)) {
            displayText = false;
            NicknameMenu nicknameMenu;
            PopupReturnValues checker{};

            std::string nick = nicknameMenu.init(windowPtr, background, checker, defaultCursor, pointCursor);
            if (checker == PopupReturnValues::DONE) {
                std::cout << "\nNickname: " << nick << "\n";

                /* start the connection thread */
                std::thread t(&MainMenu::handleClientConnection, this, nick);
                t.detach();
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
            MenuConfirmationExit menuConfirmationExit;
            PopupReturnValues checker{};

            menuConfirmationExit.init(windowPtr, background, checker, defaultCursor, pointCursor);
            if (checker == PopupReturnValues::EXIT) {
                exitRequested = true;
            }
        }
    }
}

bool MainMenu::handleConnectionMsg(const NetMessages& msg) {
    if (msg == NetMessages::NICK_EXITS) {
        /* Nick exists text */
        notificationSound->play();
        displayTextFuncTime(menuMsgs[1], 7);
        return false;
    }
    else {
        /* in queue for a match text */
        notificationSound->play();
        displayTextFunc(menuMsgs[3]);
    }

    return true;
}

void MainMenu::displayTextFunc(Entity& entity) {
    msgToDisplay = &entity;
    msgToDisplay->getSprite().setPosition(15.f, 480.f);
    displayText = true;
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

void MainMenu::handleClientConnection(std::string nick) {
    Client client;

    try {
        if (!client.connect("127.0.0.1", 8888)) {
            /* "Server Down" message */
            notificationSound->play();
            displayTextFuncTime(menuMsgs[0], 7);
        }
        else {
            NetMessages msg = NetUtils::read_(*client.getSocket()).getMsgType();

            if (msg == NetMessages::SERVER_FULL) {
                /* display the "Server Full" message */
                notificationSound->play();
                displayTextFuncTime(menuMsgs[2], 7);
                return;
            }
            else {
                /* send the nickname */
                NetUtils::send_(*client.getSocket(), NetPacket(NetMessages::IDLE, reinterpret_cast<const uint8_t*>(nick.c_str()), nick.size()));
                handleConnectionMsg(NetUtils::read_(*client.getSocket()).getMsgType());
            }
        }
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "\nError while connecting: " << e.what() << std::endl;
        /* "Server Down" message */
        notificationSound->play();
        displayTextFuncTime(menuMsgs[0], 7);
    }
}
