/********************************************************************************/
/*	Weltrend Semiconductor, Inc.												*/
/*	Header file for WT6573 USB microcontroller.									*/
/*	Copyright (c) Weltrend Semiconductor, Inc.									*/
/*	All rights reserved.														*/
/********************************************************************************/
#ifndef	_GrobalDefine_H
#define _GrobalDefine_H

//===========================================================
// SET ICE ENABLE/DISABLE
//===========================================================
#define ICE	0xff	//ff: ENABLE ICE
					//00: DISABLE ICE

//===========================================================
// SET FLASH ROM READ PROTECT
//===========================================================
#define READ_PROTECT	0xff	//0x00: ENABLE READ PROTECT
								//0xff: DISABLE READ PROTECT

//===========================================================
// SET CODE ENCRYPTION
//===========================================================
#define CODE_ENCRYPTION	0xFF	//0xAA: ENABLE CODE ENCRYPTION
								//Other: DISABLE CODE ENCRYPTION


#endif
