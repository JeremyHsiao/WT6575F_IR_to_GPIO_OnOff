						//=============================================================================//
// Copyright:															       //
//											      	        				   //
//       Copyright(C) 2008                          						   //
//	All rights reserved.						      						   //
//									      									   //
// Filename:								      							   //
//	MAIN.C 									      							   //
//									      									   //
// History:								      								   //
//									       									   //
//=============================================================================//
#include <absacc.h>
#include <REG52.H>
#include <INTRINS.H>
#include "Port_ARM.h"
#include "MEMALLOC.h"
#include "XFR.h"
#include "MAIN.h"
#include "buffer.h"
#include "int.h"
#include "automata_common.h"

									  //
// In order to optimize assembly code of Int0Function()
// 	Push_Rx_Width() of buffer.c is directly integrated into Int0Function()
//

//
// In order to optimize assembly code of Int1Function()
// 	FUN_INT() of usb.c is directly integrated into Int0Function()
//

//**************************************************
// 8x52 INT1 FUNCTION(USB ENDPOINT INTERRUPT) 
//**************************************************
void Int1Function(void)	interrupt 2		
{
}


// Jeremy Added
void InitialEXINT(void)
{
//	EXTINTEN=(1<<2);	//Enable PA2 EXINT function
	EXTINTEN=(1<<4);	//Enable PA4 EXINT function
	EX0=1;
//	EA=1;
}

void INIT_TIMER0(void)
{
	TMOD = (TMOD&0xf0) | 0x01;		//Set Timer 0 to 16-bit timer
//	TH0=(RELOAD_CNT)/256;	//
//	TL0=(RELOAD_CNT)%256;	// 1000us/ 0.125us=8000
	TL0 = 0;
	TH0 = 0;
  Counter_8192us = 0;
 	TR0=1;			//Timer 0 Start
	ET0=1;			//Enable Timer 0 interrupt
}
// Jeremy Added

#ifdef RC6_SUPPORT
static Bool	RC6_Timeout_Timer_Enable = FALSE;
uint16_t   MyTimerValue_value=0xffff;

void SetMyTimer_us(uint16_t delay_us_value)
{
    RC6_Timeout_Timer_Enable = TRUE;
    MyTimerValue_value = delay_us_value;
}

Bool CheckMyTimerTimeOut()
{
  uint16_t current_timer;
  
  if(RC6_Timeout_Timer_Enable==FALSE)
    return FALSE;
  
  BYTELOW(current_timer) = TH0;
  BYTEHIGH(current_timer) = Counter_8192us;
  if(current_timer>MyTimerValue_value)
    return TRUE;
  else
    return FALSE;
}

void ClearMyTimer_us()
{
	RC6_Timeout_Timer_Enable = FALSE;
  MyTimerValue_value = 0xffff;
}
#endif // RC6_SUPPORT

//**************************************************
// Timer0 Interrupt 
//**************************************************
uint8_t     Counter_8192us=0;

void Timer0Funtion(void)	interrupt 1
{
	EX0=0;
 
  if((Counter_8192us&0x80)==0)
	{
	  Counter_8192us++;
	}
  else
  {
    Counter_8192us=0;
//    P1^=(1<<3);			//if EXINT, toggle P1(PB) for testing
  }
	EX0=1;
}

//**************************************************
// External Interrupt 0 
//**************************************************
void Int0Function(void)	interrupt 0
{
	// Real External Int 0, not USB
	if(USBFI2&EXINT)
	{
    TR0 = 0;
		USBFI2=~EXINT;
//		P1^=(1<<3);			//if EXINT, toggle P1(PB) for testing
//    BYTELOW(temp_timer) = TH0;
//		BYTEHIGH(temp_timer) = Counter_8192us;
	  {
    	uint8_t temp_index;
    
    	temp_index = in_index;
    	if(temp_index)
    	{
    	  temp_index--;
    	}
    	else
    	{
    	  temp_index=PULSE_BUF_MAX-1;
    	}	   
    
    	if( temp_index == out_index )		// Buffer Overrun
    	{
    		//putstr("IR_Transmit_Buffer_Push_overrun\r\n");
    		temp_index=0;
    	}
    	else
    	{
        BYTELOW(pulse_data[in_index]) = TH0;
        BYTEHIGH(pulse_data[in_index]) = Counter_8192us;
//   	    pulse_data[in_index] = temp_timer;
    		in_index = temp_index;
    	}
	  }
		TL0 = 0;			    
		TH0 = 0;
		Counter_8192us = 0;
    TR0 = 1;
	}
  else
  {
	  uint16_t	temp_timer;
    temp_timer = 0;
  }

}


