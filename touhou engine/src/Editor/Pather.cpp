#include "Pather.h"


Pather::Pather(sf::RenderWindow& window,
    sf::Font& font,
    std::map<std::string, sf::Texture>& textureMap,
    int screenWIn,
    int screenHIn,
    std::map<std::string, BulletPattern>& patterns
) :
    upKey(sf::Keyboard::Up), downKey(sf::Keyboard::Down), m1(sf::Mouse::Left),
    pathRender(sf::LinesStrip), timeline(window.getSize().x, window.getSize().y, font),
    playableArea(sf::Vector2f(window.getSize().x * 0.7, window.getSize().y * 0.7)),
    rKey(sf::Keyboard::R), m2(sf::Mouse::Right), backspace(sf::Keyboard::Backspace),
    delkey(sf::Keyboard::Delete), eKey(sf::Keyboard::E)
{
    SCREENHEIGHT = screenHIn;
    SCREENWIDTH = screenWIn;
	prompter.setFont(font);

    playableArea.setOrigin(playableArea.getSize().x / 2,
        playableArea.getSize().y / 2);
    playableArea.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    playableArea.setFillColor(sf::Color::Transparent);
    playableArea.setOutlineColor(sf::Color::White);
    playableArea.setOutlineThickness(1);

    numKeys.push_back(Keybind(sf::Keyboard::Num1));
    numKeys.push_back(Keybind(sf::Keyboard::Num2));
    numKeys.push_back(Keybind(sf::Keyboard::Num3));
    numKeys.push_back(Keybind(sf::Keyboard::Num4));
    
    selectedEnemyIndex = -1;

    tools.push_back(Tool(textureMap["selectionIcon"], window.getSize().x / 45,
        window.getSize().y / 4, 64, 512, true)); // select enemy
    tools.push_back(Tool(textureMap["pencilIcon"], window.getSize().x / 45 * 4,
        window.getSize().y / 4, 64, 512)); // draw path
    tools.push_back(Tool(textureMap["duplicateIcon"], window.getSize().x / 45,
        window.getSize().y / 4 + 96, 64, 512)); // will duplicate then put copy under mouse to be moved wherever

    bulletPatternMenu.setPosition(14, playableArea.getPosition().y-7);
    bulletPatternMenu.setSize(180, playableArea.getSize().y/2+7);
    bulletPatternMenu.setPatternFolder("patterns/", textureMap, font, patterns);
    patternChanger.setFont(font);

    selecting = false;
    canDuplicate = false;
    enterStageName = false;
    loaded = false;
}


void Pather::poll(sf::Event& event,
    int stageLength,
    std::map<std::string, BulletPattern>& patterns
)
{
    if (stageLength == 0 && event.type == sf::Event::TextEntered)
        prompter.poll(event, 48, 57, inputStr1);
    else if (enterStageName && event.type == sf::Event::TextEntered)
        prompter.poll(event, 32, 256, inputStr2, { ',', ';' });
    else if (patternChanging != "" && event.type == sf::Event::TextEntered)
        patternChanger.poll(event, patterns[patternChanging]);
    else if (addingPattern && event.type == sf::Event::TextEntered)
        prompter.poll(event, 32, 256, inputStr3, { ',', ';' });
}


