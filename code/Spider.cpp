/*
Author: Dawson Pent
Last Date Modified: 9/30/24

Description:
This is the program file which implements the spider class to:
 - Calculate current location and movement information for the spider
 - Detect collisions between objects to do damage / turn around movement
*/

#include "Spider.h"

Spider::Spider(RenderWindow* window)
{
    //set the initial x,y of the spider to off the string until alive
    m_Position.x = -100;
    m_Position.y = 0;

    //initialize the shape size and position
    m_Shape.setSize(sf::Vector2f(m_Width, m_Height));
    m_Shape.setPosition(m_Position);

    //set the texture and sprite
    //debugging texture loading issues
    std::string imagePath = "graphics/Spider.png";
    #ifdef DEBUG
    //debug printing the path
    printf("Attempting to load image from: %s\n", imagePath);
    #endif
    if (!m_SpiderImage.loadFromFile(imagePath))
    { //image wasn't loaded, throw an error by setting spider to purple
        m_Shape.setFillColor(Color(100, 40, 100));
        m_Shape.setOutlineThickness(1);
    }
    else
    { //image successfully loaded, set the ship texture to be an image
        m_Sprite.setTexture(m_SpiderImage);
        m_Shape.setOutlineThickness(0);
    }
    m_Sprite.setPosition(m_Position);
   
    //initialize the window for drawing
    m_Window = window;
    
    //initialize screen size
    m_ScreenWidth = (float) m_Window->getSize().x;
    m_ScreenHeight = (float) m_Window->getSize().y;
    m_MaxHeight = m_ScreenHeight - 0.3f * m_ScreenHeight; //only be able to move up about 30% of the screen
}

FloatRect Spider::getPosition()
{
    //return the shape position
    return m_Shape.getGlobalBounds();
}

RectangleShape Spider::getShape()
{
    //return the shape
    return m_Shape;
}

int Spider::checkDamage(FloatRect other, int damage)
{
    //check if they intersect. If so, do damage or react
    FloatRect intersection;
    if (other.intersects(this->getPosition(), intersection))
    { 
        //objects intersects, check collision response
        if (damage == 0)
        { //no damage collision, just bounce off
            //get the edge of the object that intersected
            float relativeX, relativeY;
            if (intersection.left > m_Position.x)
            { //right edge
                #ifdef DEBUG
                printf("Right edge intersection\n");
                #endif
                relativeX = 1;
                relativeY = 0;
            }
            else if (intersection.left < m_Position.x)
            { //left edge
                #ifdef DEBUG
                printf("Left edge intersection\n");
                #endif
                relativeX = -1;
                relativeY = 0;
            }
            else if (intersection.top > m_Position.y)
            { //bottom edge
                #ifdef DEBUG
                printf("Bottom edge intersection\n");
                #endif
                relativeX = 0;
                relativeY = 1;
            }
            else
            { //top edge
                #ifdef DEBUG
                printf("Top edge intersection\n");
                #endif
                relativeX = 0;
                relativeY = -1;
            }
            
            //bounce the spider away
            changeDirection(relativeX, relativeY, true);
        }
        else
        { 
            //hit an object, do damage
            m_Health -= damage;
        }
        return m_Damage;
    }

    //otherwise, return a -1 for no damage
    return -1;
}

