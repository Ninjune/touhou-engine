#include "Bullet.h"


Bullet::Bullet(sf::Texture& texture,
    int type,
    float speed,
    sf::Vector2f spawnPos,
    bool playerOwned
)
{
    setup(texture, type, speed, spawnPos, playerOwned);
}


Bullet::Bullet(sf::Texture& texture,
    int type,
    float speed,
    sf::Vector2f
    spawnPos
)
{
    setup(texture, type, speed, spawnPos, false);
}


void Bullet::setup(sf::Texture& texture,
    int type,
    float speed,
    sf::Vector2f spawnPos,
    bool playerOwned
)
{
    spriteWidth = 16;
    spriteHeight = 32;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(239, 160, 64, 12)); // l = 191
    sprite.setOrigin(64, 0);
    sprite.setScale(2, 2);
    sprite.setRotation(270);
    bulletType = type;
    bulletSpeed = speed;
    first = true;
    spawnPosition = spawnPos;
    aliveFrames = 0;
}


void Bullet::updateSprite(sf::RenderWindow& window, int frame)
{
    if (bulletType == 0) // player
    {
        // 239 -> 191 over 11(?) frames
        if (first)
        {
            sprite.setPosition(spawnPosition);
            first = false;
        }
        else
        {
            sprite.move(0, -bulletSpeed);

            window.draw(sprite);
            // window.draw(hitbox);
        }

        if (aliveFrames < 11)
        {
            sprite.setTextureRect(sf::IntRect(sprite.getTextureRect().left - 4, sprite.getTextureRect().top,
                sprite.getTextureRect().width, sprite.getTextureRect().height));
        }
    }

    aliveFrames++;
}


float Bullet::getBulletSpeed()
{
    return bulletSpeed;
}


void Bullet::setBulletSpeed(float in)
{
    bulletSpeed = in;
}


int Bullet::getBulletType()
{
    return bulletType;
}


void Bullet::setBulletType(int in)
{
    bulletType = in;
}


bool Bullet::isPlayerOwned()
{
    return playerOwned;
}