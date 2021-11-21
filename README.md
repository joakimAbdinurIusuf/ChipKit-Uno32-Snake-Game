# IS1500-Snake-Project

## Objective and Requirements

The objective of this project is to develop the game Snake. The snake eats square rats, and each time it eats square rats it grows. If it collides withit self or one of the walls, it dies and the game restarts. The player needs to make the snake grow as much as possible. Each time the snake eats square rats, the player is awarded points. These are the requirements of the game:


- Display game on built-in OLED graphical display.
- Snake moves across the screen in X and Y directions.
- The direction of the snake is changed when the buttons are pressed.
- Snake chases square rats, which, when eaten, increases the length ofthe snake and awards points to the player.
- Snake can collide with itself and walls, in which case the player loses.
- Display number of square rats eaten (score) when the game is over.

## Solution

We will develop our project on the ChipKIT Uno32 board together with the Basic I/O shield. The game will be shown on the small display of the BasicI/O shield, and the push buttons will be used to move the snake in vertical and horizontal directions. The interrupts triggered by the built in timer will be used to update the screen. We will develop the game using MCB32tools and C.

## Verification

The game will be tested primarily through playing and checking that the game can be played as intended. We will especially test corner cases, such as what happens if several buttons are pressed at the same time, what happens when the snake hits a wall or itself and what happens when the wrong buttons are pressed.
