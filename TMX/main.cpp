
#include"pch.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include<SFML/Graphics.hpp>

#include <tmxlite/Map.hpp>


#include "SFMLOrthogonalLayer.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML window", sf::Style::Fullscreen);
    window.setFramerateLimit(200);
    tmx::Map map;
    map.load("assets/Level_Map.tmx");

    const auto& layers = map.getLayers();
    const auto& collider = layers[1]->getLayerAs<tmx::ObjectGroup>();

    MapLayer backGround(map, 0);
    // MapLayer Objects(map, 1);


    // Remove
    float delay = 0;
    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(40);
    text.setPosition(10.0f, 10.0f);
    text.setFillColor(sf::Color::White);

    sf::RectangleShape player({ 64.0f, 160.0f });
    player.setOrigin(32.0f, 64.0f);
    player.setPosition(10.0f, 500.0f);

    sf::View gameView(player.getPosition(), sf::Vector2f(1920.0f, 1080.0f));

    std::vector<sf::FloatRect> tileMapColliders;

    for (auto& obj : collider.getObjects())
    {
        tileMapColliders.push_back(sf::FloatRect(obj.getAABB().left, obj.getAABB().top, obj.getAABB().width, obj.getAABB().height));
    }

    float playerSpeed = 5.0f;

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

        if ((gameView.getCenter().x - gameView.getSize().x / 2) < 0) gameView.move(sf::Vector2f(gameView.getSize().x / 2 - player.getPosition().x, 0));
        if ((gameView.getCenter().y - gameView.getSize().y / 2) < 0) gameView.move(sf::Vector2f(0, gameView.getSize().y / 2 - player.getPosition().y));
        if ((gameView.getCenter().x + gameView.getSize().x / 2) > backGround.getGlobalBounds().width) gameView.move(sf::Vector2f(backGround.getGlobalBounds().width - (gameView.getCenter().x + gameView.getSize().x / 2), 0));
        if ((gameView.getCenter().y + gameView.getSize().y / 2) > backGround.getGlobalBounds().height) gameView.move(sf::Vector2f(0, backGround.getGlobalBounds().height - (gameView.getCenter().y + gameView.getSize().y / 2)));


        window.setView(gameView);

        // Remove
        delay += delTime;
        if (delay > 0.1)
        {
            text.setString(std::to_string(float(1.0f / delTime)));
            delay = 0;
        }
        text.setPosition(gameView.getCenter().x - gameView.getSize().x / 2, gameView.getCenter().y - gameView.getSize().y / 2);

        window.clear(sf::Color::Black);
        window.draw(backGround);
        window.draw(player);
        window.draw(text);
        window.display();
    }

    return 0;
}