float Spider::changeDirectionAbsolute(float directX, float directY, float prevX, float prevY, bool createVector)
{
    //hit an object, undo movement
    m_Position = m_PrevPosition;

    // Find the random direction of X and Y for the spider to move towards
    int num = 0;
    do {
        //calculate random opposite direction to go given relative directions from spider to object
        if (directX != 0)
        { //have to avoid an x direction
            //set x to be opposite
            directX *= -1; //change direction of the x to be opposite of avoidance

            //randomly set y to be -1, 0, or 1
            float random = ((float) std::rand()) / RAND_MAX; //random 0 to 1
            directY = (random < .33f) ? -1.0f : ((random < .66f) ? 0 : 1.0f); //change direction of y to be random -1, 0, or 1
        }
        else if (directY != 0)
        { //have to avoid a Y direction
            //set y to be opposite
            directY *= -1; //change direction of the y to be opposite of avoidance

            //randomly set x to be -1, 0, 1 or 50% of the time towards the player
            float random = ((float) std::rand()) / RAND_MAX; //random 0 to 1
	        if (random <= .5f)
	        { //go with desired X 50% of the time to go toward the player more
	            directX = abs(m_DesiredX)/m_DesiredX;
	        }
	        else
	        { //otherwise, randomly choose -1, 0, or 1
	            random = ((float) std::rand()) / RAND_MAX; //random 0 to 1
	            directX = (random < .33f) ? -1.0f : ( (random < .66f) ? 0.0f : 1.0f ); //change direction of x to be random -1, 0, or 1
	        }
        }
        else
        { //don't have to avoid any direction, no hit.  Just choose a random direction.
            //randomly set x to be -1, 0, 1, or 50% of the time towards the  player
            float random = ((float) std::rand()) / RAND_MAX; //random 0 to 1
            if (random < .5f)
	        { //go with desired X 50% of the time to go toward the player more
	            directX = abs(m_DesiredX)/m_DesiredX;
	        }
	        else
	        { //otherwise, randomly choose -1, 0, or 1
	            random = ((float) std::rand()) / RAND_MAX; //random 0 to 1
	            directX = (random < .33f) ? -1.0f : ( (random < .66f) ? 0.0f : 1.0f ); //change direction of x to be random -1, 0, or 1
	        }
            
            //randomly set y to be -1, 0, or 1
            random = ((float) std::rand()) / RAND_MAX; //random 0 to 1
            directY = (random < .33f) ? -1.0f : ((random < .66f) ? 0 : 1.0f); //change direction of y to be random -1, 0, or 1
        }
    }
    //try to have the new direction not along the same direction as before (if failed 3 times, just move on)
    while (((prevX == directX && prevY == directY) || (prevX == -directX && prevY == -directY)) && num++ < 3);

    // Creating the unit vector based on randomized direction of X and Y
    if (createVector)
    { //only create it if the method handler wants to
        createMovementUnitVector(directX, directY);
    }

    //returning relative y direction value calculated
    return directY;
}

void Spider::changeDirectionBound(float boundX, float boundY)
{
    //make the spider bounce off of a bound
    float directX, directY;
    if (boundX != 0)
    { //have to bounce off a wall
        //set the X to be the opposite of the bound direction
        directX = -boundX;

        //randomly define Y to be -1, 0, or 1
        float random = ((float) std::rand()) / RAND_MAX; //random 0 to 1
        directY = (random < .33f) ? -1.0f : ( (random < .66f) ? 0.0f : 1.0f ); //change direction of y to be random -1, 0, or 1
    }
    else
    { //have to bounce off the ceiling/floor
        //set the Y to be the opposite of the bound direction
        directY = -boundY;

        //randomly define X to be -1, 0, or 1
        float random = ((float) std::rand()) / RAND_MAX; //random 0 to 1
        if (random < .5f)
	    { //go with desired X 50% of the time to go toward the player more
	        directX = abs(m_DesiredX)/m_DesiredX;
	    }
        else
        { //otherwise, randomly choose -1, 0, or 1
	        random = ((float) std::rand()) / RAND_MAX; //random 0 to 1
	        directX = (random < .33f) ? -1.0f : ( (random < .66f) ? 0.0f : 1.0f ); //change direction of x to be random -1, 0, or 1
	    }
    }
    
    //creating the unit vector
    createMovementUnitVector(directX, directY);
}

float Spider::changeDirection(float directX, float directY, bool createVector)
{
    return Spider::changeDirectionAbsolute(
        directX, directY, (m_DirectionX == 0) ? 0 : abs(m_DirectionX)/m_DirectionX,
        (m_DirectionY == 0) ? 0 : abs(m_DirectionY)/m_DirectionY, createVector
        );
}

void Spider::createMovementUnitVector(float X, float Y)
{
    //correcting the values of x/y
    X = (Y != 0.0f) ? X*.707f : X;
    Y = (X != 0.0f) ? Y*.707f : Y;
    //set direction variables
    m_DirectionX = X;
    m_DirectionY = Y;
}

