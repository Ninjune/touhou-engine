#include "Bullet.h"


Bullet::Bullet(std::map<std::string, sf::Texture>& textureMap,
    sf::Vector2f originIn,
    int directionIn,
    int spawnDirectionIn,
    double velocityIn,
    std::string bulletTypeIn,
    bool playerOwnedIn
)
{
    setup(textureMap, originIn, directionIn, spawnDirectionIn, velocityIn, bulletTypeIn, playerOwnedIn);
}


void Bullet::setup(std::map<std::string, sf::Texture>& textureMap,
    sf::Vector2f originIn,
    int directionIn,
    int spawnDirectionIn,
    double velocityIn,
    std::string bulletTypeIn,
    bool playerOwnedIn
)
{
    playerPos.x = -1;
    playerPos.y = -1;
    playerOwned = playerOwnedIn;
    spriteWidth = 16;
    spriteHeight = 32;
    life = 1;
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
            modifyHitbox(0.5);
        }

        sprite.setRotation(directionIn);
    }

    direction = directionIn;
    velocity = velocityIn;
    sprite.setPosition(originIn);
    aliveFrames = 0;
    firstFrame = true;
}


void Bullet::updateSprite(sf::RenderWindow& window, int frame)
{
    if (playerOwned && getRender()) // player
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
    else if (getRender() && life > 0)
        window.draw(sprite);
}


void Bullet::simulateFrames(sf::RenderWindow& window,
    int simulatedFrames,
    sf::Vector2f originIn,
    sf::Vector2f playerPosIn,
    bool& inBounds
)
{
    if (playerPos.x == -1)
        playerPos = playerPosIn;

    int mag;
    sf::Vector2f diff;
    sf::Vector2f directionVector;

    if (playerOwned)
        return;
    origin = originIn;
    if (direction == -1)
    {
        diff.x = playerPos.x - origin.x;
        diff.y = playerPos.y - origin.y;
        mag = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
        directionVector.x = diff.x / mag;
        directionVector.y = diff.y / mag;

        direction = atan2(directionVector.y, directionVector.x) * 180 / 3.1415;

        if (direction < 0)
            direction += 360;
    }

    sprite.setPosition(origin);
    /*bool negX = direction - 180 >= 0;
    bool negY = direction < 91 || direction > 270;
    float percentX = negX ? 1. * (direction % 90) / 90 : -1;
    float percentY = negX ? 1. - percentX : 1. * (direction % 90)/90;*/
    //if (percentX == -1)
    //    percentX = 1. - percentY;
    //for(int i = 0; i < simulatedFrames; i++)
    //    sprite.move(velocity * percentX * (negX ? -1 : 1),
    //       velocity * percentY * (negY ? -1 : 1)
    //    );
    for (int i = 0; i < simulatedFrames; i++)
    {
        sprite.move(velocity * cos(direction * 3.1415 / 180), 
            velocity * sin(direction * 3.1415 / 180));
    }
    if (sf::FloatRect(sf::Vector2f(0, 0),
        sf::Vector2f(window.getSize().x, window.getSize().y))
        .contains(sprite.getPosition())
        )
        inBounds = true;

    window.draw(sprite);
}


void Bullet::adjust(std::vector<sf::Vector2f> path, int frequency, int index)
{
    origin.x = path[frequency * index].x;
    origin.y = path[frequency * index].y;
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