/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2021-12-14 by Joakim Abdinur Iusuf
   This file modified 2021-12-14 by Edvin Nordling

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

// Dimensions of the game field (see snakeArray) where each coordinate is 4*4 pixels.
#define height 8
#define width 32

// The directions the snake can be facing.
#define west 0
#define north 1
#define east 2
#define south 3

// The directions the snake can be turning.
#define left 0
#define forward 1
#define right 2

/* Global variables */
int snakeArray[height][width], xPos, yPos, temp, head, tail, direction;

int ratExists; 

int gameOver; 

int timeoutcount, gameOverCount;

int score, highscore;


int turnDirection = forward;
int prevBTN4 = 0;
int prevBTN3 = 0;

int timeoutcount;

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
void labinit(void) {
  TRISD = TRISD | 0xfe0;

  // 2b) Se page 9 i Timer manualen.
  T2CON = 0x70; // Set bit 4-6 to 111 -> 0000...0111 0000 -> 0x70 and set on bit to 0

  int clockRate = 80000000; //80MHz
  int scale = 256;
  int periodms = 30; // 10 gives time out about every 100ms
  PR2 = (clockRate / scale) / periodms;
  
  TMR2 = 0; // Reset timer
  T2CONSET = 0x8000; // Start the timer

  gameOver = 0;
  score = 0;
  highscore = 0;

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
Jocke and Edvin.
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
  direction = east; // initiate direction to be east

  int i;
  for (i = 0; i < head; i++) {
    temp++;
    snakeArray[yPos][temp - head] = i + 1;
  }
}

/*
Jocke and Edvin.
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
void moveWest(void) {
  xPos--;
  head++;
  gameOverOrCheckRat();
}

/*
Jocke and Edvin.
Moves snake to the right by incrementing the xPosition by 1.
We also check collision with the rat, itself and the walls.
The value of head is incremented and the value of snakeArray[yPos][xPos] is set
to head.
*/
void moveEast(void) {
  xPos++;
  head++;
  gameOverOrCheckRat();
}

/*
Jocke and Edvin.
Moves the snake upwards by decrementing the yPosition by 1.
We also check collision with the rat, itself and the walls.
The value of head is incremented and the value of snakeArray[yPos][xPos] is set
to head.
*/
void moveNorth(void) {
  yPos--;
  head++;
  gameOverOrCheckRat();
}

/*
Jocke and Edvin.
Moves the snake downwards by incrementing the yPosition by 1.
We also check collision with the rat, itself and the walls.
The value of head is incremented and the value of snakeArray[yPos][xPos] is set
to head.
*/
void moveSouth(void) {
  yPos++;
  head++;
  gameOverOrCheckRat();
}

/*
Jocke och Edvin.
Fixes bug where score is increased if one crashes into the top wall.
*/
void gameOverOrCheckRat(void) {
  if (checkGameOver()) {
    delay(1000);
    gameOver = 1;
  } else {
    checkRat();
    snakeArray[yPos][xPos] = head;
  }
}

/*
Jocke and Edvin.
Valid y-positions range from 0 to 7 (inclusive), and valid x-positions range from 0 to 31 (inclusive).
So if yPos is less than 0 or greater than 7, or if xPos is less than 0 or greater than 31 the snake is outside of the screen.
This means it has hit a wall and should die.
*/
int hitsWall() {
  if (xPos < 0 || xPos > 31 || yPos < 0 || yPos > 7) {
    return 1;
  } else {
    return 0;
  }
}

/*
Jocke and Edvin.
If the position of the head is greater than 0, it has collided with itself,
unless it's the tail. (The tail will move next frame so the coordinate is up for grabs in that case.)
*/
int snakeCollidedWithItself() {
  if (snakeArray[yPos][xPos] > 0 && snakeArray[yPos][xPos] != tail) { // A snake bodypart is at these coordinates, but not the tail.
    return 1;
  } else {
    return 0;
  }
}

/*
Jocke and Edvin.
Called in moveWest, moveEast, moveNorth and moveSouth. Check the cases where the 
snake dies, i.e. when it hits a wall or itself.
*/
int checkGameOver() {
  if (hitsWall() || snakeCollidedWithItself()) {
    return 1;
  } else {
    return 0;
  }
}

/*
Jocke.
Convert number to its' corresponding ascii value.
*/
char intToAscii(int number) {
  char num = number + 48;
  return num;
}

/*
Jocke.
Get the first digit of a three digit number.
*/
int getFirstDigit(int number) {
  int num = number / 100;
  return num;
}

/*
Jocke.
Get the middle digit of a three digit number.
*/
int getMiddleDigit(int number) {
  if (number < 100) {
    int num = number / 10;
    return num;
  } else if (number >= 100 && number < 199) {
    number = number - 100;
    int num2 = number / 10;
    return num2;
  } else {
    number = number - 200;
    int num2 = number / 10;
    return num2;
  }
}

/*
Jocke.
Get the last digit of a three digit number.
*/
int getLastDigit(int number) {
  int num = number % 10;
  return num;
}

