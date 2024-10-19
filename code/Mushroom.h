/*
Author: Dawson Pent
Last Date Modified: 9/26/24

Description:
This is the header file which describes the mushroom class to:
 - Hold the basic value for a generated mushroom
 - Detect collisions between objects to take damage / stop movement
*/

#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Mushroom
{
private:
    //creating necessary objects for the location/shape
    Vector2f m_Position;
    RectangleShape m_Shape;
    Sprite m_Sprite3;
    Sprite m_Sprite2;
    Sprite m_Sprite1;
    Sprite m_Sprite0;
    RenderWindow *m_Window;

    //setting constant values for the object
    float m_Width = 30;
    float m_Height = 30;

    int m_Points = 4;

    //creating changing values for the object
    bool m_SpriteLoad3;
    bool m_SpriteLoad2;
    bool m_SpriteLoad1;
    bool m_SpriteLoad0;

    float  m_HealthTimer = 0; //delays damage between hits
    int m_Health = 12; //identifies which stage the mushroom is in for display

public:
    /** 
     * Constructor for the Mushroom class to create the mushroom object.
     * 
     * Parameters:
     * @param X - the x position of the mushroom
     * @param Y - the y position of the mushroom
     * @param mushroomImage3 - a pointer to the generally defined mushroom image 3 texture (3 is lowest health)
     * @param spriteLoad3 - a boolean to identify whether image 3 was loaded or not
     * @param mushroomImage2 - a pointer to the generally defined mushroom image 2 texture (2 is second lowest health)
     * @param spriteLoad2 - a boolean to identify whether image 2 was loaded or not
     * @param mushroomImage1 - a pointer to the generally defined mushroom image 1 texture (1 is okay health)
     * @param spriteLoad1 - a boolean to identify whether image 1 was loaded or not
     * @param mushroomImage0 - a pointer to the generally defined mushroom image 0 texture (0 is highest health)
     * @param spriteLoad0 - a boolean to identify whether image 0 was loaded or not
     * @param window - pointer to the window which is displaying the game to draw the mushroom
     */
    Mushroom(float X, float Y, Texture* mushroomImage3, bool spriteLoad3, Texture* mushroomImage2, bool spriteLoad2,
             Texture* mushroomImage1, bool spriteLoad1, Texture* mushroomImage0, bool spriteLoad0, RenderWindow* window);


  
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

    /* Function to simply get the health of the mushroom
     * 
     * Returns:
     * int - the current health value of the mushroom
     */
    int getHealth();

    /* Function to simply get the number of points the mushroom is worth
     * 
     * Returns:
     * int - the amount of points the mushroom is worth
     */
    int getPoints();

    /* Function to simply do a certain amount of damage to the mushroom
     * 
     * Parameters:
     * int damage - the amount to reduce the mushroom's health by
     */
    void doDamage(int damage);

    // Collision and Update methods

    /* Function to draw and update the mushroom at its position and using the correct image based on
     *  its current health value.  Only takes damage a couple times a second.
     *
     * Parameters:
     * Time dt - the time since the last cycle
     */
    void draw(Time dt);

    /* Function to check collisions and do damage if there was a collision.
     *  Simply takes in an object bounds and sees if it intersects with the current object.
     *  If the object is hit, do the specified amount of damage.
     * 
     * Parameters:
     * FloatRect other - bounds box of the other object to check for collision
     * int damage - amount of damage to do to object upon collision
     *
     * Returns:
     * int - value of how much damage to do to the other object (-1 means no collision occured)
     */
    int checkDamage(FloatRect other, int damage);
};
