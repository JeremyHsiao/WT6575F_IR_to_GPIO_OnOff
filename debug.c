#include <absacc.h>
#include <REG52.H>
#include <INTRINS.H>
#include "MEMALLOC.h"

//=================================================================
//	Initial UART
//=================================================================
//-----------------------------------------------------------------------
//	Baud Rate = 230400 , SMOD = 1, x6 = 1, TH1 = (256-13)
//	Baud Rate = 115200 , SMOD = 1, x6 = 1, TH1 = (256-26)
//	Baud Rate = 38400 , SMOD = 1, TH1 = (256-13)
//	Baud Rate = 19200 , SMOD = 0, TH1 = (256-13)
//	Baud Rate = 9600 , SMOD = 0, TH1 = (256-26)
//-----------------------------------------------------------------------
#define BR_230400		//SET The Baud Rate as 230400 bps
void INIT_UART(void)
{
#ifdef BR_9600
	TMOD=0x20 | (TMOD&0x0f); 			//Use Timer1 Mode2 to CLK
	TL1=(256-26); 		//(256-26)..(3T) Baud Rate = 9600  
	TH1=(256-26);		//
	//PCON=0x00; 			//SMOD=0 ; x6=0
	PCON&=0x3F;			//SMOD=0 ; x6=0

#endif
#ifdef BR_19200
	TMOD=0x20 | (TMOD&0x0f); 			//Use Timer1 Mode2 to CLK
	TL1=(256-13); 		//(256-13)..(3T) Baud Rate = 19200  
	TH1=(256-13);		//
	//PCON=0x00; 			//SMOD=0 ; x6=0
	PCON&=0x3F;			//SMOD=0 ; x6=0
#endif
#ifdef BR_38400
	TMOD=0x20 | (TMOD&0x0f); 			//Use Timer1 Mode2 to CLK
	TL1=(256-13); 		//(256-13)..(3T) Baud Rate = 38400  
	TH1=(256-13);		//
//	PCON=0x80; 			//SMOD=1 ; x6=0
	PCON&=0x3F;	
	PCON|=0x80; 		//SMOD=1 ; x6=0	
#endif
#ifdef BR_115200
	TMOD=0x20 | (TMOD&0x0f); 			//Use Timer1 Mode2 to CLK
	TL1=(256-26); 		//(256-26)..(3T) Baud Rate = 115200  
	TH1=(256-26);		//
//	PCON=0xC0; 			//SMOD=1 ; x6=1
	PCON|=0xC0; 		//SMOD=1 ; x6=1
#endif
#ifdef BR_230400
	TMOD=0x20 | (TMOD&0x0f); 			//Use Timer1 Mode2 to CLK
	TL1=(256-13); 		//(256-13)..(3T) Baud Rate = 230400  
	TH1=(256-13);		//
//	PCON=0xC0; 			//SMOD=1 ; x6=1
	PCON|=0xC0; 		//SMOD=1 ; x6=1
#endif

	TR1=1; 				//Start to count
	SCON=0x70;		 	//8-bit UART (Mode 1) & receive enable
//	EA=1;
//	ES=1;				//Enable UART Interrupt
	RI=TI=1;
}