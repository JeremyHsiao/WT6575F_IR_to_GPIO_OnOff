/*
 * rc_automata.h
 *
 *  Created on: 2015年1月15日
 *      Author: jeremy.hsiao
 */

#ifndef SRC_APP_AUTOMATA_COMMON_H_
#define SRC_APP_AUTOMATA_COMMON_H_

#include "Port_ARM.h"

//
// Common definition
//
typedef		uint8_t		AUTOMATA_STATE;
typedef		uint8_t		AUTOMATA_POS;
typedef		uint32_t	AUTOMATA_RESULT;
//
//
//

#define TIME_BASE   32

//
// Definition for RC6
//
#define	RC6_2T_VALUE		  0
#define	RC6_3T_VALUE		  1
#define	RC6_4T_VALUE		  2
#define	RC6_5T_VALUE		  3
#define	RC6_6T_VALUE		  4
#define	RC6_8T_VALUE		  5
#define RC6_IR_TIMEOUT		6
#define	RC6_T_VALUE_COUNT	(RC6_IR_TIMEOUT+1)

//#define ARC6_T1_MAX  658
//#define ARC6_T1_MIN  250
#define ARC6_T2_MAX  ((1124 + TIME_BASE/2) / TIME_BASE)
#define ARC6_T2_MIN  ((672  + TIME_BASE/2) / TIME_BASE)
#define ARC6_T3_MAX  ((1500 + TIME_BASE/2) / TIME_BASE)
#define ARC6_T3_MIN  ((1126 + TIME_BASE/2) / TIME_BASE)
#define ARC6_T4_MAX  ((1996 + TIME_BASE/2) / TIME_BASE)
#define ARC6_T4_MIN  ((1580 + TIME_BASE/2) / TIME_BASE)
#define ARC6_T5_MAX  ((2443 + TIME_BASE/2) / TIME_BASE)
#define ARC6_T5_MIN  ((2001 + TIME_BASE/2) / TIME_BASE)
#define ARC6_T6_MAX  ((3360 + TIME_BASE/2) / TIME_BASE)
#define ARC6_T6_MIN  ((2444 + TIME_BASE/2) / TIME_BASE)
#define ARC6_T8_MAX  ((3888 + TIME_BASE/2) / TIME_BASE)	//
#define ARC6_T8_MIN  ((3361 + TIME_BASE/2) / TIME_BASE)	//
#define ARC6_IR_RP_TIMEOUT			((40000+TIME_BASE/2)/TIME_BASE)
#define RC6_AUTOMATA_TIMEOUT_TIME	(ARC6_T3_MAX+1)

extern void Clear_RC6_Automata(void);
extern void Start_RC6_Automata(void);
extern Bool Update_RC6_Automata(uint8_t incoming_value);
extern Bool Check_RC6_Automata_Finished(void);
extern Bool Check_RC6_if_missing_last_bit(void);
extern AUTOMATA_RESULT Return_RC6_Value(void);
//
// End of definition for RC6
//

//
// Definition for NEC
//
#define	NEC_T0_VALUE		    0			// 1.12
#define	NEC_T1_VALUE		    1			// 2.25
#define NEC_T_REPEAT		    2			// 9+2.25
#define	NEC_T_LEADING		    3			// 9+4.5
#define NEC_T_FINISH_DATA	  4			// between 23.94~60.1ms
#define NEC_T_FINISH_REPEAT	5			// about (110-9-2.25-0.56) ms	~= 98.19
#define NEC_T_BLANK_MIN		  6			// above 110 ms
#define	NEC_T_VALUE_COUNT	  (NEC_T_BLANK_MIN+1)

#define ANEC_T0_MIN  		    ((784    + TIME_BASE/2) / TIME_BASE)
#define ANEC_T0_MAX  		    ((1456   + TIME_BASE/2) / TIME_BASE)
#define ANEC_T1_MIN  		    ((1914   + TIME_BASE/2) / TIME_BASE)
#define ANEC_T1_MAX  		    ((2589   + TIME_BASE/2) / TIME_BASE)
#define ANEC_TRPT_MIN  		  ((9000   + TIME_BASE/2) / TIME_BASE)
#define ANEC_TRPT_MAX  		  ((12000  + TIME_BASE/2) / TIME_BASE)
#define ANEC_TLD_MIN  		  ((12001  + TIME_BASE/2) / TIME_BASE)
#define ANEC_TLD_MAX  		  ((15500  + TIME_BASE/2) / TIME_BASE)
#define ANEC_TDATA_WAIT_MIN	((18000  + TIME_BASE/2) / TIME_BASE)
#define ANEC_TDATA_WAIT_MAX	((70000  + TIME_BASE/2) / TIME_BASE)
#define ANEC_TRPT_WAIT_MIN	((80000  + TIME_BASE/2) / TIME_BASE)
#define ANEC_TRPT_WAIT_MAX	((106000 + TIME_BASE/2) / TIME_BASE)
#define ANEC_IR_RP_TIMEOUT	((110000 + TIME_BASE/2) / TIME_BASE)

extern void Clear_NEC_Automata(void);
extern void Start_NEC_Automata(void);
extern Bool Update_NEC_Automata(uint8_t incoming_value);
extern Bool Check_NEC_Automata_Finished(void);
extern AUTOMATA_RESULT Return_NEC_Value(void);
extern Bool Return_NEC_if_Repeat(void);

//#define RC6_SUPPORT		

#endif /* SRC_APP_AUTOMATA_COMMON_H_ */