void Spider::startMovement()
{ //randomly choose left vs right size and height level / up/down direction to start with
    // Initializing spider variables
    m_Health = 1;
    m_Speed = floor((((float) std::rand()) / RAND_MAX) * 3.0f + 4.0f);
    m_Points = 300 + (int) floor((m_Speed - 4.f) * 200.f);
    m_Speed *= 60.0f;
    #ifdef DEBUG
    printf("Speed: %f\n", m_Speed);
    #endif

    //getting the random float to generate left vs right and height
    float random = ((float) std::rand()) / RAND_MAX; //random 0 to 1
    if (random >= .5f)
    { //generate on the right edge
        //get the relative height value
        random = (random-.5f) / .5f; //getting relative percentage in top half
        random = (m_ScreenHeight - m_MaxHeight - 90) * random + 45; //getting relative y value in allowed bottom portion (with margins)
        random += m_MaxHeight; //getting the actual starting y value and holding it in random
        
        //starting the movement
        float Y = changeDirectionAbsolute(1, 0, 0, 0, false); //get random y movement
        float X = 1; //forcing x direction to be positive
        createMovementUnitVector(X, Y); //creating the unit vector for movement

        //setting the new position
        m_Position.y = random;
        m_Position.x = m_ScreenWidth + 10;
    }
    else
    { //generate on the left edge
        //get the relative height value
        random = random / .5f; //getting relative percentage in bottom half
        random = (m_ScreenHeight - m_MaxHeight - 90) * random + 45; //getting relative y value in allowed bottom portion (with margins)
        random += m_MaxHeight; //getting the actual starting y value and holding it in random
        
        //starting the movement
        float Y = changeDirectionAbsolute(-1, 0, 0, 0, false); //get random y movement
        float X = 1; //forcing x direction to be positive
        createMovementUnitVector(X, Y); //creating the unit vector for movement

        //setting the new position
        m_Position.y = random;
        m_Position.x = -80;
    }

    #ifdef DEBUG
    printf("Starting spider values: P(%f, %f), D(%f, %f), S:%f\n", m_Position.x, m_Position.y, m_DirectionX, m_DirectionY, m_Speed);
    #endif
}

int Spider::getHealth()
{
    //returning the current spider health
    return m_Health;
}

int Spider::getPoints()
{
    //return how many points the spider is worth
    return m_Points;
}

void Spider::doDamage(int damage)
{
    //do damage value given
    m_Health -= damage;
}

void Spider::draw()
{
    //don't draw the spider if dead
    if (m_Health <= 0)
    { //spider is dead, don't draw it
        return;
    }
  
    //draw the spider if loaded
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
}

bool Spider::update(Time dt, float shipX)
{
    // Making sure the Spider is alive
    if (m_Health <= 0)
    {
        //spider is dead, nothing to update
        return false;
    }
    
    // Update the spider location based on the unit vector direction
    //move the Spider
    m_Position.x += m_DirectionX * m_Speed * dt.asSeconds();
    m_Position.y += m_DirectionY * m_Speed * dt.asSeconds();

    //update the desired direction
    m_DesiredX = (m_Position.x > shipX) ? -1.f : 1.f;

    // Ensuring the positions aren't out of bounds
    if ( m_Position.x < 15 && m_DirectionX < .5 )
    { //too far left, bounce off left wall (unless already moving right) and undo movement
        changeDirectionBound(-1, 0);
        m_Time = 0; //reset movement timer
    }
    else if ( (m_Position.x > m_ScreenWidth - (15 + m_Width)) && m_DirectionX != -1 )
    { //too far right, bounce off right wall (unless already moving left) and undo movement
        changeDirectionBound(1, 0);
        m_Time = 0; //reset movement timer
    }
    if ( m_Position.y < m_MaxHeight && m_DirectionY != 1 )
    { //too high, bounce off ceiling (unless already moving down) and undo movement
        changeDirectionBound(0, -1);
        m_Time = 0; //reset movement timer
    }
    else if ( (m_Position.y > m_ScreenHeight - 50) && m_DirectionY != -1 )
    { //too far down, bounce off floor (unless already moving up) and undo movement
        changeDirectionBound(0, 1);
        m_Time = 0; //reset movement timer
    }
    else if (m_Time > 1000)
    { //movement timer went off, randomly shoot in another direction
        changeDirection(0, 0, true);
        m_Time = 0; //reset movement timer
    }

    // Saving current position
    m_Time += dt.asMilliseconds();
    m_PrevPosition = m_Position;

    // Move the spider
    m_Shape.setPosition(m_Position);
    m_Sprite.setPosition(m_Position);

    // Returning that the spider still exists
    return (m_Health != 0);
}
