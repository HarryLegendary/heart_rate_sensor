/*Include*/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"  /* derivative information */
#include "SCI.h"

void setClk12(void);
void setATD5(void);
void InitTimer(void);
void OutCRLF(void);
void toggelLED(void);

unsigned short val; 
unsigned int rate;
unsigned int temp;
unsigned short op1;
unsigned short op2;
unsigned short op3;
int time;
unsigned int atPeak;
int i = 0;

void OutCRLF(void){
  SCI_OutChar(CR);
  SCI_OutChar(LF);
  //PTJ ^= 0x20;          // toggle LED D2
}

void toggelLED(void){
	DDR1AD ^= 0b11011111;
	DDR0AD ^= 0xff;  
}

void main(void) { 
  
  setClk12(); 
	DDRJ = 0x01;
	DDR1AD = 0x00;
	DDR0AD = 0x00;  
	PTJ = 0x01; 
	SCI_Init(38400);
	setATD5();
	InitTimer();
	EnableInterrupts;
	temp = 70;
	rate = 70;
	time = 1000;
	
	for(;;) {//infinite for loop
	}
} 



#define PERIOD 24000 //interrupt every 2ms
void InitTimer(void){
	//bus speed = 14MHz
	TIOS = 0xfd;//channel 1 as input capture,channel 0 as output compare(1111 1101)
	PERT = 0x02;//pull up resistors for channel 1
	TIE = 0x02;
	TSCR1 = 0x90;
	TSCR2 = 0x00;
	
	TCTL2 &= 0xfc;//no external output
	TCTL3 = 0x00;//falling edge on channel 1
	TCTL4 = 0x08;
	
	TC0 = PERIOD;
}


interrupt  VectorNumber_Vtimch1 void ISR_Vtimch1(void){//detect external falling edge
	unsigned int temp;
	toggelLED();
	TIE ^= 0x01;      //Toggles pin/LED state    (enable 2ms interrupt)
	temp = TC1;       //Fast clear interrupt flag by reading from TC1
	//i = 0;
	
}

interrupt  VectorNumber_Vtimch0 void TC0_ISR(void){//output compare, interrupt at each 1ms
	TFLG1_C0F = 0x01;
	TC0 = TC0 + PERIOD;
	val = ATDDR0;
	if(val>204){
		atPeak = 1;
	} 
	else if(atPeak == 1){
		temp = 30000/time;
		if(temp>0 && temp<200){
	    rate = temp;
	  }
		atPeak = 0;
		time = 0;
		
		
	}
    op1 = rate/100;
	  op2 = rate/10 - op1*10;
	  op3 = rate - 10*op2 - 100* op1;
		
  	PT1AD = op2*2+op1;
	  PT0AD = op3;
	if(i == 25){
	  SCI_OutUDec(val);
		OutCRLF();
	  //PTJ ^= 0x01;
		
 	 /*	SCI_OutUHex(PT1AD);
		SCI_OutString("<--PT1AD  PT0AD-->");
		SCI_OutUHex(PT0AD);
		OutCRLF();
		SCI_OutUDec(rate);
		OutCRLF();*/
		
		
		i = 0;
	}
	i++;
	time++;
}

void setClk12(void){
	CPMUREFDIV = 0x03;
	CPMUSYNR = 0x05;
	CPMUPOSTDIV = 0x00;
	CPMUCLKS = 0x80;
	CPMUOSC = 0x80;
	while(!(CPMUFLG & 0x08));
}

void setATD5(void){
	ATDCTL1 = 0x2f;
	ATDCTL3 = 0x88;
	ATDCTL4 = 0x66;
	ATDCTL5 = 0x25;	
}