/*
Author: Dawson Pent
Last Date Modified: 10/1/24

Description:
This is the program file which implements the centipede class to:
 - Calculate current location and movement information for this specific segment of the centipede
 - Detect collisions between objects to do damage / change movement
 - Hold the type of centipede body part (head, end, middle)
*/
#include "Centipede.h"

Centipede::Centipede(RenderWindow* window, BodyType type, int segmentNum, float* headRandom)
{
    //calculating the screen width and height while saving window pointer
    m_Window = window;
    m_ScreenWidth  = (float) m_Window -> getSize().x;
    m_ScreenHeight = (float) m_Window -> getSize().y;
    m_MaxHeight = m_ScreenHeight - .3f * m_ScreenHeight;
      
    //initializing movement and position information randomly
    float random = (type == HEAD) ? ((float) std::rand()) / RAND_MAX : *headRandom;
    *headRandom = random;
    m_DirectionX = (random < .5f) ? -1 : 1;
    m_DirectionY = 2; //generally, go downward
    m_Position.x = (random < .5f) ? m_ScreenWidth + segmentNum * 35 : 0 - m_Width - segmentNum * 35;
    m_Position.y = 105;
    m_PrevMove.x = 0;
    m_PrevMove.y = 0;
    m_PrevPosition = m_Position;

    //initializing the shape
    m_Shape.setSize(Vector2f(m_Width, m_Height));
    m_Shape.setPosition(m_Position);

    //initializing the sprites and textures
    m_HeadImage.loadFromFile("graphics/CentipedeHead.png");
    m_BodyImage.loadFromFile("graphics/CentipedeBody.png");
    m_HeadSprite.setTexture(m_HeadImage);
    m_BodySprite.setTexture(m_BodyImage);
    m_Offset.x = m_HeadSprite.getLocalBounds().width / 2.f;
    m_Offset.y = m_HeadSprite.getLocalBounds().height / 2.f;
    m_HeadSprite.setOrigin(Vector2f(m_Offset.x, m_Offset.y));
    m_HeadSprite.setPosition(m_Position);
    m_BodySprite.setPosition(m_Position);

    //initializing specific values
    m_Type = type;
    m_Points = (type == HEAD) ? 100 : 10;
}

FloatRect Centipede::getPosition()
{
    //returning the centipede's position
    return m_Shape.getGlobalBounds();
}

RectangleShape Centipede::getShape()
{
    //returning the centipede's shape
    return m_Shape;
}

int Centipede::getPoints()
{
    //returning the centipede's points
    return m_Points;
}

BodyType Centipede::getType()
{
    //returning the centipede's type
    return m_Type;
}

void Centipede::setType(BodyType type)
{
    //setting the centipede's type
    m_Type = type;
    m_Points = (type == HEAD) ? 100 : 10;
}

int Centipede::checkDamage(FloatRect other, int damage)
{
    //check if they intersect. If so, do damage
    if (other.intersects(this->getPosition()))
    {
        //objects intersect, check if need to do damage
        if (damage != 0)
        { //return 1 damage to say delete segment
            return 1;
        }
        else if (!m_Bounce)
	    { //no damage, just turn around centipede
	        m_Bounce = true;
	        if ( abs(m_DirectionY) < 2 )
	        { //moving down/up, stop that movement (may result in centipede overlap, thats okay)
	            m_DirectionY *= 2;
	            m_Position.y  = (m_DirectionY < 0) ? m_Distance + 35 : m_Distance - 35;
	        }
	        else if ( m_DirectionX == -1 )
	        { //going left, bounce off to the right after going down
	            m_Position.x += 2;
	            m_DirectionX  = 1;
	            m_DirectionY /= 2;
	            m_Distance    = (m_DirectionY < 0) ? m_Position.y - 35 : m_Position.y + 35;
            }
            else if ( m_DirectionX == 1 )
            { //going right, bounce off to the left after going down
	            m_Position.x -= 2;
	            m_DirectionX  = -1;
	            m_DirectionY /= 2;
	            m_Distance    = (m_DirectionY < 0) ? m_Position.y - 35 : m_Position.y + 35;
            }
	    }
    }

    //otherwise, return a -1 for no damage/intersection
    return -1;
}

