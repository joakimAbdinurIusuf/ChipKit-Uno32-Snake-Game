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

/* Global variables */
int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

// 1d) porte is used in more than one function.
// volatile int = (volatile int*) 0xbf886110; // 0xbf886110

/* Interrupt Service Routine */
void user_isr( void ) {
    
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  // 1c) 
  volatile int* trise = (volatile int*) 0xbf886100; 
  *trise = (*trise) & 0xff00; 
  
  // 1d) Initialize porte to 0
  PORTE = 0;
  
  // 0000 1111 1110 0000 -> 0x0fe0
  TRISD = TRISD | 0xfe0;
  
  // 2b) Se page 9 i Timer manualen.
  
  T2CON = 0x70; // Set bit 4-6 to 111 -> 0000...0111 0000 -> 0x70 and set on bit to 0

  int clockRate = 80000000;
  int scale = 256;
  int periodms = 10;
  PR2 = (clockRate / scale) / periodms;
  
  TMR2 = 0; // Reset timer
  T2CONSET = 0x8000; // Start the timer
    
  // 3h) Page 53, data sheet
  IEC(0) = IEC(0) | 0x100; // write a one to the T2IE bit in IEC0
  // each timer has three bits that handle priority
  // write a non-zero priority value to the three T2IP bits in IPC2: 100
  IPC(2) = 4; // 4 till 31 går bra, för 2 least sig bits är subpriority
    
  IEC(0) = IEC(0) | 0x80000;
  IPC(4) = 0xFFFFFFFF;
  enable_interrupt(); // enable globally
  
  return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {
  // clearScreen();
  drawBlock(124, 28);
  display_image(0, screen);
}
