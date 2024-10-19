/*
Author: Dawson Pent
Last Date Modified: 9/30/24

Description:
This is the header file which describes the centipede class to:
 - Calculate current location and movement information for this specific segment of the centipede
 - Detect collisions between objects to do damage / change movement
 - Hold the type of centipede body part (head, end, middle)
*/

#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

    // Public global types defined for class
    enum BodyType {
	    HEAD = 0,
	    BODY = 1,
	    TAIL = 2
	};

class Centipede
{
private:
    // Creating necessary objects for the location/shape
    Vector2f m_Position;
    Vector2f m_PrevPosition;
    Vector2f m_PrevMove;
    Vector2f m_Offset;
    RectangleShape m_Shape;
    Texture m_HeadImage;
    Texture m_BodyImage;
    Sprite m_HeadSprite;
    Sprite m_BodySprite;
    RenderWindow *m_Window;

    // Setting values for the object
    float m_Width = 35;
    float m_Height = 35;

    float m_Speed = 8 * 80.0f;
    bool m_Bounce = false;

    int m_Damage = 1;

    int m_DirectionX; //identifies left vs right movement
    int m_DirectionY; //only used when going up/down to orient head
    float m_Distance; //used to ensure centipede is only moved 35 up/down

    int m_Points = 0; //variable depending on head or body //10 points per body, 100 per head

    BodyType m_Type; //identifies the type of this part of the centipede.
  
    //creating screen values
    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_MaxHeight;
    
    /**
     * Private function to update the positions and values of any centipede heads based on movment direction and timing.
     * 
     * Parameters:
     * @param dt - how long it has been since clock cycles to move the object accordingly far
     *
     * Returns:
     * @return what the change in position is for the object
     */
    Vector2f update(Time dt);

public:
    /**
     * Constructor for the Centipede class to create the current centipede segment object.
     * 
     * Parameters:
     * @param window - pointer to the window which is displaying the game to draw the centipede segment
     * @param type - the type of the current centipede object (HEAD, BODY, or TAIL)
     * @param numSegment - the segment number of the current segment to ensure it is evently spaced
     * @param headRandom - pointer to a float to write to if the current segment is the head to have all segments go in the same random direction at first
     */
    Centipede(RenderWindow* window, BodyType type, int numSegment, float* headRandom);


  
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

  

    // Specific methods
	
    /**
     * Function to simply get the number of points the centipede segment is worth, variable to the type:
     *   HEAD = 100 points
     *   BODY/TAIL = 10 points
     * 
     * Returns:
     * @return the amount of points the centipede is worth
     */
    int getPoints();

    /**
     * Function to simply get the current type of the segment.
     * 
     * Returns:
     * @return the current BodyType of the centipede segment
     */
    BodyType getType();
    
    /**
     * Function to simply set the type of the segment.
     * 
     * Parameters:
     * @param type - the new BodyType of the centipede segment
     */
    void setType(BodyType type);


  
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
     * Function to update the positions and values of the centipede segment based on movment direction timing.
     *  This is the public function which specifies a vector to hold what the previous movement was made by the last segment.
     *  Allows you to not have to recalculate the movement and just base it off of the previous segment.
     * 
     * Parameters:
     * @param dt - how long it has been since clock cycles to move the object accordingly far
     * @param aheadPosition - movement vector of the previous segment
     *
     * Returns:
     * @return previous movement vector of this segment
     */
    Vector2f update(Time dt, Vector2f aheadPosition);

    /**
     * Function to draw the segment based on the type and orientation at the current position values
     */
    void draw();
};
