#include "MainGameWindow.h"

void MainGameWindow::init(std::shared_ptr<sf::RenderWindow> windowPtr_, const std::string nickname, std::shared_ptr<Client> client) {
    /* testing area */
    // tmp this->windowPtr = windowPtr;
    this->client = client;
    
    this->windowPtr = std::make_shared<sf::RenderWindow>();
    this->windowPtr->create(sf::VideoMode(900, 600), "SkyFall Showdown", sf::Style::Close);
    this->windowPtr->setFramerateLimit(60);
    
    //handleEnemyNickname();
    this->myNickname.setString(nickname);
    this->enemyNickname.setString("Dragofinale012");
    /* end testing area */

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

    this->windowPtr->draw(myNickname);
    this->windowPtr->draw(enemyNickname);
   
    this->windowPtr->display();
}

void MainGameWindow::setTextures() {

}

void MainGameWindow::initSprites() {
    myNickname.setFont(FontManager::fredokaOne);
    myNickname.setCharacterSize(35);
    myNickname.setPosition(20, (windowPtr->getSize().y - 60));

    enemyNickname.setFont(FontManager::fredokaOne);
    enemyNickname.setCharacterSize(35);
    enemyNickname.setPosition(((windowPtr->getSize().x - enemyNickname.getGlobalBounds().width) - 20), (windowPtr->getSize().y - 60));
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
