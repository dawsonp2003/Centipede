/*
Author: Dawson Pent
Last Date Modified: 9/29/24

Description:
This is the program file which implements the mushroom class to:
 - Hold the basic value for a generated mushroom
 - Detect collisions between objects to do damage / stop movement
*/

#include "Mushroom.h"

Mushroom::Mushroom( float X, float Y, Texture* mushroomImage3, bool spriteLoad3, Texture* mushroomImage2, bool spriteLoad2,
                    Texture* mushroomImage1, bool spriteLoad1, Texture* mushroomImage0, bool spriteLoad0, RenderWindow* window)
{
    //set the x and y for the mushroom
    m_Position.x = X;
    m_Position.y = Y;

    //initialize the shape size and position
    m_Shape.setSize(sf::Vector2f(m_Width, m_Height));
    if (!spriteLoad3 || !spriteLoad2 || !spriteLoad1 || !spriteLoad0)
    {
        //one of the sprites didn't load, set the shape color to purple so it can still be drawn
        m_Shape.setFillColor(Color(200, 100, 200));
    }
    m_Shape.setPosition(m_Position);

    //initialize the sprites
    m_SpriteLoad3 = spriteLoad3;
    m_SpriteLoad2 = spriteLoad2;
    m_SpriteLoad1 = spriteLoad1;
    m_SpriteLoad0 = spriteLoad0;
    m_Sprite3.setTexture(*mushroomImage3);
    m_Sprite3.setPosition(m_Position);
    m_Sprite2.setTexture(*mushroomImage2);
    m_Sprite2.setPosition(m_Position);
    m_Sprite1.setTexture(*mushroomImage1);
    m_Sprite1.setPosition(m_Position);
    m_Sprite0.setTexture(*mushroomImage0);
    m_Sprite0.setPosition(m_Position);
   
    //initialize the window for drawing
    m_Window = window;
}

FloatRect Mushroom::getPosition()
{
    //return the shape position
    return m_Shape.getGlobalBounds();
}

RectangleShape Mushroom::getShape()
{
    //return the shape
    return m_Shape;
}

int Mushroom::checkDamage(FloatRect other, int damage)
{
    //otherwise, check if they intersect. If so, do damage
    if (other.intersects(this->getPosition()))
    {
        //objects intersect, do damage and delete item
        m_Health -= damage;
        return 0;
    }

    //otherwise, return a -1 for no damage
    return -1;
}

void Mushroom::doDamage(int damage)
{
    //ensure timer is valid to only allow damage a few times a second
    // prevents the mushroom being insta destroyed if an object gets stuck in it
    if (m_HealthTimer >= 200)
    { //enough time has passed
        //do the damage
        m_Health -= damage;
	    m_HealthTimer = 0;
    }
}

int Mushroom::getHealth()
{
    //returning the current mushroom health
    return m_Health;
}

int Mushroom::getPoints()
{
    //returning the point value of the mushroom
    return m_Points;
}

void Mushroom::draw(Time dt)
{
    //updating the health timer (limit it to 1000 max)
    m_HealthTimer = (m_HealthTimer >= 1000.0f) ? m_HealthTimer : m_HealthTimer + dt.asMilliseconds();

    //draw the mushroom differently based on its health
    if (m_Health <= 3)
    { //draw the mushroom 3 sprite for final bit of health
        //ensuring the sprite was loaded
        if (m_SpriteLoad3)
        {
            m_Window -> draw(m_Sprite3);
        }
        else
        { //didn't load, draw the purple shape
            m_Window -> draw(m_Shape);
        }
    }
    else if (m_Health <= 6)
    { //draw the mushroom 2 sprite for extensive damage
        //ensuring the sprite was loaded
        if (m_SpriteLoad2)
        {
            m_Window -> draw(m_Sprite2);
        }
        else
        { //didn't load, draw the purple shape
            m_Window -> draw(m_Shape);
        }
    }
    else if (m_Health <= 9)
    { //draw the mushroom 1 sprite for partial damage
        //ensuring the sprite was loaded
        if (m_SpriteLoad1)
        {
            m_Window -> draw(m_Sprite1);
        }
        else
        { //didn't load, draw the purple shape
            m_Window -> draw(m_Shape);
        }
    }
    else
    { //draw the mushroom 0 sprite for basically no damage
        //ensuring the sprite was loaded
        if (m_SpriteLoad0)
        {
            m_Window -> draw(m_Sprite0);
        }
        else
        { //didn't load, draw the purple shape
            m_Window -> draw(m_Shape);
        }
    }
}
