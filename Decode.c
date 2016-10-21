//=============================================================================//
// Copyright:															       //
//											      	        				   //
//       Copyright(C) 2008                          						   //
//	All rights reserved.						      						   //
//									      									   //
// Filename:								      							   //
//	Decode.c 									      							   //
//									      									   //
// History:								      								   //
//									       									   //
//=============================================================================//
#include <absacc.h>
#include <REG52.H>
#include <INTRINS.H>
#include "MEMALLOC.h"

#define DEBUG_GENERAL
#define DEBUG_ERROR_MSG
//#define DEBUG_GETKEY
//#define DEBUG_DECODER

#include "Port_ARM.h"

#include <stdio.h>
//#include "debug.h"
#include "automata_common.h"
#include "int.h"
#include "Decode.h"
#include "KeyCodeTable.h"

uint8_t uart_index=0;
char idata uart_buffer[70];
void MyUARTputchar(char input_char)
{
  uart_buffer[uart_index++]=input_char;
  if(uart_index>=70)
  {
    uart_index=0;
  }
}

#ifdef _TOUCH_RC_ENABLE

BOOL bTouchRCHasValue;
int return_x, return_y;
BOOL button_0, button_1;
unsigned char BUTTON_NOW=0x03;
unsigned char Delta_X=0,Delta_Y=0; //,ZAXIS=0; 
bit USB_SEND_F1=0, USB_SEND_F2=0; 

//
// CRC4
//
U8 calc_crc4( U32 nData )
{
	U32 mask = 0x00800000L;
	U32 x4_x_1 = 0x00980000L;
	U8 nValidBit = 20;

	while( nValidBit-- != 0 )
	{
		if( (nData & mask) != 0 )
		{
			nData = nData ^ x4_x_1;
		}
		x4_x_1 >>= 1;
		mask >>= 1;
	}
	return (U8)(nData & 0x0f);
}

int Convert_uint_to_int(unsigned int uint_val)
{
	int ret_val;

	// uint_val is limited to 10 bits long
	if ((uint_val&0x200)!=0)
	{
		ret_val = (uint_val&0x3ff) - 0x400;
	}
	else
	{
		ret_val = uint_val & 0x1ff;
	}
	return ret_val;
}

//
// Key Value Decoder
//
extern U32 _u32TouchRCData, _u32TouchRCDataReverse;

BOOL _MDrv_IR_GetKey_TouchRC(void)
{
	BOOL bRet = FALSE;
	U32	temp_index;
	U32	temp_value, temp_value_reverse;
    int return_x, return_y;
    BOOL button_0, button_1;
	{
		temp_value = _u32TouchRCDataReverse;
		temp_value_reverse = _u32TouchRCData;

		temp_value >>=1 ; // Remove leading code at LSB
		temp_value_reverse &= 0x0fffffff; // Mask out leading code at MSB

		// The following CRC check is for TouchRC
		temp_index = calc_crc4(temp_value&0xffffff);
		if( temp_index != ((temp_value>>24)&0x0f) )
		{
//			UARTputcharErr('E');
			bRet = FALSE;
		}
		else
		{
			// Passed CRC4 check, covert the mouse value

			// Touch RC
			return_x = Convert_uint_to_int((temp_value)&0x3ff);
			return_y = Convert_uint_to_int((temp_value>>10)&0x3ff);
			// END

			button_0 = (temp_value&(1L<<22))!=0?TRUE:FALSE;			// "Back"?
			button_1 = (temp_value&(1L<<23))!=0?TRUE:FALSE;			// "OK"?

			UARTprintfDebug("%d:", return_x);
			UARTprintfDebug("%d:", return_y);

			if(button_0)
			{
			  UARTputcharDebug('+');
			}
			else
			{
			  UARTputcharDebug('-');
			}			
			if(button_1)
			{
			  UARTputcharDebug('+');
			}
			else
			{
			  UARTputcharDebug('-');
			}			
			UARTputcharDebug('\n');


			//BUTTON_PRE=(temp_value>>22)&0x03;
			// BUTTON_PRE = (temp_value_reverse>>4)&0x03;
//			BUTTON_NOW = (temp_value_reverse>>22)&0x03;		//  (temp_value>>4)&0x03;

/*
			BUTTON_NOW = (temp_value_reverse>>4)&0x03;		//  (temp_value>>4)&0x03;
			BUTTON_NOW = BUTTON_NOW <<1;
			if (BUTTON_NOW & 0x04) BUTTON_NOW |= 0x01;
			BUTTON_NOW = BUTTON_NOW &0x03;
*/
			if ( (return_x==0x1f8)&&(return_y==0x1f8) )
			{
			  USB_SEND_F2 = TRUE;
			  USB_SEND_F1 = FALSE;
			  UARTputstrGetKey("Centered\n");
			  return TRUE;
			}

			if(return_x>0x7f)
			{
				return_x=0x7f;
			}
			else if (return_x<-127)
			{
				return_x = -127; 
			}

			if(return_y>0x7f)
			{
				return_y=0x7f;
			}
			else if (return_y<-127)
			{
				return_y = -127; 
			}

			Delta_X = (return_x)&0xff; 
			Delta_Y = (return_y)&0xff; 
			BUTTON_NOW = (button_0?1:0) | (button_1?2:0);

			USB_SEND_F1 = TRUE;
			bRet = TRUE;
		}
	}
	return bRet;
}

