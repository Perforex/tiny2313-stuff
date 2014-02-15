/*
 * Fast 16 Bit PWM with rotary encoder controlled duty cycle 
 * 
 * SPB Feb 2014 
 * 
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <debounce.h>

#define CW	PB0
#define ACW PB1
#define PMW PB3
#define SW 	PD5

uint16_t encoder();
uint8_t buttonPress();

uint16_t precision=2;					//Increments per click
uint16_t currentDuty=100;				//Current Duty Value
uint16_t maxDutyValue=200;				//Maximum Duty Value 

uint16_t currentFreq=200;				//Current Frequency Value
uint16_t maxFreqValue=600;		 		//Maximum range

uint8_t frequencyMode=0;

int main()
{
	DDRB |= (1<<PMW);					// PWM port set to output
	DDRD &= ~(1<<SW);					// Switch to input
	PORTD |= (1<<SW);					// Pull up enabled
	
 	ICR1 = currentFreq;					// Frequency = 8,000,000/ICR1 
 	OCR1A = ICR1/2;						// Duty cycle = OCR1A/2 = 50%
 	
 	TCCR1A |= (1<<COM1A1);				// Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at TOP
  	TCCR1A |= (1<<WGM11);	 			// Fast PWM Mode 14 TOP=ICR1
  	TCCR1B |= (1<<WGM12);
  	TCCR1B |= (1<<WGM13);
  	
   	TCCR1B |= (1<<CS10);				
 	
	sei(); 
 
 while (1) 
 {
  /*	 
   if(buttonPress()) frequencyMode = !frequencyMode;
   
   if(frequencyMode)
    {
		max_encoder_value=400;
		currentFreq=encoder();
	}
	else
    {
		max_encoder_value=400;
		currentDuty=encoder();
	}

	ICR1=currentFreq;
	OCR1A=currentDuty;
  }	
  */
	ICR1=currentFreq;
	OCR1A=encoder(maxDutyValue);
 } 
   
 }
uint8_t buttonPress()
{
 if(PIND & (1 << SW))
 {
	debounce();
	if(PIND & (1 << SW)) return(1);
 }	
 return(0);
}	 
uint16_t encoder(uint16_t max_encoder_value)
{
 static uint8_t state=0x03;					//Start with both pins high
 static uint8_t damper;						//Prevents over reporting
 static uint16_t encoder_value=50;			//The value

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
