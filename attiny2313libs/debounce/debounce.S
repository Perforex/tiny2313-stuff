; Debounce delay routine.
;
; A 20 millisecond delay when called from an 8Mhz processor which is
; intended for a switch debounce delay.
; 
; Use only registers r18 - r27 as these are "call-used" from C which is 
; where this routine is intended to be called from :
;
; #include <debounce.h>
; debounce();
;
; Timing calculation explanation is at the end of the code
;
; SPB Jan 2014
;
.NOLIST
.LIST
#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0 
#include <avr/io.h>
.global debounce
.section .text                  		; Defines a code section	
debounce:
	ldi	r18,175							; 1 Load R18 outer loop counter
outerloop:
;-> Outer
	ldi r19,130							; 1 load R19 inner loop counter
innerloop:
;-----> Inner
		nop									; 1 Timing alignment
		nop									; 1 Timing alignment
		nop									; 1 Timing alignment
;
		subi	r19,1						; 1 Decrement inner counter
		cpi		r19,0						; 1 Compare to zero
		brne	innerloop					; 1 F, 2 T Branch if not equal
;-----> Inner 
	subi	r18,1						; 1 Decrement outer counter
	cpi		r18,0						; 1 Compare to zero
	brne	outerloop					; 1 F, 2 T Branch if not equal
;-> Outer
;Adjust {
	ldi 	r20,12						; 1 Load adjust counter
	nop									; 1 Alignment
adjust:
	subi	r20,1						; 1 Decrement
	cpi		r20,0						; 1 Compare to zero
	brne	adjust						; 1 F, 2 T
;} 
	ret									; 1 Return
;
; Timing explanation
;
; The inner loop size is 7 cyles for (counter - 1) loops and 6 cycles on the last loop  
; (129 x 7) + 6 = 909 cycles
; 
; The outer loop size is 5 + inner loop for (counter - 1) loops and 4 + inner loop on the last loop
; ((5 + 909) x 174) + (4 + 909) = 159,949
;
; Initial ldi and ret = 2
; Adjust routine is 1 + 1 + (12 x 4) + 3 = 49
;
; Total cycles = 160,000
;
; Each cycle is 0.125 micro secs so total time = 20,000 micro seconds or 20 milli seconds
;

 