void Pather::update(sf::RenderWindow& window,
    int& frame,
    std::map<std::string, sf::Texture>& textures,
    std::map<std::string, BulletPattern>& patterns,
    std::vector<Enemy>& enemies,
    int& stageLength
)
{
    if (!loaded)
    {
        patherEnemies = enemies;
        for (Enemy& enemy : patherEnemies)
            enemy.setPlayableArea(playableArea);
        loaded = true;
    }

    pathRender.clear();
    if (stageLength == 0)
    {
        std::string frames = "0.00";

        if (inputStr1 != "")
            frames = std::to_string(stoi(inputStr1) / 60.).substr(0,
                std::to_string(stoi(inputStr1) / 60.).size() - 4);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            stageLength = stoi(inputStr1) > 0 ? stoi(inputStr1) + 1 : 0;

        prompter.prompt(window, "Stage length = " + frames +
            " seconds\nEnter frames: " + inputStr1, frame);
    }
    else if (enterStageName)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) // export
        {
            std::fstream stageFile;

            stageFile.open("stages/" + inputStr2 + ".stg", std::ios::out);

            stageFile << stageLength << "\n";

            for (Enemy& enemy : patherEnemies)
                stageFile << enemy.getStartFrame() << ",";
            
            stageFile << "\n";

            for (Enemy& enemy : patherEnemies)
            {
                Path path = enemy.getPath();

                for (sf::Vector2f& point : path)
                    stageFile << point.x << "," << point.y << " ";

                stageFile << ";";
            }

            stageFile << "\n";

            for (Enemy& enemy : patherEnemies)
            {
                for (std::string& pattern : enemy.getPatterns())
                    stageFile << pattern << ",";
                stageFile << ";";
            }

            stageFile << "\n";

            for (Enemy& enemy : patherEnemies)
            {
                for (int& start : enemy.getStartTimes())
                    stageFile << start << ",";
                stageFile << ";";
            }

            stageFile.close();

            enterStageName = false;
        }

        prompter.prompt(window, "Enter stage name: " + inputStr2, frame);
    }
    else if (patternChanging != "")
    {
        if (selectedEnemyIndex < 0)
            selectedEnemyIndex = 0;

        patternChanger.update(window,
            frame,
            bulletPatternMenu,
            patterns,
            textures,
            patternChanging,
            bulletPatternMenu
        );
        bulletPatternMenu.update(window,
            frame,
            patherEnemies,
            selectedEnemyIndex,
            textures,
            timeline.getCurrentFrame(),
            patternChanging,
            patterns,
            addingPattern
        );
    }
    else if (addingPattern)
    {
        std::map<std::string, float> options;
        options["originX"] = -1;
        options["originY"] = -1;
        options["frequency"] = 0;
        options["burstCount"] = 0;
        options["burstSize"] = 0;
        options["burstSizeChange"] = 0;
        options["direction"] = 0;
        options["directionChange"] = 0;
        options["spawnDirection"] = 0;
        options["spawnDirectionChange"] = 0;
        options["velocity"] = 0;
        options["velocityChange"] = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            patterns[inputStr3] = BulletPattern(options, "orb", inputStr3);
            addingPattern = false;
            patternChanging = inputStr3;
            bulletPatternMenu.addButton(patterns[inputStr3]);
            inputStr3 = "";
        }

        prompter.prompt(window, "Enter pattern name: " + inputStr3, frame);
    }
    else
    {
        mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x,
            sf::Mouse::getPosition(window).y
        );
        bool continuing = false;

        timeline.setStageLength(stageLength);

        if (eKey.consumeClick(frame, 20))
        {
            enterStageName = true;
            inputStr2 = "";
        }

        if (tools[1].getStatus() && (m2.consumeClick(frame, 20) || (
            selectedEnemyIndex == -1 &&
                playableArea.getGlobalBounds().contains(mousePos) &&
                m1.consumeClick(frame, 20, 2)
                )
            )
        )
        {
            patherEnemies.push_back(Enemy(0, textures["enemy"]));
            selectedEnemyIndex = patherEnemies.size() - 1;
            patherEnemies[selectedEnemyIndex].setPlayableArea(playableArea);
            patherEnemies[selectedEnemyIndex].setStartFrame(timeline.getCurrentFrame());
        }

        if (tools[0].getStatus() || tools[2].getStatus()) // select
            continuing = selectTool(window, frame, patherBullets);
        else if (tools[1].getStatus()) // draw
            continuing = drawTool(window, frame);

        if(!continuing)
            return draw(textures, window, frame, patterns);
        
        Path selectedEnemyPath = patherEnemies[selectedEnemyIndex].getPath();

        for (sf::Vector2f& point : selectedEnemyPath)
        {
            pathRender.append(playableToPather(
                sf::Vector2f(point.x, point.y), playableArea, window)
            );
        }

        if (rKey.consumeClick(frame, 20))
            patherEnemies[selectedEnemyIndex].clearPath();
        if (upKey.consumeClick(frame, 5))
            patherEnemies[selectedEnemyIndex]
            .setPathSpeed(selectedEnemyPath.getPathSpeed() + 1);
        if (downKey.consumeClick(frame, 5))
            patherEnemies[selectedEnemyIndex]
            .setPathSpeed(selectedEnemyPath.getPathSpeed() - 1);
        

        // we only want to update this when we have selected an enemy
        bulletPatternMenu.update(window,
            frame,
            patherEnemies,
            selectedEnemyIndex,
            textures,
            timeline.getCurrentFrame(),
            patternChanging,
            patterns,
            addingPattern
        );
        
        // HERE enemy is unable to move when in editor
        draw(textures, window, frame, patterns);
    }
}


