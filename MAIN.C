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
//									       									        //
//=============================================================================//
#include <absacc.h>
#include <REG52.H>
#include <INTRINS.H>
#include "MEMALLOC.h"
#define _DEFINE_XFR_VARIABLE
#include "XFR.h"
#include "MAIN.h"
#include "buffer.h"
#include "int.h"
#include "decode.h"
#include "automata_common.h"
#include "KeyCodeTable.h"

//=====================================================================
// Variable
//=====================================================================

unsigned char data  KEY_PRE=0xFF, KEY_NOW=0xFF;

void Delayms(const count)
{
	unsigned char i,j,k;
	for(i=0; i<count; i++)
	{
		for(j=0; j<0x7; j++)
			for(k=0; k<226; k++);
	}
}

//=================================================================
//	Initialization IO PORT FUNCTION
//=================================================================
void INIT_PORT(void)
{
	PAOE=0x00;
	PUPCTRLB=0;		//Enable pull-up register
	P1 |= 0x80;	
}

unsigned char PWMDuty;
unsigned int  ms_count = 0;

//#define EXT_CRYSTALL		//Using external crystall
void main()
{
	unsigned char tmp_key = 0;

#ifdef EXT_CRYSTALL
	SYSCR1=0x30;
	ENCALIB=0;
#endif
	SIEI|=0x80;		//Enable Full Speed
	Delayms(15);		//Delay 15ms for ISP, please reverve it.
	
	InitialEXINT();		//Initial EXINT
	INIT_PORT();
	INIT_TIMER0();
	EA=1;

	while(1)
	{
		{
			uint16_t value;

#ifdef RC6_SUPPORT				
      if(CheckMyTimerTimeOut()==TRUE)
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
          pulse_data[in_index] = RC6_AUTOMATA_TIMEOUT_TIME;
          in_index = temp_index;
        }
      }
#endif // RC6_SUPPORT
      
			if(Pop_Tx_Width(&value)!=FALSE)
			{
				uint32_t	RC_data;
				uint8_t		nec_repeat;

#ifdef RC6_SUPPORT				
				RC6_Decoder_Single_Edge(value);
        if(bRC56HasKey==TRUE)
		  	{
					tmp_key = LookupKeyCode_RC6(_u32IRData);
					bRC56HasKey = FALSE;
					_u32IRData = 0;
		  	}
				else
#endif // RC6_SUPPORT
				{
					NEC_Decoder_Single_Edge(value);
					if(_MDrv_IR_GetKey_NEC_Single_Sided(&RC_data,&nec_repeat)==TRUE)
					{
						tmp_key = LookupKeyCode_NEC(RC_data);
						RC_data = 0;
						nec_repeat = 0; 
					}
				}
				if(tmp_key!=0)
				{
          if(tmp_key==POWER_ON_KEY_CODE)
          {  
            //P1&=~(1<<3);	
            P1=0;
          }
          else if(tmp_key==POWER_OFF_KEY_CODE)
          {
            //P1|=(1<<3);		
            P1=0xff;
          }
					tmp_key = 0;
					//Delayms(20);
				}
			}
		}
	}
	
}
