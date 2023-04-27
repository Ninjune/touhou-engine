#include "Bullet.h"


Bullet::Bullet(std::map<std::string, sf::Texture>& textureMap,
    sf::Vector2f originIn,
    int directionIn,
    int spawnDirectionIn,
    double velocityIn,
    std::string bulletTypeIn,
    bool playerOwned
)
{
    setup(textureMap, originIn, directionIn, spawnDirectionIn, velocityIn, bulletTypeIn, playerOwned);
}


void Bullet::setup(std::map<std::string, sf::Texture>& textureMap,
    sf::Vector2f originIn,
    int directionIn,
    int spawnDirectionIn,
    double velocityIn,
    std::string bulletTypeIn,
    bool playerOwned
)
{
    spriteWidth = 16;
    spriteHeight = 32;
    if (playerOwned)
    {
        sprite.setTexture(textureMap["player"]);
        sprite.setTextureRect(sf::IntRect(239, 160, 64, 12)); // l = 191
        sprite.setOrigin(64, 0);
        sprite.setScale(2, 2);
        sprite.setRotation(270);
    }
    else
    {
        sprite.setTexture(textureMap["bullet1"]);

        if (bulletTypeIn == "orb")
        {
            sprite.setTextureRect(sf::IntRect(0, 48, 16, 16));
            sprite.setOrigin(8, 8);
        }
        
        if (directionIn == -1)
        {
            // go from player TBA
        }

        sprite.setRotation(directionIn);
    }

    direction = directionIn;
    velocity = velocityIn;
    sprite.setPosition(originIn);
    aliveFrames = 0;
}


void Bullet::updateSprite(sf::RenderWindow& window, int frame)
{
    if (playerOwned) // player
    {
        // 239 -> 191 over 11(?) frames
        sprite.move(0, -velocity);

        if (aliveFrames < 11)
        {
            sprite.setTextureRect(sf::IntRect(sprite.getTextureRect().left - 4, sprite.getTextureRect().top,
                sprite.getTextureRect().width, sprite.getTextureRect().height));
        }

        window.draw(sprite);
        aliveFrames++;
    }
    else
        window.draw(sprite);
}


void Bullet::simulateFrames(int simulatedFrames, sf::Vector2f origin)
{
    if (playerOwned)
        return;
    // move bullet simulatedFrames*movePerFrame
    // split velocity between x and y based on direction 0 = up, 90 = right, 180 = down, 270 = left
    // x = right, -x = left, -y = up, y = down
    // HERE
    sprite.setPosition(origin);
    //for(int i = 0; i < simulatedFrames; i++)
        //sprite.move();
}


float Bullet::getBulletSpeed()
{
    return velocity;
}


void Bullet::setBulletSpeed(float in)
{
    velocity = in;
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