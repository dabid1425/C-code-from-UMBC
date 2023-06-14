/*
  Example code for setting up and using timers

  Two tasks each waiting on a timer, flashing LED's on
  the evaluation board.  See "hardware.h" for LED definitions

    The kernel Stack is defined by the IAR linker file, or Top or SRAM
    in GCC (can be altered in the GCC link files, or defined symbol)
 */
//#define _SFR_ASM_COMPAT 1
#include "avrx/avrx-io.h"
#include "avrx/avrx-signal.h"
#include "avrx/avrx.h"
#include "avrx/serialio.h"       // From AvrX...
#include "avrx_hardware_custom.h"
#include "U0_UART.h"
#include "ADC.h"
#include "math.h"
void MusicPlayB(void);
void MusicPlayA(void);
void MusicPlayG(void);
void MusicSetupPort(void);
void MusicSetupTimer1(void);
void printMenu(void);
extern FILE uart_stream;
////////////////////

//include fix for missing AvrXYield() function in OS
#include "AvrXYieldHack.h"
int gameRunning=0;

//project
#include "U0_LCD_Driver.h"
#include <util/delay.h> 
#include "U0_UART.h"
#include <stdlib.h>
void runStuff(void);
static int music=0;
int adcBuffer[100]={0};
int sizeOfBuffer=0;
int head=0;
int tail=0;
int capt=0;
int rolled=0;
#include <avr/io.h>

/////////////////////

#define LED PORTB           // Change this per your hardware and jumper setup
#define LEDDDR DDRB
int pos=1;
//handler for system tick
// it processes software timers and allows for scheduling / preemptive context switching
AVRX_SIGINT(AVRX_HARDWARE_TIMER_SIGNAL)
{
	IntProlog();                // Switch to kernel stack/context
	AvrXTimerHandler();         // Call Time queue manager
	PORTB^=(0x02);
	Epilog();                   // Return to tasks
}



TimerControlBlock timer1,timer2,timer3,timerNote,timerMusic,timerCapture,timerPrint;


// mutex signals can be used for locking resources and syncronizing
// note mutex semaphore is not binary in AvrX
Mutex  menu,timeOut,triggerMem,triggerPrint,triggerWrite;



/*
 Task 1 simply flashes the light off for 1/5 second and then on for 4/5th
 for a 1 second cycle time.
 */
//parameters are 
//  task1 id, 
//  extra stack size beyond default (35) allow two bytes for each level of function calls + local variables 
//  priority 0-15 (lower is higher priority)


//MUSIC TASK
//required software timers

AVRX_GCC_TASKDEF(taskMusic,14,3){
	#define PORTB5_MASK  0b00100000
	DDRB |= PORTB5_MASK; //enable PORTB5 as output
	/* Timer/Counter1 Control Register A */
	int value_TCCR1A_COM1A_2b = 2; //clear on match up, set on match down
	int value_TCCR1A_COM1B_2b = 0; //disable
	//this one is spread over both regsters
	int value_TCCR1AB_WGM1_4b = 8; //PWM+frequency control using register ICR1
	/* Timer/Counter1 Control Register B */
	int value_TCCR1B_ICNC1_1b = 0; //no input capture noise cancel
	int value_TCCR1B_ICES1_1b = 0; //Dont Care since not used
	int value_TCCR1B_CS1_3b = 2; //use clock prescaler 8...see page 131 of datasheet...1Mhz for 8Mhz clk
	TCCR1A = (value_TCCR1A_COM1A_2b << COM1A0) + (value_TCCR1A_COM1B_2b << COM1B0) + ((value_TCCR1AB_WGM1_4b&0x3) << WGM10);
	TCCR1B = (value_TCCR1B_ICNC1_1b << ICNC1) + (value_TCCR1B_ICES1_1b << ICES1) + (((value_TCCR1AB_WGM1_4b&0b00001100)>>2) << WGM12) + (value_TCCR1B_CS1_3b<<CS10);
	/* Timer/Counter1 Input Capture Register */
	//also used for frequency setting
	ICR1 = 0xFF/2; //16 bits //just some initial value
	/* Timer/Counter 1 Interrupt Mask Register */
	int value_TIMSK1_ICIE1_1b = 0;//input capture int. enable
	int value_TIMSK1_OCIE1B_1b = 0;//output compare int. enable
	int value_TIMSK1_OCIE1A_1b = 0;//output compare int. enable
	int value_TIMSK1_TOIE1_1b = 0;//overflow int. enable
	TIMSK1 = (value_TIMSK1_ICIE1_1b <<ICIE1) + (value_TIMSK1_OCIE1B_1b<<OCIE1B) + (value_TIMSK1_OCIE1A_1b<<OCIE1A) + (value_TIMSK1_TOIE1_1b<<TOIE1);

	for(;;){
		MusicPlayB();
		MusicPlayA();
		MusicPlayG();
		AvrXDelay(&timerMusic,500);
	}
}


