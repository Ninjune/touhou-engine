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
#include "Menu/StageLoader/StageLoader.h"

const int SCREENWIDTH = 1422, SCREENHEIGHT = 800;
void defineTextures(std::map<std::string, sf::Texture>& textureMap);
void definePatterns(std::map<std::string, BulletPattern>&);
void map(std::map<std::string, sf::Texture>& textureMap, std::string name,
    std::string texturePath);
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
    sf::RenderWindow window(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT),
        "real touhou game");
    window.setFramerateLimit(60);
    std::chrono::steady_clock::time_point lastTime =
        std::chrono::high_resolution_clock::now(), currentTime;

    const std::string fontPath = "C:\\Windows\\Fonts\\arial.ttf";
    sf::Font font;
    font.loadFromFile(fontPath);
    sf::Text infoText;
    std::string textString;
    infoText.setFont(font);
    sf::Text gameText;
    gameText.setFont(font);

    Player player(window, textureMap);
    Keybind eKey(sf::Keyboard::E), pKey(sf::Keyboard::P), escKey(sf::Keyboard::Escape);
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
            pather.poll(event, stageLength, patterns);
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
            pather.reload();
            state = "edit";
            playerBullets.clear();
        }

        if (escKey.consumeClick(frame, 15))
        {
            state = "loader";
            stageFrame = 0;
            player.reset();
            stageLoader.checkStages();
            pather.reload();
            enemies.clear();
        }

        window.clear();
        gameText.setString("");
        if (state == "loader")
        {
            stageLoader.update(window,
                frame,
                textureMap,
                font,
                state,
                enemies,
                stageLength,
                patterns
            );
        }
        else if (state == "play")
        {
            for (unsigned int i = 0; i < playerBullets.size(); i++)
            {
                if (!playerBullets[i].getRender())
                    playerBullets.erase(playerBullets.begin() + i);
                else if (!sf::FloatRect(sf::Vector2f(0, 0),
                    sf::Vector2f(window.getSize().x, window.getSize().y))
                    .contains(playerBullets[i].getPosition()))
                    playerBullets.erase(playerBullets.begin() + i);
                else
                    playerBullets[i].updateSprite(window, frame);
            }

            for (unsigned int i = 0; i < enemies.size(); i++)
                enemies[i].updateSprite(textureMap, window, frame, bullets,
                    stageFrame, player.getPosition(), false);

            player.updateSprite(window, frame, playerBullets, enemies, bullets,
                textureMap);
            stageFrame++;

            if (stageFrame > stageLength)
                state = "win";
            else if (player.getLife() <= 0)
                state = "lose";
        }
        else if (state == "edit")
        {
            pather.update(window, frame, textureMap, patterns, enemies, stageLength);
        }
        else if (state == "win")
        {
            gameText.setString("STAGE COMPLETE!\nHit Escape to return to loader.");
        }
        else if (state == "lose\nHit Escape to return to loader.")
        {
            gameText.setString("lose");
        }
        sf::FloatRect textRect = gameText.getLocalBounds();
        gameText.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        gameText.setPosition(window.getSize().x / 2, window.getSize().y / 2);
        window.draw(gameText);

        infoText.setString(textString);
        window.draw(infoText);
        window.display();
        frame++;
    }

    return 0;
}


// defines the textures in the texture map.
void defineTextures(std::map<std::string, sf::Texture>& textureMap)
{
    map(textureMap, "player", "textures/player/pl00/player.png");
    map(textureMap, "lifeStar", "textures/player/life.png");
    map(textureMap, "enemy", "textures/enemy/enemy.png");
    map(textureMap, "bullet1", "textures/bullet/bullet1.png");
    map(textureMap, "selectionIcon", "textures/icons/selection.png");
    map(textureMap, "pencilIcon", "textures/icons/pencil.png");
    map(textureMap, "duplicateIcon", "textures/icons/duplicate.png");
    map(textureMap, "plusIcon", "textures/icons/plus.png");
    map(textureMap, "minusIcon", "textures/icons/minus.png");
    map(textureMap, "upIcon", "textures/icons/upArrow.png");
    map(textureMap, "trashIcon", "textures/icons/trashIcon.png");
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
    std::map<std::string, float> options;
    std::string bulletType;
    std::fstream patternFile;

    for (std::filesystem::directory_entry file :
    std::filesystem::directory_iterator(folderName))
    {
        options.clear();
        patternName = file.path().string().substr(9, file.path().string().find_first_of(".") - 9);
        patternFile.open(folderName + "/" + patternName + ".txt", std::ios::in);

        if (patternFile.is_open())
        {
            patternFile >> options["originX"] >> options["originY"] >>
                options["frequency"] >> options["burstCount"] >> options["burstSize"] >>
                options["burstSizeChange"] >> options["direction"] >>
                options["directionChange"] >> options["spawnDirection"] >>
                options["spawnDirectionChange"] >> options["velocity"] >>
                options["velocityChange"] >> bulletType;
        }

        patterns[patternName] = BulletPattern(options, bulletType, patternName);
        patternFile.close();
    }

}