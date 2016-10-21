//=============================================================================//
// Copyright:															       //
//											      	        				   //
//       Copyright(C) 2008                          						   //
//	All rights reserved.						      						   //
//									      									   //
// Filename:								      							   //
//	KeyCodeTable.c 									      					   //
//									      									   //
// History:								      								   //
//									       									   //
//=============================================================================//

#include <INTRINS.H>
#include "Port_ARM.h"
#include "KeyCodeTable.h"

// USB Key      USB Key       USB Key         USB Key         USB Key 
// 00 Reserved  20 # 3        40 F7           60 8 Up KP      97 5 KP     
// 01           21 $ 4        41 F8           61 9 PgUp KP    9A Attn / SysRq     
// 02           22 % 5        42 F9           62 0 Ins KP     9C Clear            
// 03           23 ^ 6        43 F10          63 . Del KP     A3 CrSel          
// 04 A         24 & 7        44 F11          64 (INT 1)      A4 ExSel / SetUp     
// 05 B         25 * 8        45 F12          65 WinMenu      E0 Ctrl L ?? ??????                      
// 06 C         26 ( 9        46 PrtSc        68 F13          E1 Shif L                        
// 07 D         27 ) 0        47 Scroll Lock  69 F14          E2 Alt L                         
// 08 E         28 Enter      48 Pause/Bk     6A F15          E3 Win L      
// 09 F         29 Esc        49 Ins CP       6B F16          E4 Ctrl R  
// 0A G         2A Back Space 4A Home CP      6C F17          E5 Shift R      
// 0B H         2B Tab        4B PgUp CP      6D F18          E6 Alt R        
// 0C I         2C Space      4C Del CP       6E F19          E7 Win R        
// 0D J         2D _ -        4D End CP       6F F20                          
// 0E K         2E + =        4E PgDn CP      70 F21       
// 0F L         2F { [        4F Right CP     71 F22       
// 10 M         30 } ]        50 Left CP      72 F23       
// 11 N         31 | \        51 Down CP      73 F24       
// 12 O         32 (INT 2)    52 Up CP        75 Help      
// 13 P         33 : ;        53 Num Lock     7A Undo      
// 14 Q         34 " ,        54 / KP         7B Cut       
// 15 R         35 ~ `        55 * KP         7C Copy      
// 16 S         36 < ,        56 - KP         7D Paste     
// 17 T         37 > .        57 + KP         7F Mute      
// 18 U         38 ? /        58 Enter KP     80 VolumeUp
// 19 V         39 Caps Lock  59 1 End KP     81 VolumeDown
// 1A W         3A F1         5A 2 Down KP    87 (INT 3)
// 1B X         3B F2         5B 3 PgDn KP    88 katakana
// 1C Y         3C F3         5C 4 Left KP    89 (INT 4)
// 1D Z         3D F4         5E 6 Right KP   8A kanji
// 1E ! 1       3E F5         5F 7 Home KP    8B hiragana
// 1F @ 2       3F F6                         8C furigana

