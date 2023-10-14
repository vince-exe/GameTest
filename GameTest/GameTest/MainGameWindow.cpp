#include "MainGameWindow.h"

void MainGameWindow::init(std::shared_ptr<sf::RenderWindow> windowPtr_, const std::string nickname, std::shared_ptr<Client> client) {
	/// tmp this->windowPtr = windowPtr;
    
    this->client = client;
    
    this->windowPtr = std::make_shared<sf::RenderWindow>();
    this->windowPtr->create(sf::VideoMode::getDesktopMode(), "SkyFall Showdown", sf::Style::None);
    this->windowPtr->setFramerateLimit(60);
    
    //handleEnemyNickname();
    this->myNickname.setString(nickname);
    this->enemyNickname.setString("DRAGOFINALE012");
    sf::Event event;

    setTextures();
    initSprites();

    while (windowPtr->isOpen()) {
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowPtr->close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                ;
            }
        }
        draw();
    }
}

void MainGameWindow::draw() {
    this->windowPtr->clear();

    this->windowPtr->draw(background);
    this->windowPtr->draw(line);
    this->windowPtr->draw(myNickname);
    this->windowPtr->draw(enemyNickname);
    
    for (int i = 0; i < 4; i++) {
        this->windowPtr->draw(myCheckBtns[i]);
        this->windowPtr->draw(enemyCheckBtn[i]);
        this->windowPtr->draw(myAnimalCounters[i]);
    }
   
    this->windowPtr->display();
}

void MainGameWindow::setTextures() {
    this->background.setTexture(MainGameTextureManager::background);

    myCheckBtns[0].setTexture(MainGameTextureManager::catCheckBtn);
    myCheckBtns[1].setTexture(MainGameTextureManager::rabbitCheckBtn);
    myCheckBtns[2].setTexture(MainGameTextureManager::foxCheckBtn);
    myCheckBtns[3].setTexture(MainGameTextureManager::mouseCheckBtn);

    enemyCheckBtn[0].setTexture(MainGameTextureManager::catCheckBtn);
    enemyCheckBtn[1].setTexture(MainGameTextureManager::rabbitCheckBtn);
    enemyCheckBtn[2].setTexture(MainGameTextureManager::foxCheckBtn);
    enemyCheckBtn[3].setTexture(MainGameTextureManager::mouseCheckBtn);

    myAnimalCounters[0].setTexture(MainGameTextureManager::catCounter);
    myAnimalCounters[1].setTexture(MainGameTextureManager::rabbitCounter);
    myAnimalCounters[2].setTexture(MainGameTextureManager::foxCounter);
    myAnimalCounters[3].setTexture(MainGameTextureManager::mouseCounter);
}

void MainGameWindow::initSprites() {
    line.setSize(sf::Vector2f(12, windowPtr->getSize().y - 200));
    line.setFillColor(sf::Color(237, 237, 237, 215));
    line.setPosition(windowPtr->getSize().x / 2, 0);

    myNickname.setFont(FontManager::fredokaOne);
    myNickname.setCharacterSize(35);
    myNickname.setPosition(20, (windowPtr->getSize().y - 60));

    enemyNickname.setFont(FontManager::fredokaOne);
    enemyNickname.setCharacterSize(35);
    enemyNickname.setPosition(((windowPtr->getSize().x - enemyNickname.getGlobalBounds().width) - 20), (windowPtr->getSize().y - 60));
    
    myCheckBtns[0].getSprite().setPosition(180, 752);
    myCheckBtns[1].getSprite().setPosition(350, 752);
    myCheckBtns[2].getSprite().setPosition(520, 752);
    myCheckBtns[3].getSprite().setPosition(690, 752);

    enemyCheckBtn[0].getSprite().setPosition(1135, 752);
    enemyCheckBtn[1].getSprite().setPosition(1305, 752);
    enemyCheckBtn[2].getSprite().setPosition(1475, 752);
    enemyCheckBtn[3].getSprite().setPosition(1645, 752);

    myAnimalCounters[0].getSprite().setPosition(15, 290);
    myAnimalCounters[1].getSprite().setPosition(15, 370);
    myAnimalCounters[2].getSprite().setPosition(15, 450);
    myAnimalCounters[3].getSprite().setPosition(15, 530);
}

void MainGameWindow::handleEnemyNickname() {
    try {
        this->enemyNickname.setString(NetUtils::read_(*this->client->getSocket()).getStr());
    }
    catch (const boost::system::system_error& e) {
        std::cout << "\nError in handle enemy nickname\n";
        this->client->close();
    }
}
