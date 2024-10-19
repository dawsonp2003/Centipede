/*
Author: Dawson Pent
Last Date Modified: 9/25/24

Description:
This is the header file which describes the starship class to:
 - Calculate current location and movement information based on
   key inputs gathered in game to move the starship.
 - Detect collisions between objects to kill the starship
 - Store the starship values of health (lives left)
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "LaserBlast.h"

using namespace sf;

class StarShip
{
private:
    //creating necessary objects for the location/shape
	Vector2f m_Position;	
    Vector2f m_PrevPosition;
	RectangleShape m_Shape;
    Texture m_ShipImage;
    Sprite m_Sprite;
    RenderWindow *m_Window;

    //setting constant values for the object
	const float m_Speed = 600.0f;
    const int m_Damage = 7;

    const float m_Width = 20;
    const float m_Height = 30;

    const static int m_ShotNum = 30;
    const float m_ShootDelay = 0.1f;

    //creating changing values for the object
    Laser shots[m_ShotNum];
    int m_CurrentShot = 0;
    bool m_Shooting = false;
    float m_CurrentShootDelay = 0;

    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_MaxHeight;

    int m_Left = 0;
    int m_Right = 0;
    int m_Up = 0;
    int m_Down = 0;

    int m_Health = 3; //identifies when the laser is deleted

public:
    /**
     * Constructor for the Star Ship class
     * 
     * Parameters:
     * @param startX - this is a float which identifies the starting x position of the ship
     * @param startY - this is a float which identifies the starting y position of the ship
     * @param window - this is a pointer to the game's window object to draw the star ship and lasers
     */
    StarShip(float startX, float startY, RenderWindow* window);


  
    // Location and shape methods

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


  
    // Movement methods

    /**
     * Function to set the left movement boolean of the ship (tells the ship to move left based on user input)
     * 
     * Parameters:
     * @param active - this is the boolean of whether to go left or not.
     */
    void moveLeft(bool active);

    /**
     * Function to set the right movement boolean of the ship (tells the ship to move right based on user input)
     * 
     * Parameters:
     * @param active - this is the boolean of whether to go right or not.
     */
    void moveRight(bool active);

    /**
     * Function to set the up movement boolean of the ship (tells the ship to move up based on user input)
     * 
     * Parameters:
     * @param active - this is the boolean of whether to go up or not.
     */
    void moveUp(bool active);

    /**
     * Function to set the down movement boolean of the ship (tells the ship to move down based on user input)
     * 
     * Parameters:
     * @param active - this is the boolean of whether to go down or not.
     */
    void moveDown(bool active);

    /* Function to undo a movement and put the ship back at the previous position]
     */
    void undoMove();


  
    // Specific methods
  
    /**
     * Function to set if the ship is shooting lasers (tells the ship to shoot down based on user input)
     * 
     * Parameters:
     * @param active - this is the boolean of whether to shoot or not.
     */
    void shoot(bool active);

    /**
     * Function to get the health of the ship currently
     * 
     * Returns:
     * @return current integer value of the health of the ship
     */
    int getHealth();
  
    /**
     * Function to set the new health of the ship
     * 
     * Parameters:
     * @param health - new value to set the health to
     */
    void setHealth(int health);

    /**
     * Function to get the pointer to the ship's laser array for hit detection.
     * 
     * Returns:
     * @return pointer to the ship's laser array
     */
    Laser* getLaserArray();

    /**
     * Function to draw the ship and its lasers at the current position values
     */
    void draw();


  
    // Collision and Update methods

    /**
     * Function to check collisions and do damage if there was a collision.
     *  Simply takes in an object bounds and sees if it intersects with the current ship to see if the ship needs to stop or lose a health.
     * 
     * Parameters:
     * @param other - bounds box of the other object to check for collision
     *
     * Returns:
     * @return value of how much damage to do to the other object (-1 means no collision occured)
     */
    int checkDamage(FloatRect other);

    /**
     * Function to update the positions and values of the ship including the lasers in the laser array based on user input and timing.
     * 
     * Parameters:
     * @param dt - how long it has been since clock cycles to move the object accordingly far
     *
     * Returns:
     * @return whether the ship is still alive or not
     */
    bool update(Time dt);
};
