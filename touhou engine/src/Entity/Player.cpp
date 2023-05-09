#include "Player.h"
#include <iostream>

Player::Player(sf::RenderWindow& window,
        std::map<std::string, sf::Texture>& textureMap
    )
{
    spriteLeft = 0;
    spriteTop = 0;

    sprite.setTexture(textureMap["player"]);
    hitboxTexture.loadFromFile("textures/player/hitbox.png");
    sprite.setTextureRect(sf::IntRect(spriteLeft, spriteTop, 32, 50));
    sprite.setOrigin(16, 25);
    sprite.setScale(2, 2);
    sprite.setPosition(window.getSize().x/2.f,
        window.getSize().y - window.getSize().y/6.f);

    shootSpeed = 5;
    speed = 6;
    shootCooldown = 0;
    life = 3;

    hitbox.setSize(sf::Vector2f(16, 16));
    hitbox.setTexture(&hitboxTexture);
    hitbox.setOrigin(8, 8);
    hitbox.setScale(1.3f, 1.3f);

    movementKeyMap[sf::Keyboard::Left] = sf::Vector2f(-speed, 0);
    movementKeyMap[sf::Keyboard::Right] = sf::Vector2f(speed, 0);
    movementKeyMap[sf::Keyboard::Up] = sf::Vector2f(0, -speed);
    movementKeyMap[sf::Keyboard::Down] = sf::Vector2f(0, speed);
    shootKey = sf::Keyboard::Z;

    for (std::pair<sf::Keyboard::Key, sf::Vector2f> const& key : movementKeyMap)
        movementKeys.push_back(key.first);

    rightPhase = 0;
    leftPhase = 0;
} 


void Player::updateSprite(sf::RenderWindow& window,
    int frame,
    std::vector<Bullet>& playerBullets,
    std::vector<Enemy>& enemies,
    std::vector<std::vector<std::vector<Bullet>>>& bullets,
    std::map<std::string, sf::Texture>& textureMap
)
{
    bool goingDirection = false;
    bool drawHitbox = false;
    int currentSpeed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        currentSpeed = speed/2;
        hitbox.setPosition(sprite.getPosition());
        hitbox.setRotation(hitbox.getRotation() + 0.5);
        drawHitbox = true;
    }
    else
    {
        currentSpeed = speed;
        drawHitbox = false;
    }

    for (sf::Keyboard::Key key : movementKeys)
    {
        if (sf::Keyboard::isKeyPressed(key))
        {
            movementKeyMap[sf::Keyboard::Left] = sf::Vector2f(-currentSpeed, 0);
            movementKeyMap[sf::Keyboard::Right] = sf::Vector2f(currentSpeed, 0);
            movementKeyMap[sf::Keyboard::Up] = sf::Vector2f(0, -currentSpeed);
            movementKeyMap[sf::Keyboard::Down] = sf::Vector2f(0, currentSpeed);

            if (movementKeyMap[key].x < 0) // left
            {
                rightPhase = 0;

                if(leftPhase == 0 && !goingDirection)
                    leftPhase = 1;

                goingDirection = true;

                if(sprite.getPosition().x - sprite.getOrigin().x*2 -
                    movementKeyMap[key].x > 0)
                    sprite.move(movementKeyMap[key]);
            }
            else if (movementKeyMap[key].x > 0) // right
            {
                leftPhase = 0;
                
                if(rightPhase == 0 && !goingDirection)
                    rightPhase = 1;

                goingDirection = true;

                if (sprite.getPosition().x + sprite.getOrigin().x*2  - 
                    movementKeyMap[key].x < window.getSize().x)
                    sprite.move(movementKeyMap[key]);
            }
            else if (movementKeyMap[key].y < 0) // up
            {
                if (sprite.getPosition().y - sprite.getOrigin().y * 2 - 
                    movementKeyMap[key].y > 0)
                    sprite.move(movementKeyMap[key]);
            }
            else if (movementKeyMap[key].y > 0) // down
            {
                if (sprite.getPosition().y + sprite.getOrigin().y*2 - 
                    movementKeyMap[key].y < window.getSize().y)
                    sprite.move(movementKeyMap[key]);
            }
        }
    }    

    if (!goingDirection)
    {
        leftPhase = 0;
        rightPhase = 0;
    }

    if (frame % 6 == 0) // 1/10 of a second
    {
        spriteTop = 0;

        if (leftPhase >= 1)
        {
            spriteTop = 50;

            if (leftPhase > 7)
                leftPhase = 4;

            spriteLeft = 32 * leftPhase;
            leftPhase++;
        }
        else if (rightPhase >= 1)
        {
            spriteTop = 100;

            if (rightPhase > 7)
                rightPhase = 4;

            spriteLeft = 32 * rightPhase;
            rightPhase++;

        }
        else
        {
            spriteLeft += 32;
            if (spriteLeft >= 224)
                spriteLeft = 0;
        }

        sprite.setTextureRect(sf::IntRect(spriteLeft, spriteTop, 32, 40));
    }

    if (sf::Keyboard::isKeyPressed(shootKey))
    {
        if (shootCooldown <= 0)
        {
            playerBullets.push_back(Bullet(textureMap,
                sf::Vector2f(getPosition().x, getPosition().y - 112),
                270,
                -1,
                15,
                "",
                true
            ));
            playerBullets.push_back(Bullet(textureMap,
                sf::Vector2f(getPosition().x-25, getPosition().y - 112),
                270,
                -1,
                15,
                "",
                true
            ));
            shootCooldown = (60 / shootSpeed);
        }
    }
    shootCooldown--;

    for (Bullet& bullet : playerBullets)
    {
        for (Enemy& enemy : enemies)
        {
            sf::FloatRect hb1 = enemy.getHitbox();
            sf::FloatRect hb2 = bullet.getHitbox();

            if (hb1.intersects(hb2) && enemy.getRender() && bullet.getRender())
            {
                bullet.setRender(false);
                enemy.setRender(false);
            }
        }
    }

    for (int i = 0; i < bullets.size(); i++)
    {
        for (int j = 0; j < bullets[i].size(); j++)
        {
            for (Bullet& bullet : bullets[i][j])
            {
                if (bullet.getHitbox().intersects(getHitbox()) && immunity <= 0 &&
                    bullet.getRender())
                {
                    bullet.setRender(false);
                    life -= 1;
                    immunity = 240;
                }
            }
        }
    }

    if (lifeStars.size() < life)
    {
        for(int i = 0; i < life; i++)
            lifeStars.push_back(sf::Sprite());
    }
    else if (lifeStars.size() > life)
    {
        while(lifeStars.size() > life)
            lifeStars.pop_back();
    }

    for (int i = 0; i < life; i++)
    {
        
        lifeStars[i].setTexture(textureMap["lifeStar"]);
        lifeStars[i].setPosition(window.getSize().x - 26 * i - 26, 10);
        lifeStars[i].setScale(2, 2);
        window.draw(lifeStars[i]);
    }
    
    immunity--;
    window.draw(sprite);
    if (drawHitbox)
        window.draw(hitbox);
}