sf::Vector2f Pather::patherToPlayable(sf::Vector2f point,
    sf::RectangleShape& playableArea, sf::RenderWindow& window)
{
    return sf::Vector2f(
        (point.x - playableArea.getPosition().x) *
        window.getSize().x / playableArea.getSize().x +
        playableArea.getPosition().x,
        (point.y - playableArea.getPosition().y) *
        window.getSize().y / playableArea.getSize().y +
        playableArea.getPosition().y);
}


sf::Vector2f Pather::playableToPather(sf::Vector2f point,
    sf::RectangleShape& playableArea, sf::RenderWindow& window)
{
    return sf::Vector2f(
        (point.x - playableArea.getPosition().x) /
        window.getSize().x * playableArea.getSize().x +
        playableArea.getPosition().x,
        (point.y - playableArea.getPosition().y) /
        window.getSize().y * playableArea.getSize().y +
        playableArea.getPosition().y
    );
}


Path Pather::getSelectedEnemyPath()
{
    if (selectedEnemyIndex >= 0 && selectedEnemyIndex < patherEnemies.size())
        return patherEnemies[selectedEnemyIndex].getPath();
    else
        return {};
}


void Pather::draw(std::map<std::string, sf::Texture>& textureMap,
    sf::RenderWindow& window,
    int& frame,
    std::map<std::string, BulletPattern>& patterns
)
{
    window.draw(pathRender);
    window.draw(playableArea);
    timeline.update(window, frame);
    sf::Vector2f playerPos;
    playerPos.x = window.getSize().x / 6 * 5;
    playerPos.y = window.getSize().y / 2;
    for (Enemy& enemy : patherEnemies)
        enemy.updateSprite(textureMap, window, frame,
            patherBullets, timeline.getCurrentFrame(), playerPos, true
        );
    for (Tool& tool : tools)
        tool.update(window, frame, tools);
}


bool Pather::selectTool(sf::RenderWindow& window,
    int& frame,
    std::vector<std::vector<std::vector<Bullet>>>& bullets
)
{
    for (unsigned int i = 0; i < patherEnemies.size(); i++)
    {
        if (!selecting &&
            patherEnemies[i].getHitbox().contains(mousePos) &&
            m1.consumeClick(frame, 20, 1) &&
            patherEnemies[i].getRender()
            )
        {
            selectedEnemyIndex = i;
            selecting = true;
            if (tools[2].getStatus())
                canDuplicate = true; // make sure this works
        }
    }

    if (selectedEnemyIndex < 0)
        return false;

    if (tools[2].getStatus() && canDuplicate)
    {
        patherEnemies.push_back(Enemy(patherEnemies[selectedEnemyIndex]));
        selectedEnemyIndex = patherEnemies.size() - 1;
        patherEnemies[selectedEnemyIndex].setPlayableArea(playableArea);
        canDuplicate = false;
    }

    if (selecting && !m1.isHeld())
        selecting = false;

    Enemy& selectedEnemy = patherEnemies[selectedEnemyIndex];

    if (selecting)
        selectedEnemy.setPosition(mousePos, bullets);

    if (backspace.consumeClick(frame, 20) || delkey.consumeClick(frame, 20))
    {
        patherEnemies.erase(patherEnemies.begin() + selectedEnemyIndex);
        selectedEnemyIndex = -1;
        return false;
    }

    selectedEnemy.renderHitbox(window);
    return true;
}


bool Pather::drawTool(sf::RenderWindow& window, int& frame)
{
    if (selectedEnemyIndex < 0) return false;
    Path selectedEnemyPath = patherEnemies[selectedEnemyIndex].getPath();
   
    if (m1.consumeClick(frame, 0) &&
        selectedEnemyPath.size() < selectedEnemyPath.getPathSpeed() * 60 &&
        sf::Mouse::getPosition(window).y < 0.85 * SCREENHEIGHT &&
        sf::Mouse::getPosition(window).x > playableArea.getPosition().x -
        playableArea.getSize().x / 2 &&
        sf::Mouse::getPosition(window).x < playableArea.getPosition().x +
        playableArea.getSize().x / 2)
        patherEnemies[selectedEnemyIndex].pushToPath(patherToPlayable(
            sf::Vector2f(mousePos.x, mousePos.y), playableArea, window));

    return true;
}


void Pather::reload()
{
    loaded = false;
    selectedEnemyIndex = -1;
    selecting = false;
    canDuplicate = false;
    enterStageName = false;
    loaded = false;
}