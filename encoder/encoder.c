/*
 * Fast 16 Bit PWM with rotary encoder controlled duty cycle 
 * The push button turns the PWM on or off (no output).
 * 
 * SPB Feb 2014 
 * 
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <debounce.h>

#define CW	PB0							// Clockwise pin from encoder
#define ACW PB1							// Anticlockwise pin from encoder
#define PMW PB3							// PWM output pin
#define DBG PD3							// Debug LED
#define INT PD2							// Push button from encoder	

volatile uint8_t buttonPress=0;

uint16_t encoder();

uint16_t precision=1;					//Increments per click
uint16_t currentDuty;					//Current Duty Value
uint16_t maxDutyValue=102;				//Maximum Duty Value (102 gets to 100% duty) 
  
uint16_t currentFreq=100;				//Current Frequency Value

uint8_t On=0;							// Off / On toggle

int main()
{
	DDRB |= (1<<PMW);					// PWM port set to output
	DDRD |= (1<<DBG);					// Debug LED to output
	DDRD &= ~(1<<INT);					// Ensure interrupt pin is input

	currentDuty=maxDutyValue/10;		// Start at 10% duty cycle
	
 	ICR1 = currentFreq;					// Frequency = 8,000,000/ICR1 
 	OCR1A = currentDuty;				// Duty cycle
 	
 	TCCR1A |= (1<<COM1A1);				// Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at TOP
  	TCCR1A |= (1<<WGM11);	 			// Fast PWM Mode 14 TOP=ICR1
  	TCCR1B |= (1<<WGM12);
  	TCCR1B |= (1<<WGM13);
  	
   	TCCR1B |= (1<<CS10);				// Scalar = 1
   	
   	MCUCR |= (1<<ISC00);				// Trigger int0 on rising edge
   	MCUCR |= (1<<ISC01);				
   	GIMSK |= (1<<INT0);					// Enable int0
	sei(); 
 
 while (1) 
 {
	if(buttonPress)
	 {
		 On=!On;
		 buttonPress=0;
		 PORTD ^= (1<<DBG); 
	 }	 
    if(!On)
     {
      OCR1A=0;
      DDRB &= ~(1<<PMW);				// Turn off PWM
     } 
    else 
     {
	  currentDuty=encoder(maxDutyValue,currentDuty);
	  OCR1A=currentDuty;
      DDRB |= (1<<PMW);					// PWM port set to output (turned on)
	 }
 }	
} 
uint16_t encoder(uint16_t max_encoder_value,uint16_t encoder_value)
{
 static uint8_t state=0x03;					//Start with both pins high
 static uint8_t damper;						//Prevents over reporting
 
 state <<=2;								//Save previous state
 if(PINB & (1 << ACW)) state |=(1<<0);		//Port 1
 if(PINB & (1 << CW)) state |=(1<<1); 		//Port 2
 state &= 0x0f;								//Remove high order nibble
 if(damper)
  {if(state!=15) return(encoder_value); 	//Prevent over reporting
   else damper=0;}

 if(state==0x0e && encoder_value < (max_encoder_value - precision))
  {
   encoder_value += precision;
   damper=1;
  }
 if(state==0x0d && encoder_value > precision)
  {
   encoder_value -= precision;
   damper=1;   
  } 
 return(encoder_value); 
} 
ISR(INT0_vect) 
{
	buttonPress=1;
	debounce();						// 20ms delay
	debounce();						// 20ms delay
}	



