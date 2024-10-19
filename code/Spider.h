/*
Author: Dawson Pent
Last Date Modified: 9/29/24

Description:
This is the header file which describes the spider class to:
 - Calculate current location and movement information for the spider
 - Detect collisions between objects to do damage / stop movement
*/

#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Spider
{
private:
    //creating necessary objects for the location/shape
    Vector2f m_Position;
    Vector2f m_PrevPosition;
    RectangleShape m_Shape;
    Texture m_SpiderImage;
    Sprite m_Sprite;
    RenderWindow *m_Window;

    //setting constant values for the object
    const float m_Width = 60;
    const float m_Height = 40;

    const int m_Damage = 3;

    const int m_PointsBase = -100;
    const int m_PointsPerSpeed = 100;

    //creating changing values for the object
    float m_Speed; //movement speed of the spider
    float m_DesiredX; //direction that the spider wants to go more towards
    float m_DirectionX; //unit vector direction for x movement (-1, -.707 0, .707, or 1)
    float m_DirectionY; //unit vector for y direction
    int m_Time = 0; //holds the time since the last movement to allow random, no-hit movement

    int m_Health = 0; //identifies whether the spider is alive/active or not
    int m_Points = 0; //variable depending on the speed
  
    //creating screen values
    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_MaxHeight;

    //private methods
    
    /**
     * Private function to create a unit movement vector in the direction specified.
     * 
     * Parameters:
     * @param X - this is the direction in x that you want the vector to point
     * @param Y - this is the direction in y that you want the vector to point
     */
    void createMovementUnitVector(float X, float Y);

public:
    /**
     * Constructor for the Spider class to create the spider object.
     * 
     * Parameters:
     * @param window - pointer to the window which is displaying the game to draw the spider
     */
    Spider(RenderWindow* window);


  
    // Location and shape methods
  
    /**
     * Function to simply return the bounds (position and shape) of the object.
     * 
     * Returns:
     * @param rectangle bounds of the object at the correct location and same size
     */
    FloatRect getPosition();

    /**
     * Function to simply return the shape of the object.
     * 
     * Returns:
     * @return rectangle shape of the object
     */
    RectangleShape getShape();

  

    // Specific methods

    /**
     * Function to randomly change the general direction of movement based on any objects hit (generally can go negative, 0, or positive in either x or y).
     * 
     * Parameters:
     * @param directX - this is the direction in the x direction that the collision came from (0 for no collision in this direction)
     * @param directY - this is the direction in the y direction that the collision came from (0 for no collision in this direction)
     * @param prevX - this it the previous movement direction in x to avoid going back the same exact way (x and y)
     * @param prevY - this is the previous movement direction in y to avoid going back the same exact way (x and y)
     * @param createVector - this is a boolean to identify whether you want to automatically create a unit vector based on these randomly generated values
     *
     * Returns:
     * @return the generated value in the y direction
     */
    float changeDirectionAbsolute(float directX, float directY, float prevX, float prevY, bool createVector);
    
    /**
     * Function to randomly change the general direction of movement away from an outer bound (wall, floor, or ceiling).
     * 
     * Parameters:
     * @param boundX - this is the direction in the x direction that the bound is relative to spider (0 for no bound in this direction)
     * @param boundY - this is the direction in the y direction that the bound is relative to spider (0 for no bound in this direction)
     * @param createVector - this is a boolean to identify whether you want to automatically create a unit vector based on these randomly generated values
     */
    void changeDirectionBound(float boundX, float boundY);
    
    /**
     * Function to randomly change the general direction of movement based on any objects hit, ignoring previous movement.
     * 
     * Parameters:
     * @param directX - this is the direction in the x direction that the collision came from (0 for no collision in this direction)
     * @param directY - this is the direction in the y direction that the collision came from (0 for no collision in this direction)
     * @param createVector - this is a boolean to identify whether you want to automatically create a unit vector based on these randomly generated values
     *
     * Returns:
     * @return the generated value in the y direction
     */
    float changeDirection(float directX, float directY, bool createVector);

    /**
     * Function to randomly spawn a new spider on left or right and start its movement and initialize its values without needing to create a new spider object.
     */
    void startMovement();

    /**
     * Function to simply get the health of the spider
     * 
     * Returns:
     * @return the current health value of the spider
     */
    int getHealth();

    /**
     * Function to simply get the number of points the spider is worth, variable to the speed:
     *   4 * 60 = 300 points
     *   5 * 60 = 500 points
     *   6 * 60 = 700 points
     *   7 * 60 = 900 points
     * 
     * Returns:
     * @return the amount of points the spider is worth
     */
    int getPoints();

    /**
     * Function to simply do a certain amount of damage to the spider
     * 
     * Parameters:
     * @param damage - the amount to reduce the spider's health by
     */
    void doDamage(int damage);


  
    // Collision and Update methods

    /**
     * Function to check collisions and do damage if there was a collision.
     *  Simply takes in an object bounds and sees if it intersects with the current object.
     *  If the object is hit, do the specified amount of damage.
     * 
     * Parameters:
     * @param other - bounds box of the other object to check for collision
     * @param damage - amount of damage to do to object upon collision
     *
     * Returns:
     * @return value of how much damage to do to the other object (-1 means no collision occured)
     */
    int checkDamage(FloatRect other, int damage);

    /**
     * Function to update the positions and values of the spider based on movment direction vector and timing.
     * 
     * Parameters:
     * @param dt - how long it has been since clock cycles to move the object accordingly far
     * @param shipX - what the ship's x position is so that the spider can generally be closer to the player
     *
     * Returns:
     * @return whether the spider is still alive or not
     */
    bool update(Time dt, float shipX);

    /**
     * Function to draw the spider at the current position values
     */
    void draw();
};
