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
#ifndef _MAIN_H_
#define _MAIN_H_

#include <absacc.h>
#include <INTRINS.H>
#include "Port_ARM.h"

//=====================================================================
// Variable
//=====================================================================

extern unsigned char data	USBFI2_FLAG;	
extern unsigned char data	FUN_STATUS;
extern unsigned char data  KEY_PRE, KEY_NOW;
extern unsigned char data  EP1_DATA_BUFFER[8];
extern unsigned char data  EP2_DATA_BUFFER[8];
extern bit	FUN_DEVICE_REMOTE_WAKEUP;
#ifdef LED
extern unsigned char data 	KBD_LED;
#endif
extern bit USB_SUSPEND;

#endif // #ifndef _MAIN_H_