typedef enum {
    KEY_A = 4,
    KEY_B = 5,
    KEY_C = 6,
    KEY_D = 7,
    KEY_E = 8,
    KEY_F = 9,
    KEY_G = 10,
    KEY_H = 11,
    KEY_I = 12,
    KEY_J = 13,
    KEY_K = 14,
    KEY_L = 15,
    KEY_M = 16,
    KEY_N = 17,
    KEY_O = 18,
    KEY_P = 19,
    KEY_Q = 20,
    KEY_R = 21,
    KEY_S = 22,
    KEY_T = 23,
    KEY_U = 24,
    KEY_V = 25,
    KEY_W = 26,
    KEY_X = 27,
    KEY_Y = 28,
    KEY_Z = 29,
    KEY_1 = 30,
    KEY_2 = 31,
    KEY_3 = 32,
    KEY_4 = 33,
    KEY_5 = 34,
    KEY_6 = 35,
    KEY_7 = 36,
    KEY_8 = 37,
    KEY_9 = 38,
    KEY_0 = 39,
    KEY_Enter = 40,
    KEY_Escape = 41,
    KEY_Delete = 42,
    KEY_Tab = 43,
    KEY_Space = 44,
    KEY_Minus = 45,
    KEY_Equals = 46,
    KEY_LeftBracket = 47,
    KEY_RightBracket = 48,
    KEY_Backslash = 49,
    KEY_Semicolon = 51,
    KEY_Quote = 52,
    KEY_Grave = 53,
    KEY_Comma = 54,
    KEY_Period = 55,
    KEY_Slash = 56,
    KEY_CapsLock = 57,
    KEY_F1 = 58,
    KEY_F2 = 59,
    KEY_F3 = 60,
    KEY_F4 = 61,
    KEY_F5 = 62,
    KEY_F6 = 63,
    KEY_F7 = 64,
    KEY_F8 = 65,
    KEY_F9 = 66,
    KEY_F10 = 67,
    KEY_F11 = 68,
    KEY_F12 = 69,
    KEY_PrintScreen = 70,
    KEY_ScrollLock = 71,
    KEY_Pause = 72,
    KEY_Insert = 73,
    KEY_Home = 74,
    KEY_PageUp = 75,
    KEY_DeleteForward = 76,
    KEY_End = 77,
    KEY_PageDown = 78,
    KEY_Right = 79,
    KEY_Left = 80,
    KEY_Down = 81,
    KEY_Up = 82,
    KP_NumLock = 83,
    KP_Divide = 84,
    KP_Multiply = 85,
    KP_Subtract = 86,
    KP_Add = 87,
    KP_Enter = 88,
    KP_1 = 89,
    KP_2 = 90,
    KP_3 = 91,
    KP_4 = 92,
    KP_5 = 93,
    KP_6 = 94,
    KP_7 = 95,
    KP_8 = 96,
    KP_9 = 97,
    KP_0 = 98,
    KP_Point = 99,
    KEY_NonUSBackslash = 100,
    KP_Equals = 103,
    KEY_F13 = 104,
    KEY_F14 = 105,
    KEY_F15 = 106,
    KEY_F16 = 107,
    KEY_F17 = 108,
    KEY_F18 = 109,
    KEY_F19 = 110,
    KEY_F20 = 111,
    KEY_F21 = 112,
    KEY_F22 = 113,
    KEY_F23 = 114,
    KEY_F24 = 115,
    KEY_Help = 117,
    KEY_Menu = 118,
    KEY_Mute = 127,
    KEY_Vol_Up   = 128,
    KEY_Vol_Down = 129,
    KEY_LeftControl = 224,
    KEY_LeftShift = 225,
    KEY_LeftAlt = 226,
    KEY_LeftGUI = 227,
    KEY_RightControl = 228,
    KEY_RightShift = 229,
    KEY_RightAlt = 230,
    KEY_RightGUI = 231
} HID_KEYBOARD_CODE;

//
// PHILIPS RC (RC6 code)
//

typedef enum {
	RC6_0         = 0x00,
	RC6_1         = 0x01,
	RC6_2         = 0x02,
	RC6_3         = 0x03,
	RC6_4         = 0x04,
	RC6_5         = 0x05,
	RC6_6         = 0x06,
	RC6_7         = 0x07,
	RC6_8         = 0x08,
	RC6_9         = 0x09,
	RC6_POWER     = 0x0C,
	RC6_MUTE      = 0x0D,
	RC6_VOL_UP    = 0x10,
	RC6_VOL_DOWN  = 0x11,
	RC6_CH_UP     = 0x4C,
	RC6_CH_DOWN   = 0x4D,
	RC6_MENU      = 0x54,
	RC6_UP        = 0x58,
	RC6_DOWN      = 0x59,
	RC6_LEFT      = 0x5A,
	RC6_RIGHT     = 0x5B,
	RC6_OK        = 0x5C,
	RC6_MAX_CODE
} RC_RC6_CODE;

const uint8_t code RC6_LUT[] = {          RC6_MUTE    , KEY_Mute    ,  
	                                      RC6_MENU    , KEY_Menu    ,
	                                      RC6_UP      , KEY_Up      ,
	                                      RC6_VOL_UP  , KEY_Vol_Up  ,
	                                      RC6_OK      , KP_Enter    ,
	                                      RC6_DOWN    , KEY_Down    ,
	                                      RC6_VOL_DOWN, KEY_Vol_Down,
	                                      RC6_1       , KP_1        ,
	                                      RC6_2       , KP_2        ,
	                                      RC6_3       , KP_3        ,
	                                      RC6_CH_UP   , KEY_PageUp  ,
	                                      RC6_4       , KP_4        ,
	                                      RC6_5       , KP_5        ,
	                                      RC6_6       , KP_6        ,
	                                      RC6_CH_DOWN , KEY_PageDown,
	                                      RC6_7       , KP_7        ,
	                                      RC6_8       , KP_8        ,
	                                      RC6_9       , KP_9        ,
	                                      RC6_0       , KP_0        ,
	                                      RC6_LEFT    , KEY_Left    ,
	                                      RC6_RIGHT   , KEY_Right    };

