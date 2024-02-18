//!make
#include "STC15F2K60S2.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "ws2812b.h"

#define BV(x) (1<<(x))
#define FOSC 33177600
#define _nop_() __asm \
nop \
__endasm

#define NUM_LED	10

#define BIT_DURATION		98	//*10us
#define ONE_DURATION	69
#define ZERO_DURATION	39

//TODO: wrap contatore ticks
#define checkDuration(a,b,c) (abs((a-b)-(c))<(c)/8)

volatile unsigned long ticks=0;

void Delay104us(void) {	//@33.1776MHz
	unsigned char __data i, j;

	_nop_();
	_nop_();
	i = 4;
	j = 87;
	do
		while (--j);
	while (--i);
}

void sendByte(uint8_t b) {	//@9600baud
	EA=0;
	P33=0;
	Delay104us();	//start bit
	
	for (int i=0; i<8; i++,b>>=1) {
		P33=b&1;
		Delay104us();
	}
	
	P33=1;
	Delay104us();	//stop bit
	EA=1;
}

void sendString(char *s) {
	while (*s)
		sendByte(*s++);
}

void timer0_int (void) __interrupt(1) {
	ticks++;
}

void initTimer0(void) {		//10μs @ 35MHz
	AUXR |= 0x80;			//Timer clock is 1T mode
	TMOD &= 0xF0;		//Set timer work mode
	TL0 = 0xB4;			//Initial timer value
	TH0 = 0xFE;			//Initial timer value
	TF0 = 0;				//Clear TF0 flag
	TR0 = 1;				//Timer0 start run
	ET0=1;
	EA=1;
}


void setAllLEDs(int r, int g,int b) {
	EA=0;
	for (int i=0;i<NUM_LED;i++)
		WS2812_SendRGBData(r,g,b);
	WS2812_Latch();
	EA=1;
}

void lightShow(void) {
	int i;
	
	for (i=0;i<10;i++) {
		setAllLEDs(0,255,0);
		ticks=0;
		while (ticks<20000UL);
		
		setAllLEDs(255,0,0);
		ticks=0;
		while (ticks<20000UL);
		
		setAllLEDs(0,0,255);
		ticks=0;
		while (ticks<20000UL);
		
		setAllLEDs(0,0,0);
		ticks=0;
		while (ticks<20000UL);
	}
}

void main(void) {
	int t0,t1,t2,nBit=0;
	unsigned val=0;
	
	setAllLEDs(0,1,0);
	for (int i=0;i<3000;i++)
		Delay104us();
	setAllLEDs(0,0,0);

	initTimer0();	
	while (P35);
	t2=ticks;

	while (true) {
		t0=t2;
		while (!P35);
		t1=ticks;
		while (P35);
		t2=ticks;
		if (checkDuration(t2,t0,BIT_DURATION)) {
			if (checkDuration(t2,t1,ONE_DURATION))
				val|=1;
			else if (!checkDuration(t2,t1,ZERO_DURATION)) {
				val=nBit=0;
				continue;
			}
			if (++nBit==12) {
				//~ sendByte(val);
				//~ sendByte(val>>8);
				if (val==0x0481)
					lightShow();
				val=nBit=0;
				continue;
			}
			else
				val<<=1;
		}
		else
			val=nBit=0;
	}
}