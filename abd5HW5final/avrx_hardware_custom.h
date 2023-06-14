#ifndef __AVRX_HARDWARE_CUSTOM__
#define __ARVX_HARDWARE_CUSTOM__

/*
    hardware.h
    Define my hardware dependent stuff here
    31-May-2002 - Added check for mega128
*/






#include <avr/io.h>
#include <avr/interrupt.h>

/******************************************************************************
 * The purpose of this code is to set up one timer to be used for the system tick
 * I tried to make as parameterized as possible
 *******************************************************************************/

/* I chose timer 2 */

//set the following to the interrupt that will have the system tick handler
#define AVRX_HARDWARE_TIMER_SIGNAL TIMER2_COMP_vect

//Setup
#define TCCR_SYS TCCR2A
#define TIMSK_SYS TIMSK2
#define TCNT_SYS TCNT2


//#define CPUCLK 8000000L     // CPU xtal
#define TICKRATE 1000       // AvrX timer queue Tick rate
//#define BAUDRATE 19200L     // Debug monitor baudrate not used so this is commented
#define CPUCLK (1UL*F_CPU)


// Setup the timer
#define TCNT_INIT_SYS_DIV 64UL //set this to the indended division factor that will be set with TCCR_CS_SYS
#define TCCR_CS_SYS ((1<<CS22) +(0<<CS21)) //set this to achieve divide factor specified on previous line, for the timer choosen
#define TCNT_MAX_ADJUST (1) //use to tweak frequency, I got better results with 1, but 0 is correct ideally 
#define TCNT_MAX (((CPUCLK+(TCNT_INIT_SYS_DIV*TICKRATE/2))/(TCNT_INIT_SYS_DIV*TICKRATE))-1+TCNT_MAX_ADJUST) //assumes clock divider of 256

// The macro below has the commands need to actually set up the timer
//use ctc mode (instead of overflow to avoid having to set tcnt every time in interrupt)
#define AVRX_HARDWARE_SETUP_COMMANDS MCUCR = 1<<SE;TCNT_SYS = 0;TCCR_SYS = (1 << WGM21) + TCCR_CS_SYS;TIMSK_SYS = (1<<OCIE2A);ASSR =0;OCR2A=TCNT_MAX;

#define AVRX311_WAIT_P(x) AvrXWaitSemaphore(&(x)) 
#define AVRX311_SIGNAL_V(x) AvrXSetSemaphore(&(x))

#endif //__AVRX_CUSTOM_HARDWARE__

