//=============================================================================//
// Copyright:															       //
//											      	        				   //
//       Copyright(C) 2015                         						   	   //
//	All rights reserved.						      						   //
//									      									   //
// Filename:								      							   //
//	buffer.h 									      						   //
//									      									   //
// History:								      								   //
//									       									   //
//=============================================================================//
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <absacc.h>
#include <INTRINS.H>
#include "Port_ARM.h"

//=====================================================================
// Variable
//=====================================================================

#define	PULSE_BUF_MAX		20
#define PULSE_DATA_AREA		data
extern uint16_t PULSE_DATA_AREA pulse_data[PULSE_BUF_MAX];
extern uint8_t in_index;
extern uint8_t out_index;

extern Boolean Push_Rx_Width(uint16_t width);
extern Boolean Pop_Tx_Width(uint16_t *width);


#endif //#ifndef _BUFFER_H_
