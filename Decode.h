/********************************************************************************/
/*	Weltrend Semiconductor, Inc.						*/
/*	Header file for WT65xx USB microcontroller.				*/
/*	Copyright (c) Weltrend Semiconductor, Inc.				*/
/*	All rights reserved.							*/
/********************************************************************************/
#ifndef _DECODE_H_
#define _DECODE_H_

//
// Touch RC
//

//
// Touch RC data
//

#define irGetMinCnt(time, tolerance)        ((u32)(((double)Covert_us_to_WidthBase(time))*((double)1-(double)tolerance)))
#define irGetMaxCnt(time, tolerance)        ((u32)(((double)Covert_us_to_WidthBase(time))*((double)1+(double)tolerance)))
#define irGetCnt(time)                      ((u32)Covert_us_to_WidthBase(time))

#define TOUCHRC_TOTAL_LEN           		 (1+10+10+2+2+4)	   // 29

// IR Definition for Touch RC
#define IR_NO_TOUCH_SIGNAL_TIMEOUT_TIME  	(1000000L)	// 1 sec - guess value
#define IR_LEADING_TIME_MID_TIME			(9746)
#define IR_PULSE_1_MID_TIME					(263)
#define IR_PULSE_2_MID_TIME					(IR_PULSE_1_MID_TIME*2)
#define IR_PULSE_3_MID_TIME					(IR_PULSE_1_MID_TIME*3)
#define IR_PULSE_4_MID_TIME					(IR_PULSE_1_MID_TIME*4)

#define IR_NO_TOUCH_SIGNAL_TIMEOUT			irGetCnt(IR_NO_TOUCH_SIGNAL_TIMEOUT_TIME)
#define IR_NO_SIGNAL_TIMOUT					irGetCnt(IR_NO_SIGNAL_TIMOUT_TIME)
#define IR_LEADING_TIME_MID					irGetCnt(IR_LEADING_TIME_MID_TIME)
#define IR_PULSE_1_MID						irGetCnt(IR_PULSE_1_MID_TIME)
#define IR_PULSE_2_MID						irGetCnt(IR_PULSE_2_MID_TIME)
#define IR_PULSE_3_MID						irGetCnt(IR_PULSE_3_MID_TIME)
#define IR_PULSE_4_MID						irGetCnt(IR_PULSE_4_MID_TIME)

#define IR_LEADING_TIME_LB					irGetMinCnt(IR_LEADING_TIME_MID_TIME,0.35)
#define IR_LEADING_TIME_HB					irGetMaxCnt(IR_LEADING_TIME_MID_TIME,0.35)
#define IR_PULSE_1_LB						irGetMinCnt(IR_PULSE_1_MID_TIME,0.3)
#define IR_PULSE_1_HB						irGetMaxCnt(IR_PULSE_1_MID_TIME,0.3)
#define IR_PULSE_2_LB						irGetMinCnt(IR_PULSE_2_MID_TIME,0.2)
#define IR_PULSE_2_HB						irGetMaxCnt(IR_PULSE_2_MID_TIME,0.2)
#define IR_PULSE_3_LB						irGetMinCnt(IR_PULSE_3_MID_TIME,0.15)
#define IR_PULSE_3_HB						irGetMaxCnt(IR_PULSE_3_MID_TIME,0.15)
#define IR_PULSE_4_LB						irGetMinCnt(IR_PULSE_4_MID_TIME,0.1)
#define IR_PULSE_4_HB						irGetMaxCnt(IR_PULSE_4_MID_TIME,0.1)
// END

typedef enum _TouchRCParityType
{
	TOUCHRC_PARITY_N = 0,
	TOUCHRC_PARITY_P,
	TOUCHRC_PARITY_TOTAL,
	TOUCHRC_PARITY_NN,		// NN & NP is for testing purpose only
    TOUCHRC_PARITY_NP,
}   TouchRCParityType;

typedef enum _TouchRCFrameHalfIdx
{
	TOUCHRC_1ST_FRAME_HALF = 0,
	TOUCHRC_2ND_FRAME_HALF,
	TOUCHRC_TOTAL_FRAME_HALF,
}   TouchRCFrameHalfIdx;

typedef enum _TouchRCSignal
{
    TOUCHRC_SIGNAL_NULL = 0,
    TOUCHRC_1T = 1,
    TOUCHRC_2T,
    TOUCHRC_3T,
    TOUCHRC_4T,
    TOUCHRC_5T,
    TOUCHRC_6T,
	TOUCHRC_LEADING_SIGNAL,
}   TouchRCSignal;

typedef enum _TouchRCState
{
	TOUCHRC_IDLE= 0,
	TOUCHRC_LEADER_BIT,
	TOUCHRC_DATA,
	TOUCHRC_FAIL,
	TOUCHRC_FINISH,
}   TouchRCState;

typedef enum _TouchRCDataState
{
	TOUCHRC_DATA_IDLE = 0,
	TOUCHRC_DATA_TRAIL,
	TOUCHRC_DATA_INSERT,
	TOUCHRC_DATA_RESET,

}   TouchRCDataState;

extern BOOL bTouchRCHasValue;
extern int return_x, return_y;
extern BOOL button_0, button_1;

extern unsigned char BUTTON_NOW;
extern unsigned char Delta_X,Delta_Y; //,ZAXIS=0; 

extern bit USB_SEND_F1, USB_SEND_F2; 

extern void TouchRC_Decoder_Single_Edge(U16 u32IrCounter);
extern BOOL _MDrv_IR_GetKey_TouchRC(void);

//
//  RC6
//

// RC5/6



//
// RC Automata
//

extern void RC6_Decoder_Single_Edge(U32 u32IrCounter);
//extern void NEC_Decoder_Single_Edge(U32 u32IrCounter);
extern void NEC_Decoder_Single_Edge(U16 u32IrCounter);
extern BOOL _MDrv_IR_GetKey_NEC_Single_Sided(U32 *rc_value, U8 *pu8Flag);

extern uint8_t LookupKeyCode_NEC(U32 input_rc);
extern uint8_t LookupKeyCode_RC6(U32 input_rc);

#define RC6_VALUE_AREA		idata
extern 	BOOL 					bRC56HasKey;
extern	U32 	RC6_VALUE_AREA 	_u32IRData;


#endif
