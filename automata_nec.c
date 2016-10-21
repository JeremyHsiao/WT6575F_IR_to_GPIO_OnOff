/*
 * automata_nec.c
 *
 *  Created on: 2015年1月14日
 *      Author: jeremy.hsiao
 */

#include "Port_ARM.h"
#include "automata_common.h"

static AUTOMATA_STATE 	automata_current_state=0;
static AUTOMATA_POS		automata_current_step_index=0;
static AUTOMATA_RESULT	automata_current_result_nec=0;

typedef struct { uint8_t len; uint8_t value; } STRUCT_UPDATE_VALUE;

//
// Definition for RC6
//
#define		NO_VALUE 	0
#define		ET0T 	 	1
#define		ET1T     	2
#define		ETKEEP0		3
#define		ETKEEP1		4
#define		ETLONG		5
#define		ETLDS		6
#define		ETRPT0E		7
#define		E_MAX		ETRPT0E		// Same as last one

// This define is to indicate number of rows in nec_automata_table[][]
#define STATE_0			0
#define STATE_1			1
#define STATE_MAX 		(STATE_1+1)		// Last state + 1

const STRUCT_UPDATE_VALUE	code	nec_state_transition_map[E_MAX] = {
	{   1, 	  0 },				// when 0 is received
	{   1, 	  1 },				// when 1 is received
	{   0,    0 },				// Stay at 0 row of next step
	{   0,    1 },				// Stay at 1 row of next step
	{0xff,	  1 },				// Go back to SLONG
	{0xff,    2 },				// Go back to SLDS
	{0xff,   36 },				// Go back to SRPT0E			// NEC_REPEAT_STEP
};

#define		S_ERR		0
#define		SINIT		1		// Initial State -- after error
#define		SLONG		2		// >110ms blank detected
#define		SLDS		3		// after 9+4.5 detected
#define		S0			4		// After 0 detected
#define		S1			5		// After 1 detected
#define		S0E			6		// last bit received, wait for an delay between 23~99ms detected (form S0)  (as delay)
#define		S1E			7		// last bit received, wait for an delay between 23~99ms detected (form S0)  (as delay)
#define		S0EWAIT		8		// After an delay between 23~99ms detected (form S0)
#define		S1EWAIT		9		// After an delay between 23~99ms detected (form S1)
#define		SRPT0E		10		// After an 9+2.25 detected
#define		SRPT1E		11		// After an 9+2.25 detected
#define 	SRPT0EWAIT  12		// After an (110-9-2.25-0.56) detected (from SRTP0)
#define 	SRPT1EWAIT  13		// After an (110-9-2.25-0.56) detected (from SRTP1)
#define		S_MAX		SRPT1EWAIT  // Same as last one

const uint8_t code  nec_update_table[S_MAX][NEC_T_VALUE_COUNT] = {
		// T0		T1		  TRPT		TLDS		F_Data_W	F_Rpt_W		F_Blank_W			// Timing 0~6
		{ NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE,	NO_VALUE,	NO_VALUE,	ETKEEP0	},			// SINIT
		{ NO_VALUE, NO_VALUE, NO_VALUE, ETKEEP0,	NO_VALUE,	NO_VALUE,	ETLONG  },			// SLONG
		{ ET0T, 	ET1T, 	  NO_VALUE, NO_VALUE,	NO_VALUE,   NO_VALUE,   ETLONG  },			// SLDS
		{ ET0T, 	ET1T,	  NO_VALUE, NO_VALUE,	NO_VALUE,   NO_VALUE,   ETLONG  },			// 0
		{ ET0T, 	ET1T,	  NO_VALUE, NO_VALUE,	NO_VALUE,   NO_VALUE,   ETLONG  },			// 1
		{ NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE,	ETKEEP0,    NO_VALUE,   ETLONG  },			// 0E
		{ NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE,	ETKEEP1,    NO_VALUE,   ETLONG  },			// 1E
		{ NO_VALUE, NO_VALUE, ETKEEP0,  ETLDS,		NO_VALUE,   NO_VALUE,   ETLONG  },			// S0EWAIT
		{ NO_VALUE, NO_VALUE, ETKEEP1,  ETLDS,		NO_VALUE,   NO_VALUE,   ETLONG  },			// S1EWAIT
		{ NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE,	NO_VALUE,    ETKEEP0,   ETLONG  },			// SRPT0E
		{ NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE,	NO_VALUE,    ETKEEP1,   ETLONG  },			// SRPT1E
		{ NO_VALUE, NO_VALUE, ETRPT0E,  ETLDS,		NO_VALUE,   NO_VALUE,   ETLONG  },			// SRPT0EWAIT
		{ NO_VALUE, NO_VALUE, ETRPT0E,  ETLDS,		NO_VALUE,   NO_VALUE,   ETLONG  }			// SRPT1EWAIT
};

