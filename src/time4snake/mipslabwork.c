/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog
   This file modified 2021-10-07 by Joakim Abdinur Iusuf

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#define height 8
#define width 32

/* Global variables */
int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

int snakeArray[height][width], xPos, yPos, temp, head, tail, direction, ratExists; // Jocke and Edvin

/* 
Jocke and Edvin.
Interrupt Service Routine 
*/
void user_isr(void) {
    return;
}

/* 
Jocke and Edvin.
Lab-specific initialization goes here.
*/
void labinit(void)
{
  // 1c) 
  volatile int* trise = (volatile int*) 0xbf886100; 
  *trise = (*trise) & 0xffffff00; 
  
  // 1e)
  // 0000 1111 1110 0000 -> 0x0fe0
  TRISD = TRISD | 0xfe0;
  
  return;
}

/*
Jocke.
Called in initializeSnake. Set all elements in the 2D snake array to 0.
*/
void setAllElementsOfSnakeArrayToZero(void) {
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      snakeArray[i][j] = 0;
    }
  }
}

/*
Jocke.
Start by setting all the elements in the snake array to zero.
In drawSnakeAndRat, values that are not zero will be drawn.
So set xPos to width/2 and yPos to height/2 to draw the snake in the 
*/
void initializeSnake(void) {
  setAllElementsOfSnakeArrayToZero();

  xPos= width/2; 
  yPos = height/2; 
  temp = xPos;
  head = 5;
  tail = 1;
  direction = 2; // initiate direction to be right (0 = left, 1 = up, 2 = right, 3 = down)

  int i;
  for (i = 0; i < head; i++) {
    temp++;
    snakeArray[yPos][temp - head] = i + 1;
  }
}

/*
Jocke.
Draw the snake. Loop through the entire snake array. If it isn't 0,
the block at snakeArray[i][j] is either the snake (values greater than zero),
or a rat (value of -1). We then draw a white block in this positions. 
The rest of the blocks remain black.
*/
void drawSnakeAndRat(void) {
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if (snakeArray[i][j] != 0) { 
        drawBlock(i*4, j*4);
      }
    }
  }
}

/*
Jocke.
Tail is initially set to 1. In labwork, we call this function repeatedly.
We loop thorugh the entire 2D snakeArray. When we find a block that is equal to
the value of tail, we set the value at that block to 0. This means that when we draw the snake, 
this block will be black the next it is drawn. 
*/
void removeTail(void) {
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if (snakeArray[i][j] == tail) { 
        snakeArray[i][j] = 0;
      }
    }
  }
  tail++;
}

/*
Jocke and Edvin.
Moves snake to the left by decrementing the xPosition by 1.
We also check collision with the rat, itself and the walls.
The value of head is incremented and the value of snakeArray[yPos][xPos] is set
to head.
*/
void moveLeft(void) {
  xPos--;
  head++;
  checkRat();
  checkGameOver();
  snakeArray[yPos][xPos] = head;
}

/*
Jocke and Edvin.
Moves snake to the right by incrementing the xPosition by 1.
We also check collision with the rat, itself and the walls.
The value of head is incremented and the value of snakeArray[yPos][xPos] is set
to head.
*/
void moveRight(void) {
  xPos++;
  head++;
  checkRat();
  checkGameOver();
  snakeArray[yPos][xPos] = head;
}

/*
Jocke and Edvin.
Moves the snake upwards by decrementing the yPosition by 1.
We also check collision with the rat, itself and the walls.
The value of head is incremented and the value of snakeArray[yPos][xPos] is set
to head.
*/
void moveUp(void) {
  yPos--;
  head++;
  checkRat();
  checkGameOver();
  snakeArray[yPos][xPos] = head;
}

/*
Jocke and Edvin.
Moves the snake downwards by incrementing the yPosition by 1.
We also check collision with the rat, itself and the walls.
The value of head is incremented and the value of snakeArray[yPos][xPos] is set
to head.
*/
void moveDown(void) {
  yPos++;
  head++;
  checkRat();
  checkGameOver();
  snakeArray[yPos][xPos] = head;
}