///
///	Touch RC Decoder
///

BOOL 	bTouchRCHasValue = FALSE;
U32 	_u32TouchRCData = 0, _u32TouchRCDataReverse = 0;
Bool	blNeedToRestoreLeadingBit = FALSE;

void TouchRC_Decoder_Single_Edge(U16 u32IrCounter)
{
	static U8 						_u8TouchRCBit = 0;
	static TouchRCParityType 		_TouchRCParity = TOUCHRC_PARITY_N;
	static TouchRCFrameHalfIdx 		_TouchRCFrameHalf = TOUCHRC_1ST_FRAME_HALF;
	static TouchRCState 			_TouchRCState = TOUCHRC_IDLE;
	static TouchRCDataState 		_TouchRCDataState = TOUCHRC_DATA_IDLE;
	       TouchRCSignal			TouchRCSignal=TOUCHRC_SIGNAL_NULL;


	// Determine pulse-width
 	if(IR_PULSE_2_LB < u32IrCounter && u32IrCounter <= IR_PULSE_2_HB)
	{
		TouchRCSignal=TOUCHRC_2T;
//		UARTputcharDebug('2');
	}
	else if(IR_PULSE_3_LB < u32IrCounter && u32IrCounter <= IR_PULSE_3_HB)
	{
		TouchRCSignal=TOUCHRC_3T;
//		UARTputcharDebug('3');
	}
	else if(IR_PULSE_4_LB < u32IrCounter && u32IrCounter <= IR_PULSE_4_HB)
	{
		TouchRCSignal=TOUCHRC_4T;
//		UARTputcharDebug('4');
	}
  	else if(u32IrCounter > IR_NO_TOUCH_SIGNAL_TIMEOUT)
   	{
   		_TouchRCState = TOUCHRC_IDLE;
//   		UARTputcharDebug('I');
   	}
   	else if(u32IrCounter > IR_LEADING_TIME_LB)	// Header
	{
		_TouchRCState = TOUCHRC_LEADER_BIT;
//		UARTputcharDebug('L');
	}
	else
	{

		//UARTputchar('E');
		TouchRCSignal = TOUCHRC_SIGNAL_NULL;
		if(_TouchRCState == TOUCHRC_IDLE)
		{
			return;
		}
		else
		{
			_TouchRCState = TOUCHRC_FAIL;
		}
	}

	// Determine what is next state	(whole decoding)
	if(_TouchRCState == TOUCHRC_IDLE)
	{
		_u32TouchRCData = 0;
		_u32TouchRCDataReverse = 0;
		_u8TouchRCBit = 0;
		bTouchRCHasValue = FALSE;

		_TouchRCState = TOUCHRC_DATA;
		_TouchRCDataState = TOUCHRC_DATA_INSERT;
		_TouchRCFrameHalf = TOUCHRC_1ST_FRAME_HALF;
		TouchRCSignal = TOUCHRC_LEADING_SIGNAL;
		_TouchRCParity = TOUCHRC_PARITY_N;
		blNeedToRestoreLeadingBit = FALSE;
		//UARTputchar('i');
	}
	else if(_TouchRCState == TOUCHRC_LEADER_BIT)
	{
		if(blNeedToRestoreLeadingBit==TRUE)
		{
			blNeedToRestoreLeadingBit = FALSE;
			_u32TouchRCData = 0x01;
			_u32TouchRCDataReverse = (1L<<(TOUCHRC_TOTAL_LEN-1));
			_u8TouchRCBit = 1;
			_TouchRCState = TOUCHRC_DATA;
			_TouchRCDataState = TOUCHRC_DATA_INSERT;
			_TouchRCFrameHalf = TOUCHRC_1ST_FRAME_HALF;
			bTouchRCHasValue = FALSE;

			// Adopted from below -- else if(_TouchRCState == TOUCHRC_DATA)
			switch(TouchRCSignal)
			{
				case TOUCHRC_2T:
						_TouchRCParity = TOUCHRC_PARITY_N;
					break;
				case TOUCHRC_3T:
						_TouchRCParity = TOUCHRC_PARITY_P;
						_TouchRCFrameHalf = TOUCHRC_2ND_FRAME_HALF;
					break;
				default:
					_TouchRCState = TOUCHRC_FAIL;
					break;
			}
			//UARTputchar('j');
		}
		// Check if one last bit (1) is missing
		else if((_u8TouchRCBit == (TOUCHRC_TOTAL_LEN-1))&&(_TouchRCFrameHalf!=TOUCHRC_1ST_FRAME_HALF))
		{
			_u32TouchRCData = ((_u32TouchRCData<<1) | 0x01);
			_u32TouchRCDataReverse = ((_u32TouchRCDataReverse>>1) | (1L<<(TOUCHRC_TOTAL_LEN-1)));
			_TouchRCDataState = TOUCHRC_DATA_IDLE;
			_u8TouchRCBit = 0;
			bTouchRCHasValue = TRUE;
			blNeedToRestoreLeadingBit = TRUE;
#ifdef CORE_M0			// Testing on M0 board
			UARTputchar('*');
			UART_Show_CPU_Timer();
#endif // #ifdef CORE_M0			// Testing on M0 board
			return;		// No need to be further processed.
		}
		else
		{
			// Same as if(_TouchRCState == TOUCHRC_IDLE) {}
			_u32TouchRCData = 0;
			_u32TouchRCDataReverse = 0;
			_u8TouchRCBit = 0;
			bTouchRCHasValue = FALSE;

			_TouchRCState = TOUCHRC_DATA;
			_TouchRCDataState = TOUCHRC_DATA_INSERT;
			_TouchRCFrameHalf = TOUCHRC_1ST_FRAME_HALF;
			TouchRCSignal = TOUCHRC_LEADING_SIGNAL;
			_TouchRCParity = TOUCHRC_PARITY_N;
			//blNeedToRestoreLeadingBit = FALSE;
			//UARTputchar('k');
		}
	}
	else if(_TouchRCState == TOUCHRC_DATA)
	{
		//UARTputchar('d');
		switch(TouchRCSignal)
		{
			case TOUCHRC_2T:
				if(TOUCHRC_1ST_FRAME_HALF == _TouchRCFrameHalf)
				{
					_TouchRCParity = TOUCHRC_PARITY_N;
				}
				else
				{
					_TouchRCParity = TOUCHRC_PARITY_P;
				}
				_TouchRCDataState = TOUCHRC_DATA_INSERT;
				break;
			case TOUCHRC_3T:
				if(TOUCHRC_1ST_FRAME_HALF == _TouchRCFrameHalf)
				{
					_TouchRCParity = TOUCHRC_PARITY_P;
					_TouchRCFrameHalf = TOUCHRC_2ND_FRAME_HALF;
				}
				else
				{
					_TouchRCParity = TOUCHRC_PARITY_NN;
					_TouchRCFrameHalf = TOUCHRC_1ST_FRAME_HALF;
				}
				_TouchRCDataState = TOUCHRC_DATA_INSERT;
				break;

			case TOUCHRC_4T:
				if(TOUCHRC_1ST_FRAME_HALF == _TouchRCFrameHalf)
				{
					_TouchRCDataState = TOUCHRC_DATA_RESET;
				}
				else
				{
					_TouchRCParity = TOUCHRC_PARITY_NP;
					_TouchRCDataState = TOUCHRC_DATA_INSERT;
					//_TouchRCFrameHalf = TOUCHRC_2ND_FRAME_HALF;
				}
				break;

			case TOUCHRC_LEADING_SIGNAL:
				_TouchRCDataState = TOUCHRC_DATA_INSERT;
				break;

			default:
				_TouchRCState = TOUCHRC_FAIL;
				break;
		}
	}

	// Process Data insertion
	if(_TouchRCState == TOUCHRC_DATA)
	{
		if(_TouchRCDataState == TOUCHRC_DATA_INSERT)
		{
			if(_u8TouchRCBit < (TOUCHRC_TOTAL_LEN))	 	// still bit to be filled?
			{
				if(_TouchRCParity == TOUCHRC_PARITY_N)
				{
					_u32TouchRCData = ((_u32TouchRCData<<1) | 0x01);
					_u32TouchRCDataReverse = ((_u32TouchRCDataReverse>>1) | (1L<<(TOUCHRC_TOTAL_LEN-1)));
					_u8TouchRCBit++;
				}
				else if(_TouchRCParity == TOUCHRC_PARITY_P)
				{
					_u32TouchRCData = (_u32TouchRCData<<1);
					_u32TouchRCDataReverse = (_u32TouchRCDataReverse>>1);
					_u8TouchRCBit++;
				}
				else if(_u8TouchRCBit < (TOUCHRC_TOTAL_LEN-1))
				{
					if(_TouchRCParity == TOUCHRC_PARITY_NN)
					{
						_u32TouchRCData = ((_u32TouchRCData<<2) | 0x03);
						_u32TouchRCDataReverse = (_u32TouchRCDataReverse>>2) | (3L<<(TOUCHRC_TOTAL_LEN-2));
						_u8TouchRCBit+=2;
					}
					else if(_TouchRCParity == TOUCHRC_PARITY_NP)
					{
						_u32TouchRCData = (_u32TouchRCData<<2) | 0x02;
						_u32TouchRCDataReverse = (_u32TouchRCDataReverse>>2) | (1L<<(TOUCHRC_TOTAL_LEN-2));
						_u8TouchRCBit+=2;
					}
					else
					{
						_TouchRCState = TOUCHRC_FAIL;
					}
			    }

				if(_u8TouchRCBit == TOUCHRC_TOTAL_LEN)
				{
					_TouchRCState = TOUCHRC_FINISH;
				}
			}
			else
			{
				_TouchRCDataState = TOUCHRC_DATA_RESET;
			}
		}

		if(_TouchRCDataState == TOUCHRC_DATA_RESET)
		{
			_TouchRCState = TOUCHRC_FAIL;
		}

		_TouchRCDataState = TOUCHRC_DATA_IDLE;
	}

	//UARTputHEX_U(_u8TouchRCBit);

	if(_TouchRCState == TOUCHRC_FINISH)
	{
		if(_u8TouchRCBit == TOUCHRC_TOTAL_LEN)
		{
//			UARTputcharErr('=');

			//  IR finish, Has Key
			//printk("\n@@ 0x%X(%u)\n", _u32TouchRCData, (_u32TouchRCData&0xFFFF));
#ifdef CORE_M0			// Testing on M0 board
			UART_Show_CPU_Timer();
#endif // #ifdef CORE_M0			// Testing on M0 board
			bTouchRCHasValue = TRUE;
	
			// Adopted from // Line ca.830
			_TouchRCState = TOUCHRC_IDLE;
			_TouchRCDataState = TOUCHRC_DATA_IDLE;
			_u8TouchRCBit = 0;
			blNeedToRestoreLeadingBit = FALSE;
			// END
		}
		else
		{
			_TouchRCState = TOUCHRC_FAIL;
		}
	}
	
	if(_TouchRCState == TOUCHRC_FAIL)
	{
//		UARTputcharErr('F');
		_TouchRCState = TOUCHRC_IDLE; 			// Line ca.830
		_TouchRCDataState = TOUCHRC_DATA_IDLE;
		_u8TouchRCBit = 0;
		blNeedToRestoreLeadingBit = FALSE;
	}
	return;
}

