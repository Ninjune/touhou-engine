#include "Enemy.h"
#include <iostream>


Enemy::Enemy(int type,
    sf::Texture& texture
)
{
    init(type, texture);
}


void Enemy::init(int type, 
    sf::Texture& texture
)
{
    movementFrame = 0;
    spriteLeft = 0;
    spriteTop = 0;
    spriteWidth = 0;
    spriteHeight = 0;
    moveX = 0;
    moveY = 0;
    point = 1;
    startFrame = 0;

    setType(type);
    setTexture(texture);
    sprite.setOrigin(spriteWidth / 2, spriteHeight / 2);
    sprite.setTextureRect(sf::IntRect(spriteLeft, spriteTop, spriteWidth, spriteHeight));

    originAdjust.x = -1;
    originAdjust.y = -1;
}


// note: do not call without setting texture
void Enemy::updateSprite(std::map<std::string, sf::Texture>& textureMap,
    sf::RenderWindow& window,
    int frame,
    std::vector<std::vector<std::vector<Bullet>>>& bullets,
    int stageFrame,
    bool pather
)
{
    if (path.size() < 1) return;
    if (frame % 6 == 0) // 1/10 of a second
    {
        spriteLeft += 32;
        if (spriteLeft >= 160)
            spriteLeft = 0;

        sprite.setTextureRect(sf::IntRect(spriteLeft, spriteTop, 32, 32));
    }

    if (stageFrame == -1)
    {
        if (movementFrame > 0)
        {
            sprite.move(moveX, moveY);
        }
        movementFrame--;

        if (point < path.size() && movementFrame <= 0)
        {
            moveX = 0;
            moveY = 0;
            //std::cout << timer.getElapsedTime().asMilliseconds() << " " << 60 * path.getPathSpeed() / path.size() << "\n";
            moveToPoint(path[point], 60 * path.getPathSpeed() / path.size()); 
            point++;
        }
    }
    else
    {
        sf::Vector2f pos;
        if (stageFrame >= startFrame && stageFrame < startFrame + path.size())
            pos = path[stageFrame - startFrame];
        else
            pos = path[path.size() - 1];

        if (pather)
            pos = playableToPather(pos, window);

        sprite.setPosition(pos);
    }

    for (unsigned int i = 0; i < patterns.size(); i++)
    {
        patterns[i].update(window, textureMap, bullets, stageFrame,
            sprite.getPosition(), patternStartTimes[i]);
    }

    if (stageFrame < startFrame || stageFrame > startFrame + path.size())
        return setRender(false);
    else
        setRender(true);

    originAdjust.x = -1;
    originAdjust.y = -1;
    if(getRender())
        window.draw(sprite);
}


void Enemy::setTexture(sf::Texture& texture)
{
    sprite.setTexture(texture);
}


void Enemy::setType(int type)
{
    enemyType = type;

    if (enemyType < 4 && enemyType >= 0)
    {
        spriteLeft = 0;
        spriteTop = 320 + 32 * type;
        spriteWidth = 32;
        spriteHeight = 32;

        sprite.setScale((float)1.3, (float)1.3);
    }
}


void Enemy::setPathSpeed(int speed)
{
    path.setPathSpeed(speed);
}


void Enemy::setPosition(sf::Vector2f pos,
    std::vector<std::vector<std::vector<Bullet>>>& bullets
)
{
    for (unsigned int i = 0; i < path.size(); i++)
    {
        path[i].x += pos.x - sprite.getPosition().x;
        path[i].y += pos.y - sprite.getPosition().y;
    }
    int bulletsIndex;
    sf::Vector2f adjust;
    adjust.x = pos.x - sprite.getPosition().x;
    adjust.y = pos.y - sprite.getPosition().y;

    for (BulletPattern& pattern : patterns)
    {
        bulletsIndex = pattern.getBulletIndex();
        for (unsigned int i = 0; i < bullets[bulletsIndex].size(); i++)
        {
            for (unsigned int j = 0; j < bullets[bulletsIndex][i].size(); j++)
            {
                bullets[bulletsIndex][i][j].adjust(path, pattern.getFrequency(), i);
            }
        }
    }
    
}


void Enemy::setPlayableArea(sf::RectangleShape in) // means this enemy is in stage editor
{
    playableArea = in;
    sprite.setScale(1, 1);
}


void Enemy::setStartFrame(int in)
{
    startFrame = in;
}


void Enemy::pushToPath(sf::Vector2f point)
{
    path.push_back(point);
    if(path.size() < 1)
        sprite.setPosition(path[0]);
}


void Enemy::pushToPatterns(BulletPattern pattern, int startTime)
{
    patterns.push_back(pattern);
    patternStartTimes.push_back(startTime);
}


void Enemy::eraseFromPatterns(int currentFrame)
{
    int foundIndex = -1;

    for (unsigned int i = 0; i < patterns.size(); i++)
        if (patternStartTimes[i] == currentFrame)
            foundIndex = i;

    if (foundIndex >= 0)
    {
        patterns.erase(patterns.begin() + foundIndex);
        patternStartTimes.erase(patternStartTimes.begin() + foundIndex);
    }
}


Path Enemy::getPath()
{
    return path;
}


std::vector<std::string> Enemy::getPatterns()
{
    std::vector<std::string> returns;
    for (BulletPattern& pattern : patterns)
        returns.push_back(pattern.getName());
    return returns;
}


void Enemy::setPatterns(std::map<std::string, BulletPattern>& patternMap,
    std::vector<std::string> patternsToAdd,
    std::vector<int> startTimes
)
{
    for (int i = 0; i < patternsToAdd.size(); i++)
    {
        if (!patternMap.count(patternsToAdd[i]))
            continue;
        patterns.push_back(patternMap[patternsToAdd[i]]);
        patternStartTimes.push_back(startTimes[i]);
    }
}


std::vector<int> Enemy::getStartTimes()
{
    return patternStartTimes;
}


int Enemy::getStartFrame()
{
    return startFrame;
}


void Enemy::clearPath()
{
    path.clear();
    point = 1; // reset location of path
    movementFrame = -1;
}


void Enemy::moveToPoint(sf::Vector2f pointToMove, int framesToTake)
{
    moveX = abs(pointToMove.x - sprite.getPosition().x);
    moveY = abs(pointToMove.y - sprite.getPosition().y);

    if (pointToMove.x - sprite.getPosition().x < 0)
        moveX *= -1;

    if (pointToMove.y - sprite.getPosition().y < 0)
        moveY *= -1;

    moveX /= framesToTake+1;
    moveY /= framesToTake+1;

    movementFrame = std::max(framesToTake, 1);
}


sf::Vector2f Enemy::playableToPather(sf::Vector2f point, sf::RenderWindow& window)
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