#define	NEC_LENGTH			(32)
#define NEC_REPEAT_STEP		(36)
#define NEC_DATA_OK_STEP	(34)
#define NEC_TOTAL_STEP		(37+1)

const uint8_t code	nec_automata_table[STATE_MAX][NEC_TOTAL_STEP] = {
//     0	  1    2     3  4  5  6  7   8  9  10 11 12  13 14 15 16 17  18 19 20 21 22  23 24 25 26 27  28 29 30 31 32  33 34   35       36     37
  {	SINIT, SLONG, SLDS,  S0,S0,S0,S0,S0, S0,S0,S0,S0,S0, S0,S0,S0,S0,S0, S0,S0,S0,S0,S0, S0,S0,S0,S0,S0, S0,S0,S0,S0,S0, S0,S0E,S0EWAIT,SRPT0E,SRPT0EWAIT },
  { S_ERR, S_ERR, S_ERR, S1,S1,S1,S1,S1, S1,S1,S1,S1,S1, S1,S1,S1,S1,S1, S1,S1,S1,S1,S1, S1,S1,S1,S1,S1, S1,S1,S1,S1,S1, S1,S1E,S1EWAIT,SRPT1E,SRPT1EWAIT }
};

void Clear_NEC_Automata(void)
{
	automata_current_state=0;
	automata_current_step_index=0;
	automata_current_result_nec=0;
}

Bool Update_NEC_Automata(uint8_t incoming_value)
{
	uint8_t	table_index, temp;

	//
	// Filtering error state/step
	//
	if((incoming_value>=NEC_T_VALUE_COUNT)||(automata_current_step_index>=(NEC_TOTAL_STEP)))
	{
		return FALSE;
	}

	//
	// (1) According to current state & step, find out the corresponding update table
	//
	table_index = nec_automata_table[automata_current_state][automata_current_step_index];

	//
	// Filtering error state/step/case
	//
	if(table_index==S_ERR)
	{
		return FALSE;
	}
	//
	// (2) Referring to update table, find out what is the update value (by pulse_ width)
	//
	table_index--;		// adjust value to be array index
	table_index = nec_update_table[table_index][incoming_value];
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
	temp = nec_state_transition_map[table_index].len;
	if(temp==0)
	{
		// 0 bit value here is used to indicate setting state value without updating result (step still +1)
		// Update step by +1
		automata_current_step_index ++;
		// Result no update
		// Update state
		automata_current_state = nec_state_transition_map[table_index].value;
	}
	else if (temp==0xFF)
	{
		// Force value to be 0 and state to be 0 and step to be specific value
		automata_current_step_index = nec_state_transition_map[table_index].value;
		automata_current_state = 0;
		automata_current_result_nec = 0;
	}
	else
	{
		// Update step
		automata_current_step_index ++;
		// Update result
		automata_current_result_nec >>= 1;
		temp = nec_state_transition_map[table_index].value;
		if(temp!=0)
		{
			automata_current_result_nec |= 0x80000000;
		}
		// Update state
		automata_current_state = temp;
	}

	return TRUE;
}

Bool Check_NEC_Automata_Finished(void)
{
	if(automata_current_step_index==NEC_DATA_OK_STEP)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

AUTOMATA_RESULT Return_NEC_Value(void)
{
	return automata_current_result_nec;
}

Bool Return_NEC_if_Repeat(void)
{
	if(automata_current_step_index==NEC_REPEAT_STEP)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
