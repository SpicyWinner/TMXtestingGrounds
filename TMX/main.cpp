
#include"pch.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include<SFML/Graphics.hpp>

#include <tmxlite/Map.hpp>


#include "SFMLOrthogonalLayer.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    window.setFramerateLimit(200);
    tmx::Map map;
    map.load("assets/kripper.tmx");
    std::cout << std::endl;

    const auto& layers = map.getLayers();
    const auto& collider = layers[2]->getLayerAs<tmx::ObjectGroup>();

    MapLayer backGround(map, 0);
    MapLayer Objects(map, 1);
    
    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(40);
    text.setPosition(10.0f, 10.0f);
    text.setFillColor(sf::Color::White);

    sf::CircleShape player(10.0f);
    player.setOrigin(10.0f, 10.0f);
    player.setPosition(10.0f, 500.0f);

    sf::View gameView(player.getPosition(), sf::Vector2f(800.0f, 600.0f));
    
    std::vector<sf::FloatRect> tileMapColliders;

    for (auto& obj : collider.getObjects())
    {
        tileMapColliders.push_back(sf::FloatRect(obj.getAABB().left, obj.getAABB().top, obj.getAABB().width, obj.getAABB().height));
    }

    float playerSpeed = 1.0f;

    sf::Clock globalClock, animationClock;
    while (window.isOpen())
    {
        float delTime = globalClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player.move(0.0f, -playerSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) player.move(0.0f, playerSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) player.move(-playerSpeed, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) player.move(playerSpeed, 0.0f);

        for (auto& currentObj : tileMapColliders)
        {
            if (currentObj.intersects(player.getGlobalBounds()))
            {
                if (player.getPosition().y < currentObj.top) player.move(0.0f, -playerSpeed);
                if (player.getPosition().y > currentObj.top + currentObj.height) player.move(0.0f, playerSpeed);
                if (player.getPosition().x < currentObj.left) player.move(-playerSpeed, 0.0f);
                if (player.getPosition().x > currentObj.left + currentObj.width) player.move(playerSpeed, 0.0f);
            }
        }

        backGround.update(sf::Time(animationClock.getElapsedTime()));

        gameView.setCenter(player.getPosition());
        window.setView(gameView);

        text.setString(std::to_string(float(1.0f / delTime)));
        text.setPosition(gameView.getCenter() - sf::Vector2f(700.0f, 600.0f));

        std::string str = text.getString();
        std::cout << str << std::endl;

        window.clear(sf::Color::Black);
        window.draw(backGround);
        window.draw(Objects);
        window.draw(player);
        window.draw(text);
        window.display();
    }

    return 0;
}