const uint8_t  code RC6_MODE		  = 0x00; 
const uint8_t  code RC6_CONTROL		  = 0x00; 

//
// AOC RC (NEC code)
//

typedef enum {
	NEC_POWER     = 0x01,
	NEC_MUTE      = 0x04,
	NEC_MENU      = 0x0A,
	NEC_UP        = 0x0B,
	NEC_VOL_UP    = 0x0C,
	NEC_OK        = 0x0D,
	NEC_DOWN      = 0x0F,
	NEC_VOL_DOWN  = 0x10,
	NEC_1         = 0x15,
	NEC_2         = 0x16,
	NEC_3         = 0x17,
	NEC_CH_UP     = 0x18,
	NEC_4         = 0x19,
	NEC_5         = 0x1A,
	NEC_6         = 0x1B,
	NEC_CH_DOWN   = 0x1C,
	NEC_7         = 0x1D,
	NEC_8         = 0x1E,
	NEC_9         = 0x1F,
	NEC_0         = 0x41,
	NEC_LEFT      = 0x49,
	NEC_RIGHT     = 0x4A,
	NEC_MAX_CODE
} RC_NEC_CODE;

const uint8_t code NEC_LUT[] = {        NEC_MUTE    , KEY_Mute    ,  
	                                      NEC_MENU    , KEY_Menu    ,
	                                      NEC_UP      , KEY_Up      ,
	                                      NEC_VOL_UP  , KEY_Vol_Up  ,
	                                      NEC_OK      , KP_Enter    ,
	                                      NEC_DOWN    , KEY_Down    ,
	                                      NEC_VOL_DOWN, KEY_Vol_Down,
	                                      NEC_1       , KP_1        ,
	                                      NEC_2       , KP_2        ,
	                                      NEC_3       , KP_3        ,
	                                      NEC_CH_UP   , KEY_PageUp  ,
	                                      NEC_4       , KP_4        ,
	                                      NEC_5       , KP_5        ,
	                                      NEC_6       , KP_6        ,
	                                      NEC_CH_DOWN , KEY_PageDown,
	                                      NEC_7       , KP_7        ,
	                                      NEC_8       , KP_8        ,
	                                      NEC_9       , KP_9        ,
	                                      NEC_0       , KP_0        ,
	                                      NEC_LEFT    , KEY_Left    ,
	                                      NEC_RIGHT   , KEY_Right    };

const uint8_t  code NEC_CUSTOM_CODE_0  = 0x5A; 
const uint8_t  code NEC_CUSTOM_CODE_1  = 0xB4; 
const uint8_t  code POWER_ON_KEY_CODE  = 0x01; 
const uint8_t  code POWER_OFF_KEY_CODE = 0x02; 

