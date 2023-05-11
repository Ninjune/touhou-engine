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
	inputText.setFont(font);

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
    inputStr = "";

    tools.push_back(Tool(textureMap["selectionIcon"], window.getSize().x / 45,
        window.getSize().y / 4, 64, 512, true)); // select enemy
    tools.push_back(Tool(textureMap["pencilIcon"], window.getSize().x / 45 * 4,
        window.getSize().y / 4, 64, 512)); // draw path
    tools.push_back(Tool(textureMap["duplicateIcon"], window.getSize().x / 45,
        window.getSize().y / 4 + 96, 64, 512)); // will duplicate then put copy under mouse to be moved wherever

    bulletPatternMenu.setPosition(14, playableArea.getPosition().y-7);
    bulletPatternMenu.setSize(180, playableArea.getSize().y/2+7);
    bulletPatternMenu.setPatternFolder("patterns/", textureMap, font, patterns);

    selecting = false;
    canDuplicate = false;
    enterStageName = false;
    loaded = false;
}


void Pather::poll(sf::Event& event, int stageLength)
{
    if (stageLength == 0 && event.type == sf::Event::TextEntered)
    {
        if ((char)event.text.unicode >= 48 && (char)event.text.unicode < 58)
            inputStr += event.text.unicode;
        else if ((char)event.text.unicode == 8 && inputStr.size() > 0)
            inputStr.pop_back();
    }
    else if (enterStageName && event.type == sf::Event::TextEntered)
    {
        if ((char)event.text.unicode == 8 && inputStr.size() > 0)
            inputStr.pop_back();
        else if ((char)event.text.unicode >= 32)
            inputStr += (char)event.text.unicode;
    }
}


void Pather::update(sf::RenderWindow& window,
    int& frame,
    std::map<std::string, sf::Texture>& textureMap,
    std::map<std::string, BulletPattern>& patterns,
    std::vector<Enemy>& enemies,
    int stageLength
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

        if (inputStr != "")
            frames = std::to_string(stoi(inputStr) / 60.).substr(0,
                std::to_string(stoi(inputStr) / 60.).size() - 4);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            stageLength = stoi(inputStr) > 0 ? stoi(inputStr) + 1 : 0;

        prompt(window, "Stage length = " + frames +
            " seconds\nEnter frames: " + inputStr);
    }
    else if (enterStageName)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) // export
        {
            std::fstream stageFile;

            stageFile.open("stages/" + inputStr + ".stg", std::ios::out);

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

            enterStageName = false;
        }

        prompt(window, "Enter stage name (no commas ',' or semicolons ';'): " + inputStr);
    }
    else if (patternChanging)
    {
        patternChanger.update(window, frame, bulletPatternMenu, patterns);
        bulletPatternMenu.update(window,
            frame,
            patherEnemies,
            selectedEnemyIndex,
            textureMap,
            timeline.getCurrentFrame(),
            patternChanging
        );
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
            inputStr = "";
        }

        if (tools[1].getStatus() && (m2.consumeClick(frame, 20) || (
            selectedEnemyIndex == -1 &&
                playableArea.getGlobalBounds().contains(mousePos) &&
                m1.consumeClick(frame, 20, 2)
                )
            )
        )
        {
            patherEnemies.push_back(Enemy(0, textureMap["enemy"]));
            selectedEnemyIndex = patherEnemies.size() - 1;
            patherEnemies[selectedEnemyIndex].setPlayableArea(playableArea);
            patherEnemies[selectedEnemyIndex].setStartFrame(timeline.getCurrentFrame());
        }

        if (tools[0].getStatus() || tools[2].getStatus()) // select
            continuing = selectTool(window, frame, patherBullets);
        else if (tools[1].getStatus()) // draw
            continuing = drawTool(window, frame);

        if(!continuing)
            return draw(textureMap, window, frame, patterns);
        
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
            textureMap,
            timeline.getCurrentFrame(),
            patternChanging
        );
        
        // HERE enemy is unable to move when in editor
        draw(textureMap, window, frame, patterns);
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

    for (Enemy& enemy : patherEnemies)
        enemy.updateSprite(textureMap, window, frame,
            patherBullets, timeline.getCurrentFrame(), true);
    for (Tool& tool : tools)
        tool.update(window, frame, tools);
}


void Pather::prompt(sf::RenderWindow& window, std::string input)
{
    sf::FloatRect textRect = inputText.getLocalBounds();
    inputText.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);

    inputText.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    inputText.setString(input);
    window.draw(inputText);
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

    sf::RectangleShape hitboxOutline;
    Enemy& selectedEnemy = patherEnemies[selectedEnemyIndex];
    sf::FloatRect hitbox = selectedEnemy.getHitbox();

    hitboxOutline.setPosition(hitbox.left, hitbox.top);
    hitboxOutline.setSize(sf::Vector2f(hitbox.width, hitbox.height));
    hitboxOutline.setFillColor(sf::Color::Transparent);
    hitboxOutline.setOutlineColor(sf::Color::White);
    hitboxOutline.setOutlineThickness(1);

    if (selecting)
        selectedEnemy.setPosition(mousePos, bullets);

    if (backspace.consumeClick(frame, 20) || delkey.consumeClick(frame, 20))
    {
        patherEnemies.erase(patherEnemies.begin() + selectedEnemyIndex);
        selectedEnemyIndex = -1;
        return false;
    }

    window.draw(hitboxOutline);
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