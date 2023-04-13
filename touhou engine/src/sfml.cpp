/*
Header goes here (dont forgor to add)
make sure to make instructions/readme explaining the premise of the game, controls, and a warning not to mess with the window size.
*/
#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include <map>
#include "Entity/Player.h"
#include "Entity/Bullet.h"
#include "Entity/Enemy.h"
#include "Util/Keybind.h"
#include "Editor/Pather.h"

const int SCREENWIDTH = 1422, SCREENHEIGHT = 800;
void defineTextures(std::map<std::string, sf::Texture>& map);
/*
Todo:
- Enemy bullets
- Dying
- Stages
- Title screen
- Score
- Bomb?
- other characters IF have time
*/

int main()
{
    std::map<std::string, sf::Texture> textureMap;
    defineTextures(textureMap);
    int frame = 0, fps = 0;
    bool titleScreen = true, patherEnabled = false;
    sf::RenderWindow window(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), "real touhou game");
    window.setFramerateLimit(60);
    std::chrono::steady_clock::time_point lastTime = std::chrono::high_resolution_clock::now(), currentTime;

    const std::string fontPath = "C:\\Windows\\Fonts\\arial.ttf";
    sf::Font font;
    font.loadFromFile(fontPath);
    sf::Text text;
    std::string textString;
    text.setFont(font);

    Player player(window, textureMap["player"]);
    Keybind eKey(sf::Keyboard::E), pKey(sf::Keyboard::P), RBracketKey(sf::Keyboard::RBracket);
    Pather pather(window, font, SCREENWIDTH, SCREENHEIGHT);
    std::vector<Enemy> enemies, enemiesTemp;
    std::vector<Bullet> bullets, bulletsTemp;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            pather.poll(event);
        }
        
        if (window.hasFocus())
        {
            if (frame % 5 == 0) // 1/12 of a second
            {
                currentTime = std::chrono::high_resolution_clock::now();
                fps = 1000000000.0 / (std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count());
            }
            textString = ("Fps: " + std::to_string(fps) + 
                (patherEnabled ? "\nPath length: " + std::to_string(pather.getSelectedEnemyPath().size()) +
                "\nPath speed: " + std::to_string(pather.getSelectedEnemyPath().getPathSpeed()) +
                "s\nReset path: 'R'\nExport path: 'E'\nClear enemies: ]" : "")
            );
            lastTime = std::chrono::high_resolution_clock::now();

            if (pKey.consumeClick(frame, 15))
            {
                patherEnabled = !patherEnabled;
                bullets.clear();
            }

            if (RBracketKey.consumeClick(frame, 15))
                enemies.clear();

            window.clear();
            for (unsigned int i = 0; i < bullets.size(); i++)
            {
                if (!bullets[i].getRender())
                    bullets.erase(bullets.begin() + i);
                else
                    bullets[i].updateSprite(window, frame);
                
            }

            for (unsigned int i = 0; i < enemies.size(); i++)
            {
                if (!enemies[i].getRender())
                    enemies.erase(enemies.begin() + i);
                else
                    enemies[i].updateSprite(window, frame, bullets);
            }

            if (patherEnabled) // make pather draw everything in window
                pather.update(window, frame, textureMap["enemy"]);

            for (unsigned int i = 0; i < enemies.size(); i++)
            {
                if (!enemies[i].isPathFine())
                    textString += "\n#" + std::to_string(i) + 
                    " enemy path is not smoothed.";
            }

            text.setString(textString);
            if(!patherEnabled) player.updateSprite(window, frame, bullets);
            window.draw(text);
            window.display();
            frame++;
        }
    }

    return 0;
}


// defines the textures in the texture map.
void defineTextures(std::map<std::string, sf::Texture>& textureMap)
{
    textureMap["player"] = sf::Texture();
    textureMap["player"].loadFromFile("textures/player/pl00/player.png");

    textureMap["enemy"] = sf::Texture();
    textureMap["enemy"].loadFromFile("textures/enemy/enemy.png");
}