#endif // 0

#ifdef RC6_SUPPORT

BOOL 						bRC56HasKey = FALSE;
U32		RC6_VALUE_AREA		_u32IRData = 0;

void RC6_Decoder_Siggle_Edge_Last_Bit_TimeOut(void)
{
	_u32IRData = ((Return_RC6_Value() << 1)|1);
	bRC56HasKey = TRUE;
	Clear_RC6_Automata();
	//UARTputchar('*');
}

void RC6_Decoder_Single_Edge(U32 u32IrCounter)
{
	Bool	Automata_OK;

	ClearMyTimer_us();
	// here we deal with the situation when a not-long-enough (<=IR_RP_TIMEOUT+2ms)  pulse-width occurred at missing last bit
	// at main-loop, it deal with long-enough pulse-width occurred at missing last bit
	if(Check_RC6_if_missing_last_bit()==TRUE)
	{
		// Required when Timeout mechanism is implemented
		if(ARC6_T3_MAX < u32IrCounter)
		{
			RC6_Decoder_Siggle_Edge_Last_Bit_TimeOut();
			if(RC6_AUTOMATA_TIMEOUT_TIME==u32IrCounter)
			{
				return;
			}
		}
	}

	if(ARC6_T2_MIN < u32IrCounter && u32IrCounter <= ARC6_T2_MAX)
	{
		Automata_OK = Update_RC6_Automata(RC6_2T_VALUE);
		//UARTputchar('2');
	}
	else if(ARC6_T3_MIN < u32IrCounter && u32IrCounter <= ARC6_T3_MAX)
	{
		Automata_OK = Update_RC6_Automata(RC6_3T_VALUE);
		//UARTputchar('3');
	}
	else if(ARC6_T4_MIN < u32IrCounter && u32IrCounter <= ARC6_T4_MAX)
	{
		Automata_OK = Update_RC6_Automata(RC6_4T_VALUE);
		//UARTputchar('4');
	}
	else if(ARC6_T5_MIN < u32IrCounter && u32IrCounter <= ARC6_T5_MAX)
	{
		Automata_OK = Update_RC6_Automata(RC6_5T_VALUE);
		//UARTputchar('5');
	}
	else if(ARC6_T6_MIN < u32IrCounter && u32IrCounter <= ARC6_T6_MAX)
	{
		Automata_OK = Update_RC6_Automata(RC6_6T_VALUE);
		//UARTputchar('6');
	}
	else if(ARC6_T8_MIN < u32IrCounter && u32IrCounter <= ARC6_T8_MAX)
	{
		Automata_OK = Update_RC6_Automata(RC6_8T_VALUE);
		//UARTputchar('8');
	}
	else if(u32IrCounter > ARC6_IR_RP_TIMEOUT)
	{
		Automata_OK = Update_RC6_Automata(RC6_IR_TIMEOUT);
		//UARTputchar('T');
	}
	else
	{
		Clear_RC6_Automata();
		//UARTputchar('X');
		return;
	}

	if(Automata_OK==TRUE)
	{
		if(Check_RC6_Automata_Finished()==TRUE)
		{
			_u32IRData = Return_RC6_Value();
			bRC56HasKey = TRUE;
			Clear_RC6_Automata();
			//UARTputchar(' ');
		}
		// Required when Timeout mechanism is implemented
		else if(Check_RC6_if_missing_last_bit()==TRUE)
		{
			SetMyTimer_us(RC6_AUTOMATA_TIMEOUT_TIME);
		}
	}
	else
	{
		// Error State;
		Clear_RC6_Automata();
		//UARTputchar('F');
		//UARTputchar('\n');
	}
}

