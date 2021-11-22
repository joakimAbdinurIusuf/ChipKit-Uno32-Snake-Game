#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(void);
int getbtns(void);

int getsw(void) {
    // SW1-SW2 are bits 8-11 in PORTD, they should be bits 0-3 so we bit shift them by 8
    int statusOfToggleSwitches = (PORTD >> 8) & 0xf; // mask so all bits, except for the 4 LSB (1111), are zero
    return statusOfToggleSwitches;
}

int getbtns(void) {
    // BTN2-BTN4 are bits 5-7 in PORTD, they should be bits 0-2 so we bit shift them by 5
    int statusOfPushButtons = (PORTD >> 5) & 0x7; // mask so all bits, except for the 3 LSB (0111), are zero
    return statusOfPushButtons;
}