// ******************************************************************************
//  MUSIC FUNCTIONS USING TIMER 1
// ******************************************************************************



void MusicSetNote(int period_us){
	ICR1=period_us/2;
	OCR1A=period_us/4;
	TCNT1=0;
}

void MusicRest(){
	OCR1A=0;
};

void MusicPlayG(){
	MusicSetNote(2552);
	AvrXDelay(&timerNote, 500);
	MusicRest();
}

void MusicPlayA(){
	MusicSetNote(2272);
	AvrXDelay(&timerNote, 500);
	MusicRest();
}

void MusicPlayB(){
	MusicSetNote(2024);
	AvrXDelay(&timerNote, 500);
	MusicRest();
}

//global variables
char  g_buttonPressedFlag;
char g_buttonPressed; //up=4,down=3,left=2,right=1,center=0
#define BUTTON_CODE_UP 4
#define BUTTON_CODE_DOWN 3
#define BUTTON_CODE_CENTER 0

//Setting the interrupt handler
void SetupButtonInterrupts(void){
	PCMSK1  |= (1<<PCINT15)+(1<<PCINT14)+(1<<PCINT12); //Unmask bit for Center Button and LR on Butterfly, PB4->PCINT12 to allow it to trigger interrupt flag PCIF1
	EIMSK   = (1<<PCIE1);    //Enable interrupt for flag PCIF1
}
int HasButtonPressed(){
	return g_buttonPressedFlag;
}
int GetButtonPressed(){
	int return_val;
	if (g_buttonPressedFlag==1){
		//save button THEN release hold
		return_val = g_buttonPressed;
		g_buttonPressedFlag=0;
		return return_val;
		}else{
		return -1;
	}

}
//setup pins for input
void SetupJoystickPins(void){
	//set as input and enable pull up resistor for pins 4,6,7 to use the pushbuttons
	DDRB  &= 0b00101111; //set three pin directions
	PORTB |= 0b11010000; //set three pin pull ups
}
//required timer
TimerControlBlock timerGame;
static volatile unsigned char display[3] = {'0','0','0'};

const int xDelay = 500;

static enum state_t {open00,open0,open1,roll0,roll123,roll4,win,loss} state=open00;