#endif // RC6_SUPPORT

static uint32_t	u32_previous_code=0xffffffff;

//void NEC_Decoder_Single_Edge(U32 u32IrCounter)
void NEC_Decoder_Single_Edge(U16 u32IrCounter)
{
	Bool	Automata_OK;

	if(u32IrCounter > ANEC_IR_RP_TIMEOUT)
	{
		Automata_OK = Update_NEC_Automata(NEC_T_BLANK_MIN);
		MyUARTputchar('^');
	}
	else if(ANEC_TRPT_WAIT_MIN < u32IrCounter && u32IrCounter <= ANEC_TRPT_WAIT_MAX)
	{
		Automata_OK = Update_NEC_Automata(NEC_T_FINISH_REPEAT);
		MyUARTputchar('_');
	}
	else if(ANEC_TDATA_WAIT_MIN < u32IrCounter && u32IrCounter <= ANEC_TDATA_WAIT_MAX)
	{
		Automata_OK = Update_NEC_Automata(NEC_T_FINISH_DATA);
		MyUARTputchar('-');
	}
	else if(ANEC_TLD_MIN < u32IrCounter && u32IrCounter <= ANEC_TLD_MAX)
	{
		Automata_OK = Update_NEC_Automata(NEC_T_LEADING);
		MyUARTputchar('L');
	}
	else if(ANEC_TRPT_MIN < u32IrCounter && u32IrCounter <= ANEC_TRPT_MAX)
	{
		Automata_OK = Update_NEC_Automata(NEC_T_REPEAT);
		MyUARTputchar('R');
	}
	else if(ANEC_T1_MIN < u32IrCounter && u32IrCounter <= ANEC_T1_MAX)
	{
		Automata_OK = Update_NEC_Automata(NEC_T1_VALUE);
		MyUARTputchar('1');
	}
	else if(ANEC_T0_MIN < u32IrCounter && u32IrCounter <= ANEC_T0_MAX)
	{
		Automata_OK = Update_NEC_Automata(NEC_T0_VALUE);
		MyUARTputchar('0');
	}
	else
	{
		Automata_OK = FALSE;
		MyUARTputchar('x');
	}

	if(Automata_OK==FALSE)
	{
		// Error State;
		Clear_NEC_Automata();
		u32_previous_code=0xffffffff;
		MyUARTputchar('f');
	}
}

