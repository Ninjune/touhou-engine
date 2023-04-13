#include "Pather.h"

Pather::Pather(sf::RenderWindow& window, sf::Font& font, int screenWIn, int screenHIn)
    : upKey(sf::Keyboard::Up), downKey(sf::Keyboard::Down), m1(sf::Mouse::Left),
    pathRender(sf::LinesStrip), timeline(window.getSize().x, window.getSize().y, font),
    playableArea(sf::Vector2f(window.getSize().x * 0.7, window.getSize().y * 0.7)),
    rKey(sf::Keyboard::R), m2(sf::Mouse::Right), backspace(sf::Keyboard::Backspace), delkey(sf::Keyboard::Delete)
{
    SCREENHEIGHT = screenHIn;
    SCREENWIDTH = screenWIn;
	inputText.setFont(font);

    playableArea.setOrigin(playableArea.getSize().x / 2, playableArea.getSize().y / 2);
    playableArea.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    playableArea.setFillColor(sf::Color::Transparent);
    playableArea.setOutlineColor(sf::Color::White);
    playableArea.setOutlineThickness(1);
    stageLength = 0;

    numKeys.push_back(Keybind(sf::Keyboard::Num1));
    numKeys.push_back(Keybind(sf::Keyboard::Num2));
    numKeys.push_back(Keybind(sf::Keyboard::Num3));
    numKeys.push_back(Keybind(sf::Keyboard::Num4));
    
    selectedEnemyIndex = -1;
    inputStr = "";

    selectionTexture.loadFromFile("textures/tools/selection.png");
    pencilTexture.loadFromFile("textures/tools/pencil.png");
    duplicateTexture.loadFromFile("textures/tools/duplicate.png");

    tools.push_back(Tool(selectionTexture, window.getSize().x / 25, window.getSize().y / 4, 64, 512, true)); // select enemy
    tools.push_back(Tool(pencilTexture, window.getSize().x / 28*3, window.getSize().y / 4, 64, 512)); // draw path
    tools.push_back(Tool(duplicateTexture, window.getSize().x / 25, window.getSize().y / 4 + 96, 64, 512)); // will duplicate then put copy under mouse to be moved wherever

    selecting = false;
    canDuplicate = false;
}


void Pather::poll(sf::Event& event)
{
    if (stageLength == 0 && event.type == sf::Event::TextEntered)
    {
        if ((char)event.text.unicode >= 48 && (char)event.text.unicode < 58)
            inputStr += event.text.unicode;
        else if ((char)event.text.unicode == 8)
            inputStr.pop_back();
    }
}


void Pather::update(sf::RenderWindow& window, int& frame, sf::Texture& texture)
{
    // make window on left for pattern selection
    // make stage loader to import stage or create new > enter stage len
    // make selection tool, path tool
    // make enemies spawn with start frame.
    // make enemies work on stageFrame time.
    pathRender.clear();
    if (stageLength == 0)
    {
        std::string frames = "0.00";
        sf::FloatRect textRect = inputText.getLocalBounds();
        inputText.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            stageLength = stoi(inputStr) > 0 ? stoi(inputStr)+1 : 0;
        if (inputStr != "")
            frames = std::to_string(stoi(inputStr) / 60.).substr(0, std::to_string(stoi(inputStr) / 60.).size()-4);
        inputText.setPosition(window.getSize().x / 2, window.getSize().y / 2);
        inputText.setString("Stage length = " + frames + " seconds\nEnter frames: " + inputStr);
        window.draw(inputText);
    }
    else
    {
        mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

        timeline.setStageLength(stageLength);

        if (tools[1].getStatus() && m2.consumeClick(frame, 20))
        {
            patherEnemies.push_back(Enemy(0, texture));
            selectedEnemyIndex = patherEnemies.size() - 1;
            patherEnemies[selectedEnemyIndex].setPlayableArea(playableArea);
        }

        if (tools[0].getStatus() || tools[2].getStatus()) // select
        {
            for (int i = 0; i < patherEnemies.size(); i++)
            {
                if (!selecting && patherEnemies[i].getHitbox().contains(mousePos) && m1.consumeClick(frame, 20, 1))
                {
                    selectedEnemyIndex = i;
                    selecting = true;
                    canDuplicate = true;
                }
            }

            if (selectedEnemyIndex < 0)
                return draw(window, frame);

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
                selectedEnemy.setPosition(mousePos);

            if (backspace.consumeClick(frame, 20) || delkey.consumeClick(frame, 20))
            {
                patherEnemies.erase(patherEnemies.begin() + selectedEnemyIndex);
                selectedEnemyIndex = -1;
                return draw(window, frame);
            }

            window.draw(hitboxOutline);
        }
        else if (tools[1].getStatus())
        {
            if (selectedEnemyIndex < 0)
                return draw(window, frame);
            Path selectedEnemyPath = patherEnemies[selectedEnemyIndex].getPath();

            if (m1.consumeClick(frame, 0) &&
                selectedEnemyPath.size() < selectedEnemyPath.getPathSpeed() * 60 &&
                sf::Mouse::getPosition(window).y < 0.85 * SCREENHEIGHT &&
                sf::Mouse::getPosition(window).x > playableArea.getPosition().x - playableArea.getSize().x / 2 &&
                sf::Mouse::getPosition(window).x < playableArea.getPosition().x + playableArea.getSize().x / 2)
                patherEnemies[selectedEnemyIndex].pushToPath(patherToPlayable(
                    sf::Vector2f(mousePos.x, mousePos.y), playableArea, window));
        }
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
            patherEnemies[selectedEnemyIndex].setPathSpeed(selectedEnemyPath.getPathSpeed() + 1);
        if (downKey.consumeClick(frame, 5))
            patherEnemies[selectedEnemyIndex].setPathSpeed(selectedEnemyPath.getPathSpeed() - 1);

        draw(window, frame);
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
        return patherEnemies[selectedEnemyIndex].getPath(); // returning something innacurate
    else
        return {};
}


void Pather::draw(sf::RenderWindow& window, int& frame)
{
    window.draw(pathRender);
    window.draw(playableArea);
    timeline.update(window, frame);

    for (Enemy& enemy : patherEnemies)
        enemy.updateSprite(window, frame, patherBullets, timeline.getCurrentFrame());
    for (Tool& tool : tools)
        tool.update(window, frame, tools);
}