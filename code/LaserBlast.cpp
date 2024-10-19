/*
Author: Dawson Pent
Last Date Modified: 9/25/24

Description:
This is the program file which implements the laser class to:
 - Calculate current location and movement information for the laser blast
 - Detect collisions between objects to decrease their health
 - Be scaleable to many laser blasts
*/ 

#include "LaserBlast.h"

// This the constructor function for the laser
Laser::Laser()
{
    //set the laser's x and initial Y based on starship
	m_Position.x = -10;
	m_Position.y = -10;

    //initialize the shape and position
	m_Shape.setSize(sf::Vector2f(m_Width, m_Height));
	m_Shape.setPosition(m_Position);
    m_Shape.setFillColor(Color(200, 50, 50));
}

FloatRect Laser::getPosition()
{
    //return the shape position
	return m_Shape.getGlobalBounds();
}

RectangleShape Laser::getShape()
{
    //return the shape
	return m_Shape;
}

void Laser::startLaser(float startX, float startY)
{
    //restarting the laser to be active at the given location
    // stops having to create new laser objects every time the player shoots
    m_Position.x = startX;
    m_Position.y = startY;
    m_Health = 1;
}

int Laser::checkDamage(FloatRect other)
{
    //otherwise, check if they intersect. If so, do damage
    if (m_Health > 0 && other.intersects(this->getPosition()))
    {
        //objects intersect, do damage and delete the laser
        m_Health -= 1;
        return m_Damage;
    }

    //otherwise, return a -1 for no damage
    return -1;
}

int Laser::getHealth()
{
    return m_Health;
}

bool Laser::update(Time dt)
{
    // Ensuring the laser exists
    if (m_Health <= 0)
    { //laser doesn't exist
        return false;
    }

	// Update the laser height
	m_Position.y -= m_Speed * dt.asSeconds();

    //ensuring the laser is still on the screen
    if (m_Position.y < 0)
    {
        m_Health = 0;
    }

	// Move the laser
	m_Shape.setPosition(m_Position);

    // Returning that the laser still exists
    return true;
}