BOOL _MDrv_IR_GetKey_NEC_Single_Sided(U32 *rc_value, U8 *pu8Flag)
{
    if(Return_NEC_if_Repeat()!=FALSE)
    {
      // Repeat last keycode
      *rc_value = u32_previous_code;
      *pu8Flag = 1;
      return TRUE;
    }
    else
    {
      if(Check_NEC_Automata_Finished()!=FALSE)
      {
        u32_previous_code = Return_NEC_Value();
        *rc_value = u32_previous_code;
        *pu8Flag = 0;
        return TRUE; 
      }
      else
      {
        return FALSE;
      }
    }
    return FALSE;
}

#ifdef RC6_SUPPORT
uint8_t LookupKeyCode_RC6(uint32_t rc_value)
{
  uint16_t	index;
  uint8_t	input_rc;

  if(((uint8_t)(rc_value>>17)&0x07)!=RC6_MODE)
		return 0;
	
  if((uint8_t)(rc_value>>8)!=RC6_CONTROL)
		return 0;

  input_rc = ((uint8_t)rc_value);

  if(input_rc==(uint8_t)(0x0C))  // POWER_BUTTON
  {
    P1^=(1<<3);			//if EXINT, toggle P1(PB) for testing
  }

  /*
  index=0;
  do
  {
    if(RC6_LUT[index]==input_rc)
	{
		return RC6_LUT[index+1];
	}
    index+=2;
  }
  while(index<RC6_LUT_SIZE);
*/
  return 0;
}

