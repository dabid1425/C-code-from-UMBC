/*
 * HW4.c
 *
 * Created: 11/19/2013 11:40:25 AM
 *  Author: dan
 */

#ifndef F_CPU

#ifdef USING_BOOTLOADER
#define F_CPU 2000000UL
#else
#define F_CPU 8000000UL
#endif

#endif

#include <avr/io.h>
#include <util/delay.h>
#include "ADC.h"
#include "U0_LCD_Driver.h"
#include <avr/interrupt.h>
int bound1=20;
int bound2=74;
int lightsValue=0;
int followAvoid=0;
int primary=20;
void intToLCD(int angle){//writes the angle to the screen
	char array[3]={0};
	angle=angle-bound1;
	angle=angle*3.35;
	itoa(angle,array,10);
	LCD_WriteDigit(array[0],3);
	LCD_WriteDigit(array[1],4);
	LCD_WriteDigit(array[2],5);
}
void Sweep(){//sweeps 10 degrees to the left and the right of the primary angle and will either follow or avoid the light depending on where the servo is it may only be able to just move left or right 
	ADCSetup();
	if(followAvoid==0){
		LCD_WriteDigit('F',0);
		LCD_WriteDigit('T',1);
		LCD_WriteDigit('L',2);
	}else{
		LCD_WriteDigit('A',0);
		LCD_WriteDigit('T',1);
		LCD_WriteDigit('L',2);
	}
	int newPrimary=0;
	while(1){
		int newLightValue=0;
		if(followAvoid==0){
			if(primary==bound1){//if the servo is at the lower bound checks to see if 10 degrees to the right of the servo is brighter if it is saves the primary to that angle
				OCR0A=primary+3;
				_delay_ms(255);
				intToLCD(primary+3);
				newLightValue=ADCAquire();
				if(newLightValue<=lightsValue){
					newPrimary=primary+3;
					lightsValue=newLightValue;
				}
			}else if(primary==bound2){//if the servo is at the higher bound checks to see if 10 degrees to the left of the servo is brighter if it is saves the primary to that angle
				OCR0A=primary-3;
				_delay_ms(255);
				intToLCD(primary-3);
				newLightValue=ADCAquire();
				if(newLightValue<=lightsValue){
					newPrimary=primary-3;
					lightsValue=newLightValue;
				}
			}else{//if the servo is not at any of the bounds checks to see if the light is brighter to the left or right and if it is moves the servo to that angle 
				OCR0A=primary+3;
				_delay_ms(255);
				intToLCD(primary+3);
				newLightValue=ADCAquire();
				if(newLightValue<=lightsValue){
					newPrimary=primary+3;
					lightsValue=newLightValue;
				}else{
					OCR0A=primary-3;
					_delay_ms(255);
					intToLCD(primary-3);
					newLightValue=ADCAquire();
					if(newLightValue<=lightsValue){
						newPrimary=primary-3;
						lightsValue=newLightValue;
					}
				}
			}
		}else if(followAvoid==1){//if the servo is at the lower bound checks to see if 10 degrees to the right of the servo is darker if it is saves the primary to that angle
			if(primary==bound1){
				OCR0A=primary+3;
				_delay_ms(255);
				intToLCD(primary+3);
				newLightValue=ADCAquire();
				if(newLightValue>=lightsValue){
					newPrimary=primary+3;
					lightsValue=newLightValue;
				}
			}else if(primary==bound2){//if the servo is at the higher bound checks to see if 10 degrees to the left of the servo is darker if it is saves the primary to that angle
				OCR0A=primary-3;
				_delay_ms(255);
				intToLCD(primary-3);
				newLightValue=ADCAquire();
				if(newLightValue>=lightsValue){
					newPrimary=primary-3;
					lightsValue=newLightValue;
				}
			}else{//if the servo is not at any of the bounds checks to see if the light is darker to the left or right and if it is moves the servo to that angle
				OCR0A=primary+3;
				_delay_ms(255);
				intToLCD(primary+3);
				newLightValue=ADCAquire();
				if(newLightValue>=lightsValue){
					newPrimary=primary+3;
					lightsValue=newLightValue;
				}else{
					OCR0A=primary-3;
					_delay_ms(255);
					intToLCD(primary-3);
					newLightValue=ADCAquire();
					if(newLightValue>=lightsValue){
						newPrimary=primary-3;
						lightsValue=newLightValue;
					}
				}
			}
		}
		
		OCR0A=primary;
		intToLCD(primary);
		_delay_ms(500);
		lightsValue=ADCAquire();
		if(newPrimary!=0){
			primary=newPrimary;
			newPrimary=0;
			OCR0A=primary;
			intToLCD(primary);
			_delay_ms(500);
			lightsValue=ADCAquire();
		}
	}
	
}
void FullSweep(){
	cli();
	ADCSetup();
	int timesIN=0;
	int newValue=bound1;
	primary=bound1;
	cli();
	int newLightValue=0;
	while(newValue<=bound2){//while loop that moves from 0 to 180 degrees
		if(timesIN==0){
			newLightValue=ADCAquire();
			lightsValue=newLightValue;
			timesIN++;
		}else{
			newLightValue=ADCAquire();
		}
		if(followAvoid==0){//if follow the light and the light is greater at that point saves the primary to that point
			if((newLightValue<lightsValue)){
				primary=newValue;
				lightsValue=newLightValue;
			}
		}else if(followAvoid==1){//if avoid the light and the light is lower at that point saves the primary to that point
			if((newLightValue>lightsValue)){
				primary=newValue;
				lightsValue=newLightValue;
			}
		}
		intToLCD(newValue);
		newValue=newValue+6;
		OCR0A=newValue;
		_delay_ms(500);
	}
	OCR0A=primary;
	_delay_ms(500);
	sei();
	Sweep();
}
ISR(PCINT1_vect){ 		//remember this is called on pin change 0->1 and 1->0
	static uint8_t pinBPrev=0b11111111; //for storing previous value of port to detect

	//toggle PORTB0 based on up button status being newly pressed, but not when it is released
	if(((PINB& (1<<6))  == 0) &&((pinBPrev & (1<<6))  != 0)){
		LCD_Init ();
		LCD_WriteDigit('F',0);
		LCD_WriteDigit('T',1);
		LCD_WriteDigit('L',2);
		_delay_ms(500);
		followAvoid=0;
		//toggle PORTB0 based on down button status being newly pressed, but not when it is released
	}else if(((PINB& (1<<7))  == 0) &&((pinBPrev & (1<<7))  != 0)){
		LCD_Init ();
		LCD_WriteDigit('A',0);
		LCD_WriteDigit('T',1);
		LCD_WriteDigit('L',2);
		_delay_ms(500);
		followAvoid=1;
	}

	pinBPrev = PINB; //save button status
}
ISR(PCINT0_vect) 		//remember this is called on pin change 0->1 and 1->0
{
	static uint8_t pinEPrev=0b11111111; //for storing previous value of port to detect

	//toggle PORTE0 based on right button status being newly pressed, but not when it is released
	if(((PINE     & (1<<3))  == 0) &&((pinEPrev & (1<<3))  != 0)){
		LCD_Init ();
		LCD_WriteDigit('F',0);
		LCD_WriteDigit('S',1);
		LCD_WriteDigit('W',2);
		FullSweep();
	}else{
		//do nothing
	}

	pinEPrev = PINE; //save button status
}
void SetupInterrupts(void)
{
	//Setup for UP/Down Button Interrupt
	PCMSK1  |= ((1<<PCINT14)|(1<<PCINT15)); //Unmask bit for UP/Down Button on Butterfly, PB4->PCINT14/PB4->PCINT15 to allow it to trigger interrupt flag PCIF1
	//Setup for Right Button Interrupt
	PCMSK0  |= (1<<PCINT3); //Unmask bit for Right Button on Butterfly, PE3->PCINT3 to allow it to trigger interrupt flag PCIF1
	EIMSK   = ((1<<PCIE1)|(1<<PCIE0));    //Enable interrupt for flag PCIF
}

int main(void){
	//Setup All Pushbuttons
	DDRB  &= ~0b11000000;  //set B4,B6,B7 as inputs
	DDRE  &= ~0b00001100;  //set all to inputs
	PORTB |=  0b11000000;  //enable pull up resistors on B4,B6,B7
	PORTE |=  0b00001100;  //enable pull up resistor on pin E2,E3
	LCD_Init ();
	LCD_WriteDigit('F',0);
	LCD_WriteDigit('T',1);
	LCD_WriteDigit('L',2);
	TCCR0A=0b01101100;//sets the timer to be in fast pwm with non-inverting mode with 256 pre scalar  
	DDRB=(1<<PB4);
	SetupInterrupts();	//setup the interrupts
	sei();//turns on the interrupts 
	OCR0A=bound1;
	ADCSetup();//turns on the light sensor 
	FullSweep();//starts a full sweep 
}

