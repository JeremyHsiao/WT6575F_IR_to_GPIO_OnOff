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

//=====================================================================
// Variable
//=====================================================================

uint16_t PULSE_DATA_AREA pulse_data[PULSE_BUF_MAX];
uint8_t in_index=0;
uint8_t out_index=0;

/*
Boolean Push_Rx_Width(uint16_t width)
{
  	Boolean blRet;
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
		blRet = FALSE;
	}
	else
	{
	    pulse_data[in_index] = width;
		in_index = temp_index;
		blRet = TRUE;
	}
	return blRet;
}
*/

Boolean Pop_Tx_Width(uint16_t *width)
{
  	Boolean blRet;

	if( out_index != in_index )		// Buffer Overrun
    {
	   *width = pulse_data[out_index];
	   if(out_index>0)
	   {
           EX0=0;
	       out_index--;
           EX0=1;
	   }
	   else
       {
           EX0=0;
	       out_index=PULSE_BUF_MAX-1;
           EX0=1;
	   }	   
	   blRet = TRUE;
	}
	else
	{
	   blRet = FALSE;
	}

	return blRet;
}

