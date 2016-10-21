/*
 * rc_automata.c
 *
 *  Created on: 2015年1月14日
 *      Author: jeremy.hsiao
 */

#include "Port_ARM.h"
#include "automata_common.h"

#ifdef RC6_SUPPORT

static AUTOMATA_STATE 	automata_current_state=0;
static AUTOMATA_POS		automata_current_step_index=0;
static AUTOMATA_RESULT	automata_current_result=0;

typedef struct { uint8_t len; uint8_t value; } STRUCT_UPDATE_VALUE;

//
// Definition for RC6
//
#define		NO_VALUE 	0
#define		E0T2 	 	1
#define		E0T3     	2
#define		E0T4     	3
#define  	E1T2     	4
#define 	E1T3     	5
#define 	E_START    	6
#define		E_MAX		E_START		// Same as last one

const STRUCT_UPDATE_VALUE code	rc6_state_transition_map[E_MAX] = {
	{   1, 0 },
	{   2, 3 },
	{   2, 2 },
	{   1, 1 },			// E1T2
	{   1, 0 },
	{0xff, 1 }			// E_START
};

#define		S_ERR		0
#define		S0			1
#define		S0_3RD		2
#define		S0_4TH		3
#define		S0_5TH		4
#define		S1			5
#define		S1_4TH		6
#define		S1_5RD		7
#define		S_INIT		8
#define		S_LONG	    9
#define		S_MAX		S_LONG		// Same as last one

const uint8_t code rc6_update_table[S_MAX][RC6_T_VALUE_COUNT] = {									// 0~5
		// 2T		3T			4T			5T			6T		  8T		IR_Out
		{ E0T2, 	E0T3, 		E0T4, 		NO_VALUE, 	NO_VALUE, NO_VALUE, E_START },			// Normal 0			// 1
		{ E0T2, 	E0T3, 		NO_VALUE, 	E0T4, 		NO_VALUE, NO_VALUE,	E_START },			// 0 @ 3rd bit		// 2
		{ NO_VALUE,	E0T2, 		NO_VALUE,	E0T3, 		E0T4 	, NO_VALUE,	E_START },			// 0 @ 4th bit		// 3
		{ NO_VALUE,	E0T2, 		E0T3, 		E0T4, 		NO_VALUE, NO_VALUE,	E_START },			// 0 @ 5th bit		// 4
		{ E1T2, 	E1T3, 		NO_VALUE, 	NO_VALUE, 	NO_VALUE, NO_VALUE,	E_START },			// Normal 1			// 5
		{ E1T2, 	NO_VALUE, 	E1T3, 		NO_VALUE, 	NO_VALUE, NO_VALUE,	E_START },			// 1 @ 4th bit		// 6
		{ NO_VALUE,	NO_VALUE,	E1T2, 		E1T3, 		NO_VALUE, NO_VALUE, E_START },			// 1 @ 5th bit		// 7
		{ NO_VALUE,	NO_VALUE,	NO_VALUE, 	NO_VALUE, 	NO_VALUE, NO_VALUE,	E_START },			// S_INIT			// 8
		{ NO_VALUE,	NO_VALUE,	NO_VALUE, 	NO_VALUE, 	NO_VALUE, E1T2, 	E_START },			// S_LONG			// 9
};

#define	RC6_LENGTH			(5+8+8)
#define RC6_TOTAL_STEP		(RC6_LENGTH+2)
#define RC6_DATA_OK_STEP	(22)

const uint8_t code rc6_automata_table[2][RC6_TOTAL_STEP] = {
//		0	1			2		3   4          5      6     7   8   9  10   11  12  13  14  15  16  17  18  19  20  21  22
  {	S_INIT, S_LONG,	 S_ERR, 	S0, S0_3RD, S0_4TH, S0_5TH, S0, S0, S0, S0, S0, S0, S0, S0, S0, S0, S0, S0, S0, S0, S0,	S_INIT },
  { S_ERR,  S_ERR,		S1,		S1, S1,		S1_4TH, S0_5TH, S1, S1, S1, S1, S1, S1, S1, S1, S1, S1, S1, S1, S1, S1, S1,	S_INIT }
};

void Clear_RC6_Automata(void)
{
	automata_current_state=0;
	automata_current_step_index=0;
	automata_current_result=0;
}

Bool Update_RC6_Automata(uint8_t incoming_value)
{
	uint8_t	table_index, temp;

	//
	// Filtering error state/step
	//
	if((incoming_value>=RC6_T_VALUE_COUNT)||(automata_current_step_index>=RC6_TOTAL_STEP))
	{
		return FALSE;
	}
	table_index = rc6_automata_table[automata_current_state][automata_current_step_index];
	//
	// (1) According to current state & step, find out the corresponding update table
	//
	if(table_index==S_ERR)
	{
		return FALSE;
	}
	table_index--;		// adjust value to be array index
	table_index = rc6_update_table[table_index][incoming_value];
	//
	// Filtering error state/step/case
	//
	if(table_index==NO_VALUE)
	{
		return FALSE;
	}
	//
	// (3) update step & state -- Normally 1 step more each time, except RC6
	//
	table_index--;		// adjust value to be array index
	temp = rc6_state_transition_map[table_index].len;
	if (temp==0xFF)
	{

			// Force value to be 0 and state to be 0 and step to be specific value
			automata_current_step_index = rc6_state_transition_map[table_index].value;
			automata_current_state = 0;
			automata_current_result = 0;
	}
	else
	{
		// Update step
		automata_current_step_index += temp;
		// Update result & state
		automata_current_result <<= temp;
		temp = rc6_state_transition_map[table_index].value;
		if(temp!=0)
		{
			automata_current_result |= temp;
			automata_current_state = temp&0x01;
		}
		else
		{
			automata_current_state = 0;
		}
	}
	return TRUE;
}

Bool Check_RC6_Automata_Finished(void)
{
	if(automata_current_step_index==(RC6_DATA_OK_STEP))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// This function is only applied when detecting a long delay
// A better way is: setup a timeout timer (larger than 4T) at specific step
// If timeout occurs, it means this situation occurs
// If next bit is coming, stop this timeout timer
Bool Check_RC6_if_missing_last_bit(void)
{
	if((automata_current_step_index==(RC6_DATA_OK_STEP-1))&&(automata_current_state==0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

AUTOMATA_RESULT Return_RC6_Value(void)
{
	return automata_current_result;
}

#endif // RC6_SUPPORT

