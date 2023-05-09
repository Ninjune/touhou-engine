/*
Header goes here (dont forgor to add)
make sure to make instructions/readme explaining the premise of the game, controls, and a warning not to mess with the window size.
*/
#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include <map>
#include <filesystem>
#include <fstream>
#include "Entity/Player.h"
#include "Entity/Bullet.h"
#include "Entity/Enemy.h"
#include "Util/Keybind.h"
#include "Editor/Pather.h"
#include "Menu/StageLoader.h"

const int SCREENWIDTH = 1422, SCREENHEIGHT = 800;
void defineTextures(std::map<std::string, sf::Texture>& textureMap);
void definePatterns(std::map<std::string, BulletPattern>&);
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
    std::map<std::string, BulletPattern> patterns;
    std::string state = "loader";
    defineTextures(textureMap);
    definePatterns(patterns);
    int frame = 0, fps = 0, stageFrame = 0, stageLength = 0; // update stage frame when stage is loaded
    bool titleScreen = true;
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
    Pather pather(window, font, textureMap, SCREENWIDTH, SCREENHEIGHT, patterns);
    std::vector<Enemy> enemies;
    std::vector<Bullet> playerBullets;
    std::vector<std::vector<std::vector<Bullet>>> bullets;
    StageLoader stageLoader;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            pather.poll(event);
        }
        
        if (!window.hasFocus())
            continue;

        if (frame % 5 == 0) // 1/12 of a second
        {
            currentTime = std::chrono::high_resolution_clock::now();
            fps = 1000000000.0 / (std::chrono::duration_cast
                <std::chrono::nanoseconds>(currentTime - lastTime).count());
        }
        textString = ("Fps: " + std::to_string(fps) +
            (state == "edit" ? "\nPath length: " +
                std::to_string(pather.getSelectedEnemyPath().size()) +
                "\nPath speed: " +
                std::to_string(pather.getSelectedEnemyPath().getPathSpeed()) +
                "\nExport path : 'E'" : "")
            );
        lastTime = std::chrono::high_resolution_clock::now();

        if (pKey.consumeClick(frame, 15))
        {
            state = "edit";
            playerBullets.clear();
        }

        window.clear();
        if (state == "loader")
        {
            stageLoader.update(window,
                frame,
                textureMap,
                font,
                state,
                enemies,
                stageLength
            );
        }
        else if (state == "play")
        {
            for (unsigned int i = 0; i < playerBullets.size(); i++)
            {
                if (!playerBullets[i].getRender())
                    playerBullets.erase(playerBullets.begin() + i);
                else
                    playerBullets[i].updateSprite(window, frame);
            }

            for (unsigned int i = 0; i < enemies.size(); i++)
            {
                if (!enemies[i].getRender())
                    enemies.erase(enemies.begin() + i);
                else
                    enemies[i].updateSprite(textureMap, window, frame, bullets);
            }

            player.updateSprite(window, frame, playerBullets, textureMap);
            stageFrame++;
        }
        else if (state == "edit")
        {
            pather.update(window, frame, textureMap, patterns);
        }

        text.setString(textString);
        window.draw(text);
        window.display();
        frame++;
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


void definePatterns(std::map<std::string, BulletPattern>& patterns)
{
    const std::string folderName = "patterns/";
    std::string patternName;
    unsigned int count = 0;

    // all of these will be passed through to BulletPattern
    sf::Vector2f origin;
    int frequency, burstCount,
        burstSize, burstSizeChange,
        direction, directionChange,
        spawnDirection, spawnDirectionChange;
    double velocity, velocityChange;
    std::string bulletType;

    std::fstream patternFile;

    for (std::filesystem::directory_entry file :
    std::filesystem::directory_iterator(folderName))
    {
        patternName = file.path().string().substr(9, file.path().string().find_first_of(".") - 9);
        patternFile.open(folderName + "/" + patternName + ".txt");

        if (patternFile.is_open())
        {
            patternFile >> origin.x >> origin.y >> frequency >> burstCount >>
                burstSize >> burstSizeChange >> direction >> directionChange >>
                spawnDirection >> spawnDirectionChange >> velocity >> velocityChange >>
                bulletType;
        }
        else
            throw "file not found?";

        patterns[patternName] = BulletPattern(origin, frequency, burstCount,
            burstSize, burstSizeChange, direction, directionChange, spawnDirection,
            spawnDirectionChange, velocity, velocityChange, bulletType, patternName
        );
    }
}