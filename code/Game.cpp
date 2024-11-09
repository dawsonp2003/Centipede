/*
Author: Dawson Pent
Last Date Modified: 11/9/24

Description:
This is a simple program to take in socket TCP input from multiple sources and then to move a robot based on it.
*/

#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>

using namespace sf;

/** 
 * Main function to run the overall window object.
 */
int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game
	//open up the game window
	RenderWindow window(vm, "Centipede", Style::Default);

	//creating the random seed
	std::srand(static_cast<unsigned int>(std::time(nullptr))); //use current time as the seed for the random generator

	//set the background as a gradient
	// basically gradients are created using vertex arrays which take an array of multiple positions and then 
	// create a shape in between the points filled with a specified color. If you change the color of each point,
	// you can create a gradient.

	//creating the array to hold the vertices
	VertexArray gradient(Quads, 4); //quads is the type to create gradients

	//getting screen values
	float winWidth = (float) window.getSize().x;
	float winHeight = (float) window.getSize().y;

	//defining the colors for the gradient
	Color gradColorTopLeft(20, 20, 40);
	Color gradColorBottomRight(5, 5, 20);

	//setting up the vertex positions for all 4 vertices: the 4 corners of the screen
	gradient[0].position = Vector2f(0.0f, 0.0f); //top left
	gradient[1].position = Vector2f(0.0f, winHeight); //bottom left
	gradient[2].position = Vector2f(winWidth, winHeight); //bottom right
	gradient[3].position = Vector2f(winWidth, 0.0f); //top right

	//create the color transitions for all 4 points (vertices)
	for (int i = 0; i < (int) gradient.getVertexCount(); ++i)
	{
		//change the color of the vector area depending on how far the vector is along the window
		float colorStrength = 0.8f * (gradient[i].position.x / winWidth) + 0.4f * (gradient[i].position.y / winHeight);

		//setting the vector's color based on the color strength formula
		gradient[i].color = Color
		(
			(int) ((1.0f-colorStrength)*((float) gradColorTopLeft.r) + colorStrength*((float) gradColorBottomRight.r)),
			(int) ((1.0f-colorStrength)*((float) gradColorTopLeft.g) + colorStrength*((float) gradColorBottomRight.g)),
			(int) ((1.0f-colorStrength)*((float) gradColorTopLeft.b) + colorStrength*((float) gradColorBottomRight.b))
		);
	}

	// Initializing everything for main screen
	//initializing basic values
	int mainScore = 0;

	//initializing clock for program
	Clock clock;

	//creating text to display previous score on the main screen
	//creating text objects
	Text prevScore;
	Font font;
	//initializing font and text objects
	font.loadFromFile("fonts/PressStart.ttf");
	prevScore.setFont(font);
	prevScore.setCharacterSize(30);
	prevScore.setFillColor(Color::White);
	prevScore.setStyle(Text::Bold);
	prevScore.setString("Play to Hold a Score...");
	//putting the text in a visible position
	prevScore.setOrigin(prevScore.getOrigin() + Vector2f(prevScore.getGlobalBounds().width / 2.f, prevScore.getGlobalBounds().height / 2.f));
	prevScore.setPosition(window.getSize().x/2 + .1f*window.getSize().x, 100);

	// Main loop to wait for input to start a round
	while (window.isOpen())
	{
		// Checking player inputs
		//checking if the window was closed
		Event event;
		while (window.pollEvent(event))
		{
			//if the window event is closing the window, make sure the window gets closed
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		//checking if the player hit the escape key to quit the game
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//checking if the player pressed enter to start a match
		if (Keyboard::isKeyPressed(Keyboard::Enter))
		{
		  //starting the match on the enter press
		  #ifdef DEBUG
		  printf("Start Match\n");
		  #endif
		  
		  //clearing the screen and beginning the match (where it creates the mushrooms and such)
		  window.clear();
		  beginMatch(&window, gradient, winWidth, winHeight, &mainScore);

		  #ifdef DEBUG
		  printf("End Match\n");
		  #endif

		  //setting the previous score string to be the main score from the last match
		  prevScore.setString(std::to_string(mainScore));
		  prevScore.setPosition(window.getSize().x/2 + .1f*window.getSize().x, 100);
		}


		
		// Drawing the window objects
		//first clear the window
		window.clear();
		//drawing the window picture background
		window.draw(mainBackground);
		//drawing the previous score text
		window.draw(prevScore);
		
		//displaying all drawn objects on the screen
		window.display();
	}

	//return that the game ran successfully
	return 0;
}