/*
Author: Dawson Pent
Last Date Modified: 10/17/24

Description:
This is the main program which runs the entire centipede game program, bringing all classes together.
*/

#include "StarShip.h"
#include "InformationOverlay.h"
#include "Mushroom.h"
#include "Spider.h"
#include "Centipede.h"
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>

using namespace sf;

// Defining image constants for all mushroom parts
static Texture mushroomImage0;
static bool	mushroomImageLoad0;
static Texture mushroomImage1;
static bool	mushroomImageLoad1;
static Texture mushroomImage2;
static bool	mushroomImageLoad2;
static Texture mushroomImage3;
static bool	mushroomImageLoad3;
static std::vector<Mushroom> mushrooms;

/** 
 * This is a function that is only ran by the main method when the user hits enter upon opening up the game.  This begins the round by
 * initializing all important values for the match including the mushrooms (which stay consistent after dying), the score, and the lives.
 * 
 * Parameters:
 * @param window - this is a pointer to the game's window object to give to the game object classes for drawing
 * @param gradient - this is the background gradient image for when the game is being played
 * @param winWidth - this is the width of the window to give to classes (no need to calculate again)
 * @param winHeight - this is the height of the window to give to classes (no need to calculate again)
 * @param score - this is the pointer to the main's score integer so it can display the final score on the main screen after death
 */
void beginMatch(RenderWindow* window, VertexArray gradient, float winWidth, float winHeight, int* mainScore);

/** 
 * This is a function ran for every player life during the round.  Restarts by creating the centipede randomly and creating the spider eventually at
 * random.  This is the overall general game loop which updates entities, reacts to collisions, and keeps the game timing functioning.
 * 
 * Parameters:
 * @param RenderWindow* window - this is a pointer to the game's window object to give to the game object classes for drawing
 * @param VertexArray gradient - this is the background gradient image for when the game is being played
 * @param float winWidth - this is the width of the window to give to classes (no need to calculate again)
 * @param float winHeight - this is the height of the window to give to classes (no need to calculate again)
 * @param int* score - this is the pointer to the main's score integer so it can display the final score on the main screen after death
 * @param int health - this is basically a counter to how much longer to keep the round going (until the ship has lost all lives with 0 health)
 */
void nextRound(RenderWindow* window, VertexArray gradient, float winWidth, float winHeight, int* score, int health);

/** 
 * Main function to run the overall window object.  Waits for the user to start the game with enter and then runs a new round.
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

	//creating main screen background
	Texture mainBackgroundImage;
	mainBackgroundImage.loadFromFile("graphics/StartupScreen.png");
	Sprite mainBackground;
	mainBackground.setTexture(mainBackgroundImage);
	//setting image characteristics
	mainBackground.setPosition(0,0);
	float scaleX = winWidth  / mainBackgroundImage.getSize().x;
	float scaleY = winHeight / mainBackgroundImage.getSize().y;
	mainBackground.setScale(scaleX, scaleY);


	// Initializing the 4 texturesx
	mushroomImageLoad0 = mushroomImage0.loadFromFile("graphics/Mushroom0.png");
	mushroomImageLoad1 = mushroomImage1.loadFromFile("graphics/Mushroom1.png");
	mushroomImageLoad2 = mushroomImage2.loadFromFile("graphics/Mushroom2.png");
	mushroomImageLoad3 = mushroomImage3.loadFromFile("graphics/Mushroom3.png");


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





/** 
 * Function to create a new mushroom at the relative (could be random between 0 and 1) x and y coordinates
 *
 * Parameters:
 * @param relativeX - the relative (percentage) x position of desired new mushroom
 * @param relativeY - the relative (percentage) y position of desired new mushroom
 * @param window - pointer to the window for the mushroom class to be able to draw the mushroom
 *
 * Returns:
 * @return whether adding the mushroom was successful or not
 */
bool createNewShroom(float relativeX, float relativeY, RenderWindow* window)
{
	//calculating the new position of the mushroom while keeping them in a grid
	float positionX = relativeX * (window->getSize().x - 130) + 65;
	positionX = ((int) (positionX / 35.0f)) * 35.0f;
	float positionY = relativeY * (window->getSize().y - 265) + 130;
	positionY = ((int) (positionY / 35.0f)) * 35.0f;
	
	//creating the new Mushroom object
	Mushroom newShroom(positionX, positionY, &mushroomImage3, mushroomImageLoad3, &mushroomImage2,
		     		   mushroomImageLoad2, &mushroomImage1, mushroomImageLoad1, &mushroomImage0, mushroomImageLoad0, window);
	
	//ensuring they aren't overlapping with anyone in the mushrooms array
	bool noCollision = true;
	for (int j = 0; j < (int) mushrooms.size(); ++j)
	{
		//ensuring they aren't intersecting
		if (mushrooms[j].checkDamage(newShroom.getPosition(), 0) != -1)
		{
			//they intersected, mushroom creation unsuccessful
			return false;
		}
	}
	
	//no other mushroom was in the same location, place the mushroom
	if (noCollision)
    {
		mushrooms.push_back(newShroom);
    }
	
	//return that it was successful
	return true;
}

