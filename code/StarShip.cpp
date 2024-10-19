/*
Author: Dawson Pent
Last Date Modified: 9/25/24

Description:
This is the program file which implements the starship class to:
 - Calculate current location and movement information based on
   key inputs gathered in game to move the starship.
 - Detect collisions between objects to kill the starship
 - Store the starship values of health (lives left)
*/

#include "StarShip.h"

// This the constructor function for the ship
StarShip::StarShip(float X, float startY, RenderWindow* window)
{
    //set the x and initial Y based on starship
    m_Position.x = X;
    m_Position.y = startY;

    //initialize the laser array
    for (int i = 0; i < m_ShotNum; ++i)
    {
        new(&shots[i]) Laser();
    }

    //initialize the shape size and position
    m_Shape.setSize(sf::Vector2f(m_Width, m_Height));
    m_Shape.setPosition(m_Position);

    // Set the texture and sprite
    //debugging texture loading issues
    std::string imagePath = "graphics/StarShip.png";
    #ifdef DEBUG
    //debug printing the path
    printf("Attempting to load image from: %s\n", imagePath);
    #endif
    if (!m_ShipImage.loadFromFile(imagePath))
    {
        //image wasn't loaded, set the ship to a purple rectangle
        m_Shape.setFillColor(Color(200, 40, 200));
        m_Shape.setOutlineThickness(1);
    }
    else
    {
        //image successfully loaded, set the ship texture to be an image
        m_Sprite.setTexture(m_ShipImage);
        m_Shape.setOutlineThickness(0);
    }
    m_Sprite.setPosition(0, 0);
   
    //initialize the window for drawing
    m_Window = window;
    
    //initialize screen size
    m_ScreenWidth = (float) m_Window->getSize().x;
    m_ScreenHeight = (float) m_Window->getSize().y;
    m_MaxHeight = m_ScreenHeight - 0.3f * m_ScreenHeight; //only be able to move up about 30% of the screen
}

FloatRect StarShip::getPosition()
{
    //return the shape position
    return m_Shape.getGlobalBounds();
}

RectangleShape StarShip::getShape()
{
    //return the shape
    return m_Shape;
}

int StarShip::checkDamage(FloatRect other)
{
    //check if they intersect. If so, do damage
    if (other.intersects(this->getPosition()))
    {
        //objects intersect, do damage and delete item
        m_Health -= 1;
        return m_Damage;
    }

    //otherwise, return a -1 for no damage
    return -1;
}

void StarShip::moveLeft(bool active)
{
    //set left to be -1 to move the ship left if active
    m_Left = (active) ? -1 : 0;
}

void StarShip::moveRight(bool active)
{
    //set right to be 1 to move the ship right if active
    m_Right = (active) ? 1 : 0;
}

void StarShip::moveUp(bool active)
{
    //set up to be -1 to move the ship up if active
    m_Up = (active) ? -1 : 0;
}

void StarShip::moveDown(bool active)
{
    //set up to be 1 to move the ship up if active
    m_Down = (active) ? 1 : 0;
}

void StarShip::undoMove()
{
    //some kind of conflict with movement arose, undo the movement
    m_Position.x = m_PrevPosition.x;
    m_Position.y = m_PrevPosition.y;
}

void StarShip::shoot(bool active)
{
    //set shooting to be active to start shooting
    m_Shooting = active;
}

Laser* StarShip::getLaserArray() {
    //returning the pointer to the laser array
    return shots;
}

int StarShip::getHealth()
{
    //returning the current ship health
    return m_Health;
}

void StarShip::setHealth(int health)
{
  m_Health = health;
}

void StarShip::draw()
{
    //first draw the ship
    if (m_Shape.getOutlineThickness() == 1)
    {
        //image didn't load, draw the purple square
        m_Window->draw(m_Shape);
    }
    else
    {
        //image loaded, draw the sprite
        m_Window->draw(m_Sprite);
    }
    
    //now draw all of the lasers
    for (Laser shot : shots)
    {
        if (shot.getHealth() == 0)
        {
            //skip drawing if not a live shot
            continue;
        }
        m_Window->draw(shot.getShape());
    }
}

bool StarShip::update(Time dt)
{
    // Updating the shots
    //shoot if you can
    if (m_Shooting && m_CurrentShootDelay <= 0)
    { //starting the current shot so it shoots off as a laser
        shots[m_CurrentShot].startLaser(m_Position.x + m_Width/2 - 2, m_Position.y);
        m_CurrentShot = (++m_CurrentShot < m_ShotNum) ? m_CurrentShot : 0;
        m_CurrentShootDelay = m_ShootDelay;
    }
    //decrement shoot delay so we can shoot the next one fast until it hits 0
    if (m_CurrentShootDelay > 0)
    {
        m_CurrentShootDelay = m_CurrentShootDelay - dt.asSeconds();
    }
    
    //update all of the shots
    for (int i = 0; i < m_ShotNum; ++i)
    {
        shots[i].update(dt);
    }

	// Update the ship location based on direction
    //save the current position
    m_PrevPosition.x = m_Position.x;
    m_PrevPosition.y = m_Position.y;
    //move the ship
    m_Position.x += (m_Left + m_Right) * m_Speed * dt.asSeconds();
    m_Position.y += (m_Up + m_Down) * m_Speed * dt.asSeconds();

    // Ensuring the positions aren't out of bounds
    if (m_Position.x < 15)
    { //too far left, set position back to within bounds
        m_Position.x = 15.0f;
        m_Left = 0;
    }
    else if (m_Position.x > m_ScreenWidth - (15 + m_Width))
    { //too far right, set position back to within bounds
        m_Position.x = m_ScreenWidth - (15 + m_Width);
        m_Right = 0;
    }
    if (m_Position.y < m_MaxHeight)
    { //too high, set position back to within bounds
        m_Position.y = m_MaxHeight;
        m_Up = 0;
    }
    else if (m_Position.y > m_ScreenHeight - 50)
    { //too far down, set position back to within bounds
        m_Position.y = m_ScreenHeight - 50.0f;
        m_Down = 0;
    }

    // Move the ship
    m_Shape.setPosition(m_Position);
    m_Sprite.setPosition(m_Position);

    // Returning that the laser still exists
    return (m_Health != 0);
}
