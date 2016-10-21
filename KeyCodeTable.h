// Copyright:															       //
//											      	        				   //
//       Copyright(C) 2008                          						   //
//	All rights reserved.						      						   //
//									      									   //
// Filename:								      							   //
//	KeyCodeTable.h 									      					   //
//									      									   //
// History:								      								   //
//									       									   //
//=============================================================================//
#ifndef _KEY_CODE_TABLE_H_
#define _KEY_CODE_TABLE_H_

#include <INTRINS.H>
#include "Port_ARM.h"

extern const uint8_t  code RC6_LUT[];
extern const uint16_t code RC6_LUT_SIZE;
extern const uint8_t  code RC6_MODE; 
extern const uint8_t  code RC6_CONTROL; 

extern const uint8_t  code NEC_LUT[];
extern const uint16_t code NEC_LUT_SIZE;
extern const uint8_t  code NEC_CUSTOM_CODE_0; 
extern const uint8_t  code NEC_CUSTOM_CODE_1; 
extern const uint8_t  code POWER_ON_KEY_CODE; 
extern const uint8_t  code POWER_OFF_KEY_CODE; 

extern const uint8_t  code NEC_FACTORY_LUT[];
extern const uint16_t code NEC_FACTORY_LUT_SIZE;
extern const uint8_t  code NEC_FACTORY_CUSTOM_CODE_0; 
extern const uint8_t  code NEC_FACTORY_CUSTOM_CODE_1; 

#endif // _KEY_CODE_TABLE_H_
