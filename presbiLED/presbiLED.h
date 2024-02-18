#ifndef __LED2017RF_H__
#define __LED2017RF_H__

#ifdef __SDCC	//---------------------------SDCC
#define _nop_() __asm \
nop \
__endasm

#define code __code
#include "STC15F2K60S2.h"

__sbit __at (0xb1) WS2812_Data;

#else	//-----------------------------------Keil
#include <REG52.h>
#include <intrins.h>

sbit WS2812_Data = P3 ^ 1;

sfr AUXR=0x8E;
sfr T2H=0xD6;
sfr T2L=0xD7;

sfr IAP_DATA    =   0xC2;   
sfr IAP_ADDRH   =   0xC3;   
sfr IAP_ADDRL   =   0xC4;   
sfr IAP_CMD     =   0xC5;   
sfr IAP_TRIG    =   0xC6;   
sfr IAP_CONTR   =   0xC7;   
#endif

#define FOSC 33176600L
#define T2_MS (65536U-FOSC/1000U)
#define MAXLED	144
#define MAXSTRIPS 10

extern int numLed;
extern int t0;
extern unsigned long prefix;

typedef unsigned char BYTE;
typedef unsigned short WORD;

#define TRUE (-1)
#define FALSE (0)

typedef int BOOL;
typedef unsigned char BYTE;

BOOL WriteCfg(void);
void ReadCfg(void);
void DumpByte(BYTE);
#endif