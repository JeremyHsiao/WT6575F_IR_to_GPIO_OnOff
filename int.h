//=============================================================================//
// Copyright:															       //
//											      	        				   //
//       Copyright(C) 2015                         						   	   //
//	All rights reserved.						      						   //
//									      									   //
// Filename:								      							   //
//	int.h 									      						   //
//									      									   //
// History:								      								   //
//									       									   //
//=============================================================================//
#ifndef _INT_H_
#define _INT_H_

#include <absacc.h>
#include <INTRINS.H>
#include "Port_ARM.h"

extern void InitialEXINT(void);
extern void INIT_TIMER0(void);

extern uint8_t     Counter_8192us;

extern void SetMyTimer_us(uint16_t delay_us_value);
extern Bool CheckMyTimerTimeOut(void);
extern void ClearMyTimer_us();

#endif //#ifndef _INT_H_
