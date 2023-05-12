#include "Prompter.h"


Prompter::Prompter()
    : m1(sf::Mouse::Left), left(sf::Keyboard::Left), right(sf::Keyboard::Right)
{
    cursorIndex = -1;
    cursor.setFillColor(sf::Color(0, 0, 255, 255));
}


void Prompter::prompt(sf::RenderWindow& window, std::string input, int& frame)
{
    if (cursorIndex < 0)
        cursorIndex = input.size() - 1;
    sf::FloatRect textRect = getLocalBounds();
    setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);

    setPosition(window.getSize().x / 2, window.getSize().y / 2);
    setString(input);
    sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x,
        sf::Mouse::getPosition(window).y
    );
    sf::Vector2f charPos;
    sf::FloatRect charBox;
    unsigned int charSize = getCharacterSize();

    if(getGlobalBounds().contains(mousePos) && m1.consumeClick(frame, 5))
    {
        for (int i = 0; i < input.size(); i++)
        {
            charPos = findCharacterPos(i);
            charBox = sf::FloatRect(charPos.x, charPos.y,
                charSize, charSize);
            if (charBox.contains(mousePos))
                cursorIndex = i;
        }
    }

    if (left.consumeClick(frame, 6) && cursorIndex > 0)
        cursorIndex -= 1;
    if (right.consumeClick(frame, 6) && cursorIndex < input.size())
        cursorIndex += 1;

    cursor.setSize(sf::Vector2f(3, getCharacterSize()));
    //cursor.setOrigin(0, getCharacterSize() / 2);
    cursor.setPosition(findCharacterPos(cursorIndex));

    window.draw(*this);
    window.draw(cursor);
}


void Prompter::poll(sf::Event& event,
    unsigned int min,
    unsigned int max,
    std::string& inputStr,
    std::vector<char> exceptions,
    std::vector<char> includes,
    bool useCursor
)
{
    char unicode = event.text.unicode;

    if (unicode == 8 && inputStr.size() > 0)
    {
        if (useCursor)
        {
            if (cursorIndex - 1 < 0 || inputStr[cursorIndex-1] == '\n')
                return;
            inputStr.erase(cursorIndex - 1, 1);
            cursorIndex -= 1;
        }
        else
            inputStr.pop_back();
    }
    else if ((unicode >= min && unicode <= max &&
        !search(exceptions, unicode)) || 
        (search(includes, unicode))
        )
        if (useCursor)
        {
            if (cursorIndex - 1 < 0)
                return;
            inputStr.insert(cursorIndex, { unicode });
            cursorIndex += 1;
        }
        else
            inputStr += (char)event.text.unicode;
}


int Prompter::getCursorIndex()
{
    return cursorIndex;
}


bool Prompter::search(std::vector<char> arr, char val)
{
    for (int i = 0; i < arr.size(); i++)
        if (arr[i] == val)
            return true;
    return false;
}