Vector2f Centipede::update(Time dt)
{
    //saving the current position to return
    m_PrevPosition = m_Position;

    // Update the centipede location based on direction movement
    //move the centipede part
    if (abs(m_DirectionY) < 2)
    { //moving up or down
        m_Position.y += m_DirectionY * m_Speed * dt.asSeconds();
        //checking if the centipede has moved enough (keeping it within the grid)
        if ((m_DirectionY < 0) ? m_Position.y <= m_Distance : m_Position.y >= m_Distance)
        { //centipede has moved enough up/down
            m_Bounce = false;
	        m_Position.y = m_Distance;
	        m_DirectionY *= 2;
	    }
    }
    else
    { //moving left/right
        m_Position.x += m_DirectionX * m_Speed * dt.asSeconds();
    }

    // Ensuring the positions aren't out of bounds
    if ( m_Position.x < 15 && m_DirectionX < 0 )
    { //too far left, bounce off left wall (unless already moving down / right)
        m_Bounce = true;
        m_DirectionX = 1;
        m_DirectionY /= 2;
        m_Distance = (m_DirectionY < 0) ? m_Position.y - 35 : m_Position.y + 35;
        //checking if too low / high to turn around
        if (m_Position.y >= m_ScreenHeight - 80)
	    { //too low , change direction to go upwards
	        m_DirectionY = -1;
	    }
        else if (m_Position.y <= m_MaxHeight && m_DirectionY < 0)
	    { //too high (after getting down to below max), change direction to go down
	        m_DirectionY = 1;
	    }
    }
    else if ( (m_Position.x > m_ScreenWidth - (15 + m_Width)) && m_DirectionX > 0 )
    { //too far right, bounce off right wall (unless already moving left)
        m_Bounce = true;
        m_DirectionX = -1;
        m_DirectionY /= 2;
        m_Distance = (m_DirectionY < 0) ? m_Position.y - 35 : m_Position.y + 35;
        //checking if too low / high to turn around
        if (m_Position.y >= m_ScreenHeight - 80)
	    { //too low , change direction to go upwards
	        m_DirectionY = -1;
	    }
        else if (m_Position.y <= m_MaxHeight && m_DirectionY < 0)
	    { //too high (after getting down to below max), change direction to go down
	        m_DirectionY = 1;
        }
    }

    // Actually move the centipede
    m_Shape.setPosition(m_Position);
    m_HeadSprite.setPosition(m_Position + m_Offset);
    m_BodySprite.setPosition(m_Position);

    // Returning the previous movement made by the centipede to set the next particle to it
    Vector2f temp = m_PrevMove;
    m_PrevMove = m_Position - m_PrevPosition;
    return temp;
}

Vector2f Centipede::update(Time dt, Vector2f aheadPosition)
{
    //if type is head, do normal update. Otherwise, use the ahead position vector
    if (m_Type == HEAD)
    {
        return update(dt);
    }

    //simply moving by the ahead body's previous movement
    m_Position = m_Position + aheadPosition;
  
    // Move the centipede
    m_Shape.setPosition(m_Position);
    m_HeadSprite.setPosition(m_Position + m_Offset);
    m_BodySprite.setPosition(m_Position);

    // Return the previous position to set the next particle to it
    Vector2f temp(m_PrevMove);
    m_PrevMove = m_Position - m_PrevPosition;
    m_PrevPosition = m_Position;
    return temp;
}

void Centipede::draw()
{
    //based on the type of the segment, draw it differently
    if (m_Type == BODY || m_Type == TAIL)
    {
	    //just draw the body type, orientation doesn't matter
	    m_Window -> draw(m_BodySprite);
    }
    else
    { //based on the orientation of the head, draw the sprite
	    if (m_DirectionY == -1)
	    { //point the head upwards
	        m_HeadSprite.setRotation(270.0f);
	        m_Window -> draw(m_HeadSprite);
	    }
	    else if (m_DirectionY == 1)
	    { //point the head downwards
	        m_HeadSprite.setRotation(90.0f);
	        m_Window -> draw(m_HeadSprite);
	    }
	    else if (m_DirectionX == 1)
	    { //point the head right
	        m_HeadSprite.setRotation(.0f);
	        m_Window -> draw(m_HeadSprite);
	    }
	    else
	    { //point the head left
	        m_HeadSprite.setRotation(180.0f);
	        m_Window -> draw(m_HeadSprite);
	    }
    }
}