/**
 * Function to create a new mushroom at the exact x and y coordinates (but on the grid)
 *
 * Parameters:
 * @param exactX - the x position of desired new mushroom
 * @param exactY - the y position of desired new mushroom
 * @param window - pointer to the window for the mushroom class to be able to draw the mushroom
 *
 * Returns:
 * @return whether adding the mushroom was successful or not
 */
bool createNewShroomExact(float exactX, float exactY, RenderWindow* window)
{
	//force the position to be on the grid
	float positionX = floor(exactX) - ((int) exactX % 35);
	float positionY = floor(exactY) - ((int) exactY % 35);
	
	//creating the new Mushroom object
	Mushroom newShroom(positionX, positionY, &mushroomImage3, mushroomImageLoad3, &mushroomImage2,
		     		   mushroomImageLoad2, &mushroomImage1, mushroomImageLoad1, &mushroomImage0, mushroomImageLoad0, window);
	
	//ensuring they aren't overlapping with anyone in the mushrooms array
	bool noCollision = true;
	for (int j = 0; j < (int) mushrooms.size(); ++j)
	{
		//ensuring they aren't intersecting
		if (mushrooms[j].checkDamage(newShroom.getPosition(), 0) != -1)
		{
			//they intersected, mushroom creation unsuccessful
			return false;
		}
	}
	
	//no other mushroom was in the same location, place the mushroom
	if (noCollision)
    {
		mushrooms.push_back(newShroom);
    }
	
	//return that it was successful
	return true;
}







void beginMatch(RenderWindow* window, VertexArray gradient, float winWidth, float winHeight, int* score)
{
	//intitializing score to 0
	*score = 0;
	
	//empty whatever mushrooms are left in the vector
	mushrooms.clear();

	//initializing all random mushrooms
	for (int i = 0; i < 60; ++i)
	{
		//getting relative random positions (0f to 1f)
		float relativeX = ( (float) std::rand() ) / RAND_MAX;
		float relativeY = ( (float) std::rand() ) / RAND_MAX;

		//creating the mushroom object
		if (!createNewShroom(relativeX, relativeY, window))
		  {
		    //creation of the mushroom was unsuccessful (mushroom collision)
		    i--;
		    continue;
		  }
	}

	//starting the first round of the match
	nextRound(window, gradient, winWidth, winHeight, score, 3);
}