static unsigned char stopCount; //number of "wheels" stoped
//GAME TASK
AVRX_GCC_TASKDEF(taskGame,22,3){

	LCD_AllSegments(FALSE); 
	AvrXDelay(&timerGame, 2);
	
	for(;;){
		switch (state) {
			case open00: //just an entry point
			state=open0;
			break;
			case open0:  //display play prompt
			LCD_WriteDigit('P',3);
			LCD_WriteDigit('L',4);
			LCD_WriteDigit('Y',5);
			state=open1;
			break;
			case open1: //wait for button press
			// center causes game to start
			AVRX311_WAIT_P(timeOut);
			if (HasButtonPressed()){
				switch (GetButtonPressed()){
					case BUTTON_CODE_CENTER:
					state = roll0;
					rolled=1;
					break;
					case BUTTON_CODE_UP: //on up suspend and resume in same place
					state=open0;
					break;
				}
				}else{
				// do nothing if no button pressed
			}
			break;

			case roll0: //roll sub state to init stop count
			stopCount = 0;
			state = roll123;
			break;

			case roll123: //roll state, takes three center presses to complete game
			
			if (HasButtonPressed()){
				GetButtonPressed();
				stopCount+=1;
				if (stopCount == 3){
					state = roll4;
				}
			}
			LCD_WriteDigit(display[1],4);
			LCD_WriteDigit(display[2],3);
			LCD_WriteDigit(display[0],5);
			//generate and display roll of 3 random numbers
			if (stopCount < 3){
				display[0]='0' + rand()%5;
				LCD_WriteDigit(display[0],5);
				if (stopCount<2){
					display[1]='0' + rand()%5;
					LCD_WriteDigit(display[1],4);
					if (stopCount<1){
						display[2]='0' + rand()%5;
						LCD_WriteDigit(display[2],3);
					}
				}
			}

			AvrXDelay(&timerGame, 1000); //sets roll speed
			break;

			case roll4: //roll subsate t decide win or loss
			if (display[0]==display[1]&&display[0]==display[2]){
				state = win;
				}else{
				state = loss;
			}
			break;

			case win: // display win and restart
			AvrXDelay(&timerGame, xDelay); LCD_WriteDigit('W',3);
			AvrXDelay(&timerGame, xDelay); LCD_WriteDigit('I',4);
			AvrXDelay(&timerGame, xDelay); LCD_WriteDigit('N',5);
			AvrXDelay(&timerGame, xDelay); LCD_WriteDigit(display[2],3);
			AvrXDelay(&timerGame, xDelay); LCD_WriteDigit(display[1],4);
			AvrXDelay(&timerGame, xDelay); LCD_WriteDigit(display[0],5);
			AvrXDelay(&timerGame, xDelay);
			LCD_WriteDigit(';',3);  //clear
			LCD_WriteDigit('+',4);
			LCD_WriteDigit('5',5);
			//TODO: should show user cash
			state=open0;
			break;

			case loss: //display loss and restarts
			AvrXDelay(&timerGame, xDelay);        LCD_WriteDigit('X',3);
			AvrXDelay(&timerGame, xDelay);        LCD_WriteDigit('X',4);
			AvrXDelay(&timerGame, xDelay);        LCD_WriteDigit('X',5);
			AvrXDelay(&timerGame, xDelay);        LCD_WriteDigit(display[2],3);
			AvrXDelay(&timerGame, xDelay);        LCD_WriteDigit(display[1],4);
			AvrXDelay(&timerGame, xDelay);        LCD_WriteDigit(display[0],5);
			AvrXDelay(&timerGame, xDelay);
			LCD_WriteDigit(';',3);  //clear
			LCD_WriteDigit('+',4);
			LCD_WriteDigit('0',5);
			state=open0;
			break;
		}
	}
	}


//prints the which task the user is currently looking at 
AVRX_GCC_TASKDEF(taskMenu,8,3){
	for(;;){
		AVRX311_WAIT_P(menu);
		if(pos==1){
			LCD_WriteDigit('M',0);
			LCD_WriteDigit('U',1);
			LCD_WriteDigit('S',2);
			LCD_WriteDigit('C',3);
			}else if(pos==2){
			LCD_WriteDigit('G',0);
			LCD_WriteDigit('A',1);
			LCD_WriteDigit('M',2);
			LCD_WriteDigit('E',3);
			}else if(pos==3){
			LCD_WriteDigit('C',0);
			LCD_WriteDigit('A',1);
			LCD_WriteDigit('P',2);
			LCD_WriteDigit('T',3);
			}else if(pos==4){
			LCD_WriteDigit('S',0);
			LCD_WriteDigit('T',1);
			LCD_WriteDigit('S',2);
			LCD_WriteDigit('D',3);
			}else{
			LCD_WriteDigit('S',0);
			LCD_WriteDigit('P',1);
			LCD_WriteDigit('S',2);
			LCD_WriteDigit('D',3);
		}
	}
}
//prints the read in temperature to the uart if the buffer has data in it
AVRX_GCC_TASKDEF(taskPrint,14, 3){
	for(;;){
		if(head!=tail){
			cli();
			printf("%d",adcBuffer[tail]);
			printf("\n");
			tail=(tail+1)%100;
			AVRX311_SIGNAL_V(triggerMem);
			sei();
		}else{
			sei();
			AVRX311_WAIT_P(triggerPrint);
		}
		AvrXDelay(&timerPrint,20);//will slow down the music by a little bit
	}
}
//captures the adc temperature and will save the data 
AVRX_GCC_TASKDEF(taskCapture,16, 3){
	for(;;){
		int adcVal=ADCAquire();
		head=(head+1)%100;
		if(head!=tail){
			adcBuffer[head]=adcVal;
			AVRX311_SIGNAL_V(triggerPrint);
		}else{
			AVRX311_WAIT_P(triggerMem);
		}	
		AvrXDelay(&timerCapture,20);//will slow down the music by a little bit 
	}
}
ISR(TIMER0_COMP_vect){	
}


