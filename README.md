# IS1500-Snake-Project

## Objective and Requirements

The objective of this project is to develop the game called "Snake". The snake eats square rats, and each time it eats square rats it grows. If it collides with itself or one of the walls, it dies and the game restarts. The player needs to make the snake grow as much as possible. Each time the snake eats square rats, the player is awarded points. These are the main requirements of the game:


- Display the game on the built-in OLED graphical display.
- The snake moves across the screen in X and Y directions.
- The direction of the snake is changed when buttons three and four are pressed.
- The snake chases square rats, which, when eaten, increases the length of the snake and awards points to the player.
- The snake can collide with itself and walls, in which case the player loses. 
- Display the number of square rats eaten (score) when the game is over, as well as the most number of rats eaten (high score). 
- Make use of a timer to control the flow and pace of the game.

## Solution

We developed our project on the ChipKIT Uno32 board together with the Basic I/O shield. The game is shown on the small display of the Basic I/O shield, and the push buttons are used to move the snake in vertical and horizontal directions. A timer is used to update the screen. We developed the game using MCB32tools and C.

We used a 2D array for the snake. We have divided the screen into 8 by 32 blocks of 4 by 4 pixels. Blocks in which the snake was present contained values greater than 0. We also had specific values for the head and tail in order to, for instance, be able to remove the tail. The block in which the rat was present had the value -1, which could be used to, for instance, check if the head of the snake had "collided" with the rat (i.e. eaten it). We drew blocks that didn't contain the value 0 in white, and all blocks that contained the value 0 remained black. We also used the data structure to check collisions with the walls and to check when the snake collided with itself (e.g. by checking if the position of the head also contained a value greater than zero that wasn't equal to the value of the head, meaning it also contained another part of the snakes body).

## Verification

The game was tested through playing multiple times and checking that the game can be played as intended. We also checked corner cases, such as what happens if several buttons are pressed at the same time or when the wrong buttons are pressed, and what happened when the head of the snake went to the block where the tail was in the last frame. (It shouldn't collide.)

## Contributions

Both members contributed equally to the header file and mipslabmain. In mipslabwork both members handled the initialization of the timer and global variables, although Edvin mostly handled this. Both members also handled the initialization of the snake, although Joakim mostly handled this. Other than that, both members contributed to drawing the snake and rat; the move functions; collision detection; the functions checkRat and startNewGame; and the main game loop. 

In terms of individual contributions, Edvin handled the rat function; the functions that check for button-presses to make the movement of the snake smoother; the function that draws individual pixels; and the function that generates random numbers. Joakim handled the remove tail function; the game over screen in which the score and high score is displayed (and associated functions that converts an integer to its' corresponding ascii value, and getting the first, middle and last digits of a score); the start screen; clearing the screen and drawing blocks that are four by four pixels.

## Reflections

One thing we have learned is to trust in our ability to develop a product from scratch, using our own imaginations and problem solving abilities. The project was especially hard in the beginning of the project, when we had to figure out how the OLED display works and when we were debating what data structure to use for the snake. But once we solved these problems, developing the rest of the game was actually fairly easy and a lot of fun!
