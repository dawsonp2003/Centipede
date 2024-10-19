/*
Author: Dawson Pent
Last Date Modified: 9/24/24

Description:
This is the header file which describes the class to:
 - Display overlay information while the game is running including:
    > score (with a simple number)
    > lives (via displaying a starship per starship health)
*/ 

#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Overlay
{
private:
    //creating necessary objects for the method
    RenderWindow* m_Window;
    Texture m_ShipImage;
    Sprite m_ShipSprite;
    RectangleShape m_ShipShape;
    Text m_ScoreText;
    Font m_Font;

    //setting constant values for the object
    int m_WindowWidth; //holds the width of the window

    //creating changing values for the object
    int m_Health; //holds the current health of the player
    int m_Score; //holds the current score of the round

public:
    /** 
     * Constructor for the Overlay class to create the overlay object.
     * 
     * Parameters:
     * @param health - the health of the current round to identify how many ships to draw
     * @param windowWidth - integer screen width to help calculate where to put the text on the screen
     * @param window - pointer to the window which is displaying the game to draw the overlay
     */
    Overlay(int health, int windowWidth, RenderWindow *window);

    // Specific methods

    /** 
     * Function to simply set the health to display on the overlay
     * 
     * Parameters:
     * @param health - the current health value of the ship to set the overlay to
     */
    void setHealth(int health);

    // Drawing method
  
    /** 
     * Function to draw the overlay with the ships per health and score for the game.
     * 
     * Parameters:
     * @param score - what score value to draw on the overlay
     */
    void draw(int score);
};