// CATCH BAD ISR (better than infinite resetting when developing)
// bad ISR can cause infinite resetting (not good for EEPROM if you write to it)
ISR(BADISR_vect){
	//static volatile char count;
	//LCD_WriteDigit('B',0);
	//LCD_WriteDigit('D',1);
	//LCD_WriteDigit('I',2);
	//LCD_WriteDigit('S',3);
	//LCD_WriteDigit('R',4);
	//while(1){
	//	count++;
	//}
}
AVRX_SIGINT(PCINT1_vect){ 		//remember this is called on pin change 0->1 and 1->0
	IntProlog();     
	static uint8_t pinBPrev=0b11111111; //for storing previous value of port to detect

	//toggle PORTB0 based on up button status being newly pressed, but not when it is released
	if(((PINB& (1<<6))  == 0) &&((pinBPrev & (1<<6))  != 0)){
		if(gameRunning==1){
			// suspend Game
				AvrXSuspend(PID(taskGame));
				if(rolled==0){
					state=open0;
					AVRX311_SIGNAL_V(timeOut);
				}
				LCD_AllSegments(FALSE); 
				gameRunning=0;
		}else{
		if(pos!=5){
			pos++;
		}else{
			pos=1;
		}
		}
		AVRX311_SIGNAL_V(menu);
		//toggle PORTB0 based on down button status being newly pressed, but not when it is released
	}else if(((PINB& (1<<7))  == 0) &&((pinBPrev & (1<<7))  != 0) &&gameRunning==0){
		if(pos!=1){
			pos--;
		}else{
			pos=5;
		}
		AVRX311_SIGNAL_V(menu);
	}else if(((PINB& (1<<4))  == 0) &&((pinBPrev & (1<<4))  != 0)){
		if(pos==1){//Play music
			//execute music task
			if(music!=5){
				music=5;
				AvrXResume(PID(taskMusic));
			}
		}else if(pos==2){
			if(gameRunning==1){
				g_buttonPressedFlag=1;
				g_buttonPressed=BUTTON_CODE_CENTER;
				AVRX311_SIGNAL_V(timeOut);
			}else{			
				LCD_AllSegments(FALSE); 
				gameRunning=1;
				AvrXResume(PID(taskGame));
			}
		}else if(pos==3){
			capt=1;
			ADCSetup();
			TCCR0A=0b01111100;
			AvrXResume(PID(taskCapture));
		}else if(pos==4){
			AVRX311_SIGNAL_V(triggerMem);
			AVRX311_SIGNAL_V(triggerPrint);
			AvrXResume(PID(taskPrint));
		}else if(pos==5){
			AvrXSuspend(PID(taskPrint));
		}
		}
	pinBPrev = PINB; //save button status
	Epilog();
}
void SetupInterrupts(){
	//Setup for UP/Down Button Interrupt
	PCMSK1  |= ((1<<PCINT14)|(1<<PCINT15)|(1<<PCINT12)); //Unmask bit for UP/Down Button on Butterfly, PB4->PCINT14/PB4->PCINT15 to allow it to trigger interrupt flag PCIF1
	//Setup for Right Button Interrupt
	EIMSK   = (1<<PCIE1);    //Enable interrupt for flag PCIF
}
//prints the menu to the LCD

int main(void){                 // Main runs under the AvrX Stack
	AvrXSetKernelStack(0);

	//  Must initialize semaphores
	//  use AVRX311_SIGNAL_V(anysemaphore);
	AVRX_HARDWARE_SETUP_COMMANDS;
	
	// Other setup code
	DDRB  &= ~0b11010000;  //set B4,B6,B7 as inputs
	DDRE  &= ~0b00001100;  //set all to inputs
	PORTB |=  0b11010000;  //enable pull up resistors on B4,B6,B7
	PORTE |=  0b00001100;  //enable pull up resistor on pin E2,E3
	SetupInterrupts();
	LCD_Init();
	stderr = stdout = stdin= &uart_stream;
	UARTInit();//set UART
	AvrXInitTask(TCB(taskMusic));
	AvrXInitTask(TCB(taskGame));
	AvrXInitTask(TCB(taskCapture));
	AvrXInitTask(TCB(taskPrint));
	AVRX311_SIGNAL_V(menu);
	AvrXRunTask(TCB(taskMenu));
	Epilog();                   // Switch from AvrX Stack to first task
	while(1);
}