/*
Jocke.
Display game over and the score.
*/
void displayGameOverScreen(void) {
  int firstDigit = getFirstDigit(score);
  int middleDigit = getMiddleDigit(score);
  int lastDigit = getLastDigit(score);
  char firstDigitAsChar = intToAscii(firstDigit);
  char middleDigitAsChar = intToAscii(middleDigit);
  char lastDigitAsChar = intToAscii(lastDigit);

  char scoreArray[] = {'S', 'c', 'o', 'r', 'e', ':', ' ', firstDigitAsChar, middleDigitAsChar, lastDigitAsChar, '\0'};

  int firstDigitHS = getFirstDigit(highscore);
  int middleDigitHS = getMiddleDigit(highscore);
  int lastDigitHS = getLastDigit(highscore);
  char firstDigitAsCharHS = intToAscii(firstDigitHS);
  char middleDigitAsCharHS = intToAscii(middleDigitHS);
  char lastDigitAsCharHS = intToAscii(lastDigitHS);

  char highScoreArray[] = {'H', 'i', 'g', 'h', 's', 'c', 'o', 'r', 'e', ':', ' ', firstDigitAsCharHS, middleDigitAsCharHS, lastDigitAsCharHS, '\0'};

  display_string(0, "Game over!");
  display_string(1, scoreArray);
  display_string(2, highScoreArray);
  display_update();
}

/*
Jocke and Edvin.
Start a new game by initializing the snake and setting rat exists to 0.
*/
void startNewGame(void) {
  initializeSnake(); 
  gameOver = 0;
  ratExists = 0;
  score = 0;
  rat();
  turnDirection = forward;
}

/*
Jocke och Edvin.
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
in the other directions). If BTN4 is pressed, we call moveNorth() and change the direction to
1 (up). If BTN3 is pressed, we call moveSouth() and change the direction to 3. Otherwise
it will simply keep moving to the right. 
*/
void moveSnake(void) {
  if (direction == west) { 
    if (turnDirection == left) { //BTN4 left
      direction = south;
      moveSouth();    
    } else if (turnDirection == right) { //BTN3 right
      direction = north;
      moveNorth();      
    } else {
      moveWest();
    }
  } else if (direction == north) { 
    if (turnDirection == left) {
      direction = west;
      moveWest();      
    } else if (turnDirection == right) {
      direction = east;
      moveEast();      
    } else {
      moveNorth();
    }
  } else if (direction == east) { 
    if (turnDirection == left) {
      direction = north;
      moveNorth();      
    } else if (turnDirection == right) {
      direction = south;
      moveSouth();      
    } else {
      moveEast();
    }
  } else if (direction == south) {
    if (turnDirection == left) {
      direction = east;
      moveEast();      
    } else if (turnDirection == right) {
      direction = west;
      moveWest();
    } else {
      moveSouth();
    }
  }
}

/*
Edvin.
Spawn the rat in a random position.
*/
void rat(){
  while(!ratExists){
    int xRat = rand() % 32; 
    int yRat = rand() % 8;
    if(snakeArray[yRat][xRat]==0){
      snakeArray[yRat][xRat]=-1;
      ratExists=1;
    }
  }
}

/*
Edvin and Jocke.
Check if the position of the snakes head is the same as that of the rat.
If it is, increase the score and check to the if the current score is a new high score.
Otherwise, simply remove the tail. This function is called each time the snake moves.
*/
void checkRat() {
  if(snakeArray[yPos][xPos] == -1) {
    score++;
    if (highscore < score) {
      highscore = score;
    }
    ratExists = 0;
    rat();
  } else {
    removeTail();
  }
}

/*
Jocke.
Display start screen and start the game if BTN2 is pressed.
*/
void displayStartScreen(void) {
  int buttonNotPressed = 1;
  while (buttonNotPressed) {
    display_string(0, "Press BTN2 to");
    display_string(1, "start playing.");
    display_string(2, "");
    display_update();
    if (getbtns() & 0x1) {
      buttonNotPressed = 0;
    }
  }
  startNewGame();
  buttonNotPressed = 1;
}

/*
Edvin
Rising edge occurs if the button is pressed but wasn't pressed last time it was checked.
*/
int buttonRisingEdge(BTN,prevBTN){
  if(!prevBTN){
    if(BTN){
      return 1;
    }
  }
  return 0;
}


/*
Edvin
Reads the current value of buttons 3 and 4, and change turnDirection variable if the button is on rising edge.
Also saves the value for next read, so that rising edge can be checked.
*/
void readButtons(){
  int BTN4 = (getbtns() >> 2) & 0x1; //Read BTN4 1 or 0
  int BTN3 = (getbtns() >> 1) & 0x1; //Read BTN3 1 or 0

  if(buttonRisingEdge(BTN3,prevBTN3)){
    turnDirection = right;
  }
  if(buttonRisingEdge(BTN4, prevBTN4)){
    turnDirection = left;
  }
  prevBTN3 = BTN3;
  prevBTN4 = BTN4;
}


/* 
Jocke and Edvin.
This function is called repetitively from the main program.

It uses a timer, and the timer has to time out a certain amount of times
before the game logic runs once. The loop then resets and the timer has to
go out the same amount of times again.

A better name for the function would be gameLoop but because of
dependencies we will keep its' original name.
*/
void labwork(void) {
  int timerHasElapsed = IFS(0) & 0x100; // 16 bit timers

  if (gameOver) {
    if (timerHasElapsed) {
      gameOverCount++; //a time out count for the game over screen
      IFS(0) = IFS(0) & 0xFFFFFEFF; // reset timer

      displayGameOverScreen();
      if (gameOverCount == 90) {
        displayStartScreen();
        gameOverCount = 0; // reset timer loop
      }
    }
  } else {
    if (timerHasElapsed) {
      timeoutcount++;
      IFS(0) = IFS(0) & 0xFFFFFEFF; // reset timer

      readButtons();

      if (timeoutcount == 6){
        clearScreen();
        moveSnake();
        turnDirection = forward;
        drawSnakeAndRat();
        display_image(0, screen);
        timeoutcount = 0; // reset timer loop
      }
    }
  }
}