//
// TPV Factory RC (NEC code)
//
typedef enum {
	FACTORY_NEC_TEST          = 0x6D,
	FACTORY_NEC_RST           = 0x5A,
	FACTORY_NEC_FAC           = 0x3E,
	FACTORY_NEC_CSM           = 0x50,
	FACTORY_NEC_PATTERN       = 0x62,
	FACTORY_NEC_1             = 0x0F,
	FACTORY_NEC_2             = 0x12,
	FACTORY_NEC_3             = 0x4A,
	FACTORY_NEC_4             = 0x30,
	FACTORY_NEC_5             = 0x10,
	FACTORY_NEC_6             = 0x11,
	FACTORY_NEC_7             = 0x32,
	FACTORY_NEC_8             = 0x35,
	FACTORY_NEC_9             = 0x36,
	FACTORY_NEC_0             = 0x33,
	FACTORY_NEC_ANTENNA_CABLE = 0x3F,
	FACTORY_NEC_PRE_CH        = 0x6B,
	FACTORY_NEC_VOL_MAX       = 0x5F,
	FACTORY_NEC_VOL_BUZZ      = 0x60,
	FACTORY_NEC_CTC           = 0x03,
	FACTORY_NEC_MENU          = 0x21,
	FACTORY_NEC_CH_PLUS       = 0x56,
	FACTORY_NEC_CH_MINUS      = 0x55,
	FACTORY_NEC_B_I           = 0x54,
	FACTORY_NEC_CH_SCAN       = 0x57,
	FACTORY_NEC_UP            = 0x26,
	FACTORY_NEC_DOWN          = 0x27,
	FACTORY_NEC_VOL_LEFT      = 0x5D,
	FACTORY_NEC_VOL_RIGHT     = 0x5E,
	FACTORY_NEC_OK            = 0x0E,
	FACTORY_NEC_CC_TT         = 0x4B,
	FACTORY_NEC_MUTE          = 0x37,
	FACTORY_NEC_PIC           = 0x46,
	FACTORY_NEC_LOG_LED       = 0x75,
	FACTORY_NEC_AUDIO         = 0x53,
	FACTORY_NEC_2D_3D         = 0x22,
	FACTORY_NEC_ARC           = 0x23,
	FACTORY_NEC_CI_PLUS       = 0x24,
	FACTORY_NEC_VIRGIN        = 0x51,
	FACTORY_NEC_CVBS          = 0x00,
	FACTORY_NEC_YPBPR_SCART   = 0x01,
	FACTORY_NEC_HDMI          = 0x02,
	FACTORY_NEC_VGA           = 0x7B,
	FACTORY_NEC_REGIN         = 0x74,
	FACTORY_NEC_CLONE         = 0x72,
	FACTORY_NEC_CLK           = 0x73,
	FACTORY_NEC_DCR           = 0x34,
	FACTORY_NEC_ADC           = 0x31,
	FACTORY_NEC_BLK           = 0x0D,
	FACTORY_NEC_W_P           = 0x69,
	FACTORY_NEC_LIGHT_SENSOR  = 0x25,
	FACTORY_NEC_USB           = 0x70,
	FACTORY_NEC_RJ45          = 0x71,
	FACTORY_NEC_RS232         = 0x28,
	FACTORY_NEC_BLANK_BUTTON  = 0x29,
	FACTORY_NEC_MAX_CODE
} RC_FACTORY_NEC_CODE;


const uint8_t code NEC_FACTORY_LUT[] = {  FACTORY_NEC_MUTE    		, KEY_Mute    ,  
	                                        FACTORY_NEC_MENU    	, KEY_Menu    ,
	                                        FACTORY_NEC_UP      	, KEY_Up      ,
	                                        FACTORY_NEC_VOL_RIGHT  	, KEY_Vol_Up  ,
	                                        FACTORY_NEC_OK 			, KP_Enter    ,
	                                        FACTORY_NEC_DOWN    	, KEY_Down    ,
	                                        FACTORY_NEC_VOL_LEFT	, KEY_Vol_Down,
	                                        FACTORY_NEC_1       	, KP_1        ,
	                                        FACTORY_NEC_2       	, KP_2        ,
	                                        FACTORY_NEC_3       	, KP_3        ,
	                                        FACTORY_NEC_CH_MINUS   	, KEY_PageUp  ,
	                                        FACTORY_NEC_4       	, KP_4        ,
	                                        FACTORY_NEC_5       	, KP_5        ,
	                                        FACTORY_NEC_6       	, KP_6        ,
	                                        FACTORY_NEC_CH_PLUS 	, KEY_PageDown,
	                                        FACTORY_NEC_7       	, KP_7        ,
	                                        FACTORY_NEC_8       	, KP_8        ,
	                                        FACTORY_NEC_9       	, KP_9        ,
	                                        FACTORY_NEC_0       	, KP_0        ,
	                                        NEC_LEFT    , KEY_Left    ,
	                                        NEC_RIGHT   , KEY_Right    };

const uint8_t  code NEC_FACTORY_LUT_CUSTOM_CODE_0 = 0x20; 
const uint8_t  code NEC_FACTORY_LUT_CUSTOM_CODE_1 = 0x08; 



// Do not change code below
const uint16_t code RC6_LUT_SIZE = (sizeof(RC6_LUT)/sizeof(uint8_t));
const uint16_t code NEC_LUT_SIZE = (sizeof(NEC_LUT)/sizeof(uint8_t));
const uint16_t code NEC_FACTORY_LUT_SIZE = (sizeof(NEC_FACTORY_LUT)/sizeof(uint8_t));

