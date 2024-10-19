/*
Author: Dawson Pent
Last Date Modified: 9/25/24

Description:
This is the header file which describes the class to:
 - Calculate current location and movement information for the laser blast
 - Detect collisions between objects to decrease their health
 - Be scaleable to many laser blasts
*/ 

#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Laser
{
private:
    //creating necessary objects for the location/shape
	Vector2f m_Position;	
	RectangleShape m_Shape;

    //setting constant values for the object
	const float m_Speed = 1200.0f;
    const int m_Damage = 7;

    const float m_Width = 6;
    const float m_Height = 16;

    //creating changing values for the object
    int m_Health = 0; //identifies when the laser is deleted

public:
    /** 
     * Constructor for the Laser class to create the laser object.
     */
    Laser();

    // Location and Shape methods

    /** 
     * Function to simply return the bounds (position and shape) of the object.
     * 
     * Returns:
     * @return rectangle bounds of the object at the correct location and same size
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
     * Function to start the laser with its movement.  Allows you to not have to create new objects for every shot.
     * 
     * Parameters:
     * @param startX - what x value to start the laser from
     * @param startY - what y value to start the laser from
     */
    void startLaser(float startX, float startY);

    /** 
     * Function to simply see if the laser is still alive (non 1 is dead)
     * 
     * Returns:
     * @return the current health value of the laser
     */
    int getHealth();

    // Collision and Update methods

    /** 
     * Function to check collisions and do damage if there was a collision.
     *  Simply takes in an object bounds and sees if it intersects with the current object.
     *  If the object is hit, kill the laser.
     * 
     * Parameters:
     * @param other - bounds box of the other object to check for collision
     *
     * Returns:
     * @return value of how much damage to do to the other object (-1 means no collision occured)
     */
    int checkDamage(FloatRect other);

    /** 
     * Function to update the positions and values of the laser based on timing.
     * 
     * Parameters:
     * @param dt - how long it has been since clock cycles to move the object accordingly far
     *
     * Returns:
     * @return whether the laser is still alive or not
     */
    bool update(Time dt);
};