void startNewGame() {
  
}

/*
Jocke.
Valid x-positions range from 0 to 31 (inclusive). So if xPos
is less than 0 or greater than 31 the snake is outside of the screen.
This means it has hit a wall and should die.
*/
int hitSideWall() {
  if (xPos < 0 || xPos > 31) {
    return 1;
  } else {
    return 0;
  }
}

/*
Jocke.
Valid y-positions range from 0 to 7 (inclusive). So if yPos
is less than 0 or greater than 7 the snake is outside of the screen.
This means it has hit a wall and should die.
*/
int hitUpperOrLowerWall() {
  if (yPos < 0 || yPos > 7) {
    return 1;
  } else {
    return 0;
  }
}

/*
Jocke.
If the position of the head is greater than 0, it has collided with itself.
*/
int snakeCollidedWithItself() {
  if (snakeArray[yPos][xPos] > 0) { 
    return 1;
  } else {
    return 0;
  }
}

/*
Jocke and Edvin.
Called in moveLeft, moveRight, moveUp and moveDown. Check the cases where the 
snake dies, i.e. when it hits a wall or itself.
*/
void checkGameOver(void) {
  if (hitSideWall() || hitUpperOrLowerWall() || snakeCollidedWithItself())  {
    delay(1000);
    initializeSnake(); 
    ratExists = 0;
    rat();
  }
}

/*
Jocke.
Continuously move the snake in a certain direction, or move it in a new
direction when BTN4 and BTN3 is pressed.

The principle is this: We have a global variable called direction which is initialized to
2 in initializeSnake. The value 0 means left, 1 means up, 2 means right and 3 means down.
In moveSnake() we have four overarching if and else if conditions that check the value of 
direction. 

We want BTN4 to move the snake to its' left, and BTN3 to move the snake to its'
right, whatever direction it has. So, for instance, if it is moving to the right, pressing
BTN4 should move the snake upwards (its' left) and pressing BTN3 should move it downwards
(its' right).

If the direction is equal to 2, it will check three conditions (the same thing applies
in the other directions). If BTN4 is pressed, we call moveUp() and change the direction to
1 (up). If BTN3 is pressed, we call moveDown() and change the direction to 3. Otherwise
it will simply keep moving to the right. 
*/
void moveSnake(void) {
  int BTN4 = (getbtns() >> 2) & 0x1;
  int BTN3 = (getbtns() >> 1) & 0x1;

  if (direction == 0) { // left
    if (BTN4) {
      moveDown();
      direction = 3;
    } else if (BTN3) {
      moveUp();
      direction = 1;
    } else {
      moveLeft();
    }
  } else if (direction == 1) { // up
    if (BTN4) {
      moveLeft();
      direction = 0;
    } else if (BTN3) {
      moveRight();
      direction = 2;
    } else {
      moveUp();
    }
  } else if (direction == 2) { // right
    if (BTN4) {
      moveUp();
      direction = 1;
    } else if (BTN3) {
      moveDown();
      direction = 3;
    } else {
      moveRight();
    }
  } else if (direction == 3) { // down
    if (BTN4) {
      moveRight();
      direction = 2;
    } else if (BTN3) {
      moveLeft();
      direction = 0;
    } else {
      moveDown();
    }
  }
}

void rat(){
  while(!ratExists){
    int xRat = rand() % 32; // add time function later?
    int yRat = rand() % 8;
    if(snakeArray[yRat][xRat]==0){
      snakeArray[yRat][xRat]=-1;
      ratExists=1;
    }
  }
}

void checkRat(){
  if(snakeArray[yPos][xPos]==-1){
    ratExists=0;
    rat();
  } else {
    removeTail();
  }
}


/* This function is called repetitively from the main program */
void labwork( void ) {
  delay(300);
  clearScreen();
  moveSnake();
  drawSnakeAndRat();
  display_image(0, screen);
}
