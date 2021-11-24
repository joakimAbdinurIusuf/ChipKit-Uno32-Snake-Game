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
// 3a)
int prime = 1234567;

// 3f)
int timeoutcount = 0;

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

// 1d) porte is used in more than one function.
// volatile int = (volatile int*) 0xbf886110; // 0xbf886110

/* Interrupt Service Routine */
void user_isr( void ) {
    /*
    
    // 3e)
    // Acknowledge interupts from Timer 2.
    
    // int elapsed = (IFS(0) & 0x100)>>8;
    // if(elapsed==1)

     
    if (IFS(0) & 0x100) {
        // reset 8th bit to 0, data sheet page 52
        IFS(0) = IFS(0) & 0xFFFFFEFF; // 0xFFFFFEFF
        
        // 3f)
        timeoutcount++;
        
        if (timeoutcount == 10) {
            time2string( textstring, mytime );
            display_string( 3, textstring );
            display_update();
            display_image(96, icon);
            tick( &mytime );
            timeoutcount = 0;
        }
    }
    */
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
  // prime = nextprime( prime );
  //display_string( 0, itoaconv( prime ) );
  display_update();
  display_image(0, icon);
}

/*
 • When the time-out event-flag is a "1", how does your code reset it to "0"?
 
 A) It masks the 8th bit with the line: IFS(0) = IFS(0) & 0xFFFFFEFF;
 
 • What would happen if the time-out event-flag was not reset to "0" by your code? Why?
 
 A) No difference, as opposed to in the previous assignment where the time2string and display functions were in a while loop and were executed much faster.
 
 • From which part of the code is the function user_isr called? Why is it called from there?
 
 From line 209 in vector.S (jal user_isr). Because as the file says that is where interupts are handled.
 
 • Why are registers saved before the call to user_isr? Why are only some registers saved?
 
 Registers $1 through $15, $24, and $25 can be changed by any C function, or any assembler subroutine. Register $ra will be changed by the call to the C function, i.e., the jal instruction. Saving these registers is necessary before calling the C function user_isr.
 
 • Which device-register (or registers), and which processor-register (or registers) must be written to enable interrupts from the timer? Describe the functions of the relevant registers.
 
 We have to write a one to the T2IE bit in IEC0, and write a non-zero priority value to the three T2IP bits in IPC2.
 
 IECx (Interrupt Enable Control)
 - The initialization procedure should set the bit for the interrupt that should be enabled.
 
 IPCx (Interrupt Priority Control)
 - Configure the priority (0-7, where 7 is highest) and the sub- priority (0-3 where 3 is highest).
 
 */