#endif // RC6_SUPPORT

uint8_t LookupKeyCode_NEC(uint32_t rc_value)
{
  uint16_t	index;
	uint8_t		input_rc;

  // Check 1st custom code
	if((uint8_t)(rc_value)!=NEC_CUSTOM_CODE_0)
		return 0;

  // Check 2nd custom code
	rc_value >>= 8;
	if((uint8_t)(rc_value)!=NEC_CUSTOM_CODE_1)
		return 0;

  // Check compliment of last 2 bytes
	index = (uint16_t) (rc_value >> 8);
	input_rc = LOWBYTE(index);
	if(~input_rc != HIGHBYTE(index))
		return 0;

  if(input_rc==(uint8_t)(POWER_ON_KEY_CODE))  // POWER_BUTTON
  {
    return POWER_ON_KEY_CODE;
    //P1^=(1<<3);			//if EXINT, toggle P1(PB) for testing
  }
  else if(input_rc==(uint8_t)(POWER_OFF_KEY_CODE)) 
  {
    return POWER_OFF_KEY_CODE;
  }
  else
  {
    return 0;
  }
/*
	index=0;
  do
 	{
    	if(NEC_LUT[index]==input_rc)
		{
			return NEC_LUT[index+1];
		}
    	index+=2;
 	}
 	while(index<NEC_LUT_SIZE);
 	return 0;

*/
  
}
