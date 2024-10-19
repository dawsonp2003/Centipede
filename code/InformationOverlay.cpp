/*
Author: Dawson Pent
Last Date Modified: 9/24/24

Description:
This is the program file which implements the overlay class to:
 - Display overlay information while the game is running including:
    > score (with a simple number)
    > lives (via displaying a starship per starship health)
*/

#include "InformationOverlay.h"

Overlay::Overlay(int health, int windowWidth, RenderWindow *window)
{
    // Simply initialize the values
    //setting basic values
    m_Health = health;
    m_Score = 0;
    m_WindowWidth = windowWidth;
    //setting up window
    m_Window = window;
    //setting up score display
    m_Font.loadFromFile("fonts/PressStart.ttf");
    m_ScoreText.setFont(m_Font);
    m_ScoreText.setPosition(sf::Vector2f(((float) windowWidth) / 2.0f, 50.0f));
    m_ScoreText.setString("Hello World");
    m_ScoreText.setFillColor(Color::White);
    //setting up ship image to display lives left
    m_ShipImage.loadFromFile("graphics/StarShip.png");
    m_ShipSprite.setTexture(m_ShipImage);
}

void Overlay::setHealth(int health)
{
    //setting the health to the given value
    m_Health = health;
}

void Overlay::draw(int score)
{
    //draws the score and health values on the in-game screen
    m_ScoreText.setString(std::to_string(score));
    m_Window -> draw(m_ScoreText);
    for (int i = 0; i < m_Health; ++i) {
        // Sprite sprite;
        m_ShipSprite.setPosition(sf::Vector2f(((float) m_WindowWidth) / 1.5f + 50.0f * i, 50.0f));
        m_Window -> draw(m_ShipSprite);
    }
}