void nextRound(RenderWindow* window, VertexArray gradient, float winWidth, float winHeight, int* score, int health)
{
	//beginning the next round
	int tick = 0; //initializing tick as 0
	
	//create the starship
	StarShip ship(winWidth/2, winHeight-50, window);
	ship.setHealth(health);

	//create the overlay
	Overlay overlay(ship.getHealth(), (int) winWidth, window);

	//create the spider
	Spider spider(window);

	//create the centipede
	int numSegments = 15; //initializing with 15 segments
	std::vector<Centipede> centipede; //vector to hold all centipede body parts
	float headRandom; //temporary float to identify which random direction the head moves in originally
	//creating the head segment
	Centipede firstSegment(window, HEAD, 0, &headRandom);
	centipede.push_back(firstSegment);
	for (int i = 1; i < numSegments - 1; ++i)
	{
		//creating the body segments
		Centipede newSegment1(window, BODY, i, &headRandom);
        centipede.push_back(newSegment1);
	}
	//creating the tail segment
	Centipede lastSegment(window, TAIL, 14, &headRandom);
	centipede.push_back(lastSegment);

	//declaring the clock for the timer of the game
	Clock clock;

	//loop to run the game while the window is open and the round is running
	while (window -> isOpen())
	{
		// --- Checking player inputs --- //
		//checking if the window was closed
		Event event;
		while (window -> pollEvent(event))
		{
			//if the window event is closing the window, make sure the window gets closed
			if (event.type == Event::Closed)
			{
				window -> close();
			}
		}
		//checking if the player hit the escape key to quit the game
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window -> close();
		}

		//checking if the player pressed any arrow keys
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			//key left is pressed, so move
			ship.moveLeft(true);
		}
		else
		{	//key left isn't pressed
			ship.moveLeft(false);
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			//key right is pressed, so move
			ship.moveRight(true);
		}
		else
		{
			//key right isn't pressed
			ship.moveRight(false);
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			//key up is pressed, so move
			ship.moveUp(true);
		}
		else
		{
			//key up isn't pressed
			ship.moveUp(false);
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			//key down is pressed, so move
			ship.moveDown(true);
		}
		else
		{
			//key down isn't pressed
			ship.moveDown(false);
		}

		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			//key space is pressed, so shoot
			ship.shoot(true);
		}
		else
		{
			//key space isn't pressed
			ship.shoot(false);
		}



		// --- Updating Objects --- //
		//update delta time of the loop
		Time dt = clock.restart();

		//updating the ship
		ship.update(dt);

		//updating the spider
		spider.update(dt, ship.getShape().getPosition().x);

		//updating the centipede movement
		//updating the first head
		Vector2f prevPosition = centipede[0].update(dt, Vector2f(0,0));
		for (int i = 1; i < numSegments; ++i)
		{
			//updating each segment after based on the segment before (unless it is a new head)
			prevPosition = centipede[i].update(dt, prevPosition);
		}

		//every 100 ticks, random chance of spawning a spider if not already spawned
		if (tick%100 == 0 && spider.getHealth() <= 0)
		{
			//random value from 0 to 1
		    float random = ( (float) std::rand() ) / RAND_MAX;
			//if random value is greater than .8, spawn a new spider
		    if (random > .8f)
		    {
				//spawning a new spider
				spider.startMovement();
		    }
		}


		// Check for collisions

		//first check for mushroom collisions
		for (int m = 0; m < (int) mushrooms.size(); ++m)
		{
			//check for any collisions with the ship if low enough
			if (mushrooms[m].getShape().getPosition().y >= 720) {
				//getting if this mushroom collided with the ship
				int collide = mushrooms[m].checkDamage(ship.getPosition(), 0);
				if (collide != -1)
				{
					//ship collided, don't do damage, just undo movement
					#ifdef DEBUG
					printf("collision check %d:  S(%f, %f) || M(%f, %f)\n", collide, ship.getPosition().getPosition().x,
						ship.getPosition().getPosition().y, current.getPosition().getPosition().x, current.getPosition().getPosition().y);
					#endif
					ship.undoMove();
				}
			}


			//only need to check for ship collisions every time, otherwise only check for other collisions periodically
			if (tick%2 == 0)
			{
				//check for any collisions with the lasers
				Laser* shots = ship.getLaserArray();
				for (int i = 0; i < 30; ++i)
				{
					//ensuring shot x value is close to mushroom before checking collision
					if (abs(shots[i].getShape().getPosition().x - mushrooms[m].getShape().getPosition().x) > 30)
					{
						//shot isn't close, don't check
						continue;
					}
					else if (shots[i].getHealth() <= 0) {
						//shot is dead, don't check
						continue;
					}

					//checking for collisions between shots and mushroom
					int collide = shots[i].checkDamage(mushrooms[m].getPosition());
					if (collide != -1)
					{
						//bullet collided, hurt mushroom
						#ifdef DEBUG
						printf("collision check %d:  L(%f, %f) || M(%f, %f)\n", collide, shots[i].getPosition().getPosition().x,
							shots[i].getPosition().getPosition().y, mushrooms[m].getPosition().getPosition().x, mushrooms[m].getPosition().getPosition().y);
						#endif
						mushrooms[m].doDamage(collide);
						if (mushrooms[m].getHealth() <= 0)
						{
							//mushroom has been destroyed by the player, increment score
						    *score += mushrooms[m].getPoints();
						}
						break;
					}
				}

				//checking for any collisions with the spider
				int collide = spider.checkDamage(mushrooms[m].getPosition(), 0);
				if (collide != -1)
				{
					//spider collided, already bounced, just slightly damage mushroom
					mushrooms[m].doDamage(collide);
				}

				//checking for any collisions with the centipede heads
				for (int i = 0; i < numSegments; ++i)
				{
				    centipede[i].checkDamage(mushrooms[m].getPosition(), 0);
				}
			}


		    //ensure mushroom is still alive
		    if (mushrooms[m].getHealth() <= 0)
			{ 
				//mushroom has been destroyed, erase it and decrement m because of that
			    mushrooms.erase(mushrooms.begin() + m--);
			}
		}


		//check for other laser collisions with the spider or centipede
		Laser* shots = ship.getLaserArray();
		for (int i = 0; i < 30; ++i)
		{
		    // First check for spider collision
			//ensuring shot x value is close to spider before checking collision
		    if (spider.getHealth() > 0 && abs(shots[i].getShape().getPosition().x - spider.getShape().getPosition().x) <= 90)
			{
				//shot is close, check for a collision
			    int collide = shots[i].checkDamage(spider.getPosition());
			    if (collide != -1)
			    {
				    //bullet collided, kill the spider and increment score
				    spider.doDamage(collide);
				    *score += spider.getPoints();
				    continue;
			    }
			}


			// Now, check for centipede collision
			for (int j = 0; j < numSegments; ++j)
			{
			    //ensuring shot x value is close to segment before checking collision
			    if (abs(shots[i].getShape().getPosition().x - centipede[j].getShape().getPosition().x) <= 40)
			    {
					//shot is close, check for a collision
					int collide = shots[i].checkDamage(centipede[j].getPosition());
					if (collide != -1)
				  	{
				    	//bullet collided, kill the centipede and increment score
				    	*score += centipede[j].getPoints();
						
						/* 	Based on the centipede segment type, change the centipede segments around it
							a) If the type is head, simply set the next segment as the new head
							b) If the type is tail, simply set the previous segment as the new tail
							c) If the type is body, you create a new head and a new tail
						*/

						
				    	if (centipede[j].getType() == HEAD)
						{ //current segment is a head, so set the next segment as the new head
							//ensuring this isn't the end of the list and the next segment isn't already a head
							if (j+1 < numSegments && centipede[j+1].getType() != HEAD)
					  		{
					    		centipede[j+1].setType(HEAD);
					  		}
				      	}
				    	else if (centipede[j].getType() == TAIL)
				      	{ //simply set the previous segment as the tail now
							//ensuring the previous segment wasn't a head
							if (centipede[j-1].getType() != HEAD)
					  		{
					    		centipede[j-1].setType(TAIL);
					  		}
						}
				    	else
				      	{ //need to split off into two centipedes now
							//ensuring we aren't at the end to create the new centipede head
							if (j+1 < numSegments && centipede[j+1].getType() != HEAD)
					  		{
					    		centipede[j+1].setType(HEAD);
					  		}
							//ensuring the previous segment isn't a head in order to set it to a tail
							if (centipede[j-1].getType() != HEAD)
					  		{
					    		centipede[j-1].setType(TAIL);
					  		}
				      	}

						//put a mushroom in the destroyed centipede segments location
						createNewShroomExact(centipede[j].getShape().getPosition().x,
											 centipede[j].getShape().getPosition().y, 
											 window);
						
						//erase the centipede segment
						centipede.erase(centipede.begin() + j--);
						numSegments--;
			        }
			    }
			}
		}


	    // Next, check for player collisions
		if (spider.getHealth() > 0 && ship.checkDamage(spider.getPosition()) != -1)
		{ //checking if spider has collided with the player starship
	        //spider has collided, kill spider and go to next round
		    spider.doDamage(1);
			break;
		}
		
		// Lastly, check for centipede collision
		bool collision = false;
		for (int j = 0; j < numSegments; ++j)
		{
		    //ensuring centipede x is close to the player before checking collision
		    if (abs(ship.getShape().getPosition().x - centipede[j].getShape().getPosition().x) <= 40)
		    {
				//centipede is close, check for a collision
				int collide = ship.checkDamage(centipede[j].getPosition());
				if (collide != -1)
			  	{
			    	//centipede collided, decrement health and break from loop
			    	collision = true;
		        }
		    }
		}
		//checking if any centipede segments ended up colliding with the player
		if (collision)
		{ //collided with the player, restart the round
			break;
		}


		
		// --- Drawing the screen -- //
		//clearing the window
		window -> clear();

		//drawing the window background as the gradient
		window -> draw(gradient);
		
		//drawing the ship and shots
        ship.draw();
		
		//drawing the score overlay
		overlay.draw(*score);

		//drawing all of the mushrooms
		for (int m = 0; m < (int) mushrooms.size(); ++m)
		{
			mushrooms[m].draw(dt);
		}

		//drawing the spider
		spider.draw();

		//drawing all of the centipede segments
		for (int i = 0; i < numSegments; ++i)
		{
			centipede[i].draw();
		}

		//displaying the drawn parts onto the window
		window -> display();

		//increment tick to 1000 before resetting it to 0
		tick = (tick >= 1000) ? 0 : tick + 1;
	}

    // Checking if the match is over based on player health remaining
	//if the ship isn't dead and the window is open, go to the next round of the match
	if (ship.getHealth() > 0 && window -> isOpen())
	{
	        // not dead yet, start the next round
			#ifdef DEBUG
	        printf("Restarting loop\n");
			#endif
			
			//starting the next round
	        nextRound(window, gradient, winWidth, winHeight, score, --health);
	}
}
