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
void defineTextures(std::map<std::string, sf::Texture>& textureMap);
void map(std::map<std::string, sf::Texture>& textureMap, std::string name, std::string texturePath);
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
    int frame = 0, fps = 0, stageFrame = 0; // update stage frame when stage is loaded
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

    Player player(window, textureMap);
    Keybind eKey(sf::Keyboard::E), pKey(sf::Keyboard::P), RBracketKey(sf::Keyboard::RBracket);
    Pather pather(window, font, textureMap, SCREENWIDTH, SCREENHEIGHT);
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
                fps = 1000000000.0 / (std::chrono::duration_cast
                        <std::chrono::nanoseconds>(currentTime - lastTime).count());
            }
            textString = ("Fps: " + std::to_string(fps) + 
                (patherEnabled ? "\nPath length: " + 
                std::to_string(pather.getSelectedEnemyPath().size()) +
                "\nPath speed: " + 
                std::to_string(pather.getSelectedEnemyPath().getPathSpeed()) +
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
                pather.update(window, frame, textureMap);

            for (unsigned int i = 0; i < enemies.size(); i++)
            {
                if (!enemies[i].isPathFine())
                    textString += "\n#" + std::to_string(i) + 
                    " enemy path is not smoothed.";
            }

            text.setString(textString);
            if(!patherEnabled) player.updateSprite(window, frame, bullets, textureMap);
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
    map(textureMap, "player", "textures/player/pl00/player.png");
    map(textureMap, "enemy", "textures/enemy/enemy.png");
    map(textureMap, "bullet1", "textures/bullet/bullet1.png");
    map(textureMap, "selectionIcon", "textures/icons/selection.png");
    map(textureMap, "pencilIcon", "textures/icons/pencil.png");
    map(textureMap, "duplicateIcon", "textures/icons/duplicate.png");
    map(textureMap, "plusIcon", "textures/icons/plus.png");
    map(textureMap, "minusIcon", "textures/icons/minus.png");
    map(textureMap, "upIcon", "textures/icons/upArrow.png");
}


void map(std::map<std::string, sf::Texture>& textureMap, std::string name, std::string texturePath)
{
    textureMap[name] = sf::Texture();
    textureMap[name].loadFromFile(texturePath);
}