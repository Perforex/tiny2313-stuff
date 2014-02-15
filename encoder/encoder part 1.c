/*
 * Rotary Encoder controlling PWM duty cycle for Attiny2313.
 * 
 * Part 1 - Fast PWM only
 * 
 * PWM on PB3 pin 15
 * 
 *  
 * 
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>


int main()
{
	DDRB |= (1<<PB3);					// PWM port set to output
	
 	ICR1 = 65534;			    		// Frequency = 8,000,000/ICR1
 	OCR1A = ICR1/2;						// Duty cycle = OCR1A/ICR1 (50%)
 	
 	TCCR1A |= (1<<COM1A1);				// Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at TOP
  	TCCR1A |= (1<<WGM11);	 			// Fast PWM Mode 14 TOP=ICR1
  	TCCR1B |= (1<<WGM12);
  	TCCR1B |= (1<<WGM13);
  	
//  	TCCR1B |= (1<<CS12);				// Prescalers CS12=8,CS11=4,CS10=1
//  	TCCR1B |= (1<<CS11);				// 1=/1,4=/8,3=/64,8=/256,9=/1024
   	TCCR1B |= (1<<CS10);				
 	
	sei(); 
 
 while (1) 
 {
  }	
 }
