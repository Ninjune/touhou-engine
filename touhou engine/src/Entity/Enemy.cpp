#include "Enemy.h"
#include <iostream>


Enemy::Enemy(int type, sf::Texture& texture)
{
    init(type, texture);
}


void Enemy::init(int type, sf::Texture& texture)
{
    movementFrame = 0;
    spriteLeft = 0;
    spriteTop = 0;
    spriteWidth = 0;
    spriteHeight = 0;
    moveX = 0;
    moveY = 0;
    point = 1;

    setType(type);
    setTexture(texture);
    pathFine = true;
    sprite.setOrigin(spriteWidth / 2, spriteHeight / 2);
    sprite.setTextureRect(sf::IntRect(spriteLeft, spriteTop, spriteWidth, spriteHeight));
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

        sprite.setScale(1.3, 1.3);
    }
}


void Enemy::setPathSpeed(int speed)
{
    path.setPathSpeed(speed);
}


void Enemy::setPosition(sf::Vector2f pos)
{
    for (int i = 0; i < path.size(); i++)
    {
        path[i].x = path[i].x + (pos.x  - sprite.getPosition().x);
        path[i].y = path[i].y + (pos.y - sprite.getPosition().y);
    }
}


void Enemy::setPlayableArea(sf::RectangleShape in) // means this enemy is in stage editor
{
    playableArea = in;
    sprite.setScale(1, 1);
}


void Enemy::pushToPath(sf::Vector2f point)
{
    path.push_back(point);
    if(path.size() < 1)
        sprite.setPosition(path[0]);
}


Path Enemy::getPath()
{
    return path;
}


void Enemy::clearPath()
{
    path.clear();
    point = 1; // reset location of path
    movementFrame = -1;
}


// note: do NOT call without setting texture
void Enemy::updateSprite(sf::RenderWindow& window, int frame, std::vector<Bullet>& bullets, int currentFrame)
{
    if (path.size() < 1) return;
    if (frame % 6 == 0) // 1/10 of a second
    {
        spriteLeft += 32;
        if (spriteLeft >= 160)
            spriteLeft = 0;

        sprite.setTextureRect(sf::IntRect(spriteLeft, spriteTop, 32, 32));
    }

    for (Bullet& bullet : bullets)
    {
        if (bullet.isPlayerOwned() && getHitbox().intersects(bullet.getHitbox()))
        {
            setRender(false);
            bullet.setRender(false);
        }
    }

    if (currentFrame == -1)
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
            std::cout << timer.getElapsedTime().asMilliseconds() << " " << 60 * path.getPathSpeed() / path.size() << "\n";
            moveToPoint(path[point], 60 * path.getPathSpeed() / path.size());
            point++;
        }
    }
    else 
    {
        if (currentFrame < path.size())
            sprite.setPosition(playableToPather(path[currentFrame], window));
        else
            sprite.setPosition(playableToPather(path[path.size() - 1], window));
    }
    

    window.draw(sprite);
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


bool Enemy::isPathFine()
{
    return pathFine;
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
