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

int snakeArray[height][width], xPos, yPos, temp, head, tail, direction, ratExists;

// 1d) porte is used in more than one function.
// volatile int = (volatile int*) 0xbf886110; // 0xbf886110

/* Interrupt Service Routine */
void user_isr( void ) {
    return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  // 1c) 
  volatile int* trise = (volatile int*) 0xbf886100; 
  *trise = (*trise) & 0xffffff00; 
  
  // 1e)
  // 0000 1111 1110 0000 -> 0x0fe0
  TRISD = TRISD | 0xfe0;
  
  return;
}

void setAllElementsOfSnakeArrayToZero(void) {
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      snakeArray[i][j] = 0;
    }
  }
}

// Divide the 2D snake array into 32x8 blocks, each being 4x4 pixels?
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

void drawSnake(void) {
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if (snakeArray[i][j] != 0) { 
        drawBlock(i*4, j*4);
      }
    }
  }
}

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

void moveLeft(void) {
  xPos--;
  head++;
  checkRat();
  checkGameOver();
  snakeArray[yPos][xPos] = head;
}

void moveRight(void) {
  xPos++;
  head++;
  checkRat();
  checkGameOver();
  snakeArray[yPos][xPos] = head;
}

void moveUp(void) {
  yPos--;
  head++;
  checkRat();
  checkGameOver();
  snakeArray[yPos][xPos] = head;
}

void moveDown(void) {
  yPos++;
  head++;
  checkRat();
  checkGameOver();
  snakeArray[yPos][xPos] = head;
}

void checkGameOver(void) {
  if (xPos < 0 || xPos > 31) {
    delay(1000);
    initializeSnake();
    ratExists=0;
    rat();
  }

  if (yPos < 0 || yPos > 7) {
    delay(1000);
    initializeSnake();
    ratExists=0;
    rat();
  }
}

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
  // checkGameOver();
  
  
  drawSnake();
  
  display_image(0, screen);
}
