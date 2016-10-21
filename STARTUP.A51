$NOMOD51
$include (GrobalDefine.inc)
;------------------------------------------------------------------------------
;  This file is part of the C51 Compiler package
;  Copyright (c) 1988-2002 Keil Elektronik GmbH and Keil Software, Inc.
;------------------------------------------------------------------------------
;  STARTUP.A51:  This code is executed after processor reset.
;
;  To translate this file use A51 with the following invocation:
;
;     A51 STARTUP.A51
;
;  To link the modified STARTUP.OBJ file to your application use the following
;  BL51 invocation:
;
;     BL51 <your object file list>, STARTUP.OBJ <controls>
;
;------------------------------------------------------------------------------
;
;  User-defined Power-On Initialization of Memory
;
;  With the following EQU statements the initialization of memory
;  at processor reset can be defined:
;
;               ; the absolute start-address of IDATA memory is always 0
IDATALEN        EQU     256      ; the length of IDATA memory in bytes.
;
XDATASTART      EQU     0H    ; the absolute start-address of XDATA memory
XDATALEN        EQU     0H      ; the length of XDATA memory in bytes.
;
PDATASTART      EQU     0H      ; the absolute start-address of PDATA memory
PDATALEN        EQU     0H      ; the length of PDATA memory in bytes.
;
;  Notes:  The IDATA space overlaps physically the DATA and BIT areas of the
;          8051 CPU. At minimum the memory space occupied from the C51
;          run-time routines must be set to zero.
;------------------------------------------------------------------------------
;
;  Reentrant Stack Initilization
;
;  The following EQU statements define the stack pointer for reentrant
;  functions and initialized it:
;
;  Stack Space for reentrant functions in the SMALL model.
IBPSTACK        EQU     0       ; set to 1 if small reentrant is used.
IBPSTACKTOP     EQU     0FFH+1  ; set top of stack to highest location+1.
;
;  Stack Space for reentrant functions in the LARGE model.
XBPSTACK        EQU     0       ; set to 1 if large reentrant is used.
XBPSTACKTOP     EQU     0FFFFH+1; set top of stack to highest location+1.
;
;  Stack Space for reentrant functions in the COMPACT model.
PBPSTACK        EQU     0       ; set to 1 if compact reentrant is used.
PBPSTACKTOP     EQU     0FFFFH+1; set top of stack to highest location+1.
;
;------------------------------------------------------------------------------
;
;  Page Definition for Using the Compact Model with 64 KByte xdata RAM
;
;  The following EQU statements define the xdata page used for pdata
;  variables. The EQU PPAGE must conform with the PPAGE control used
;  in the linker invocation.
;
PPAGEENABLE     EQU     0       ; set to 1 if pdata object are used.
;
PPAGE           EQU     0       ; define PPAGE number.
;
PPAGE_SFR       DATA    0A0H    ; SFR that supplies uppermost address byte
;               (most 8051 variants use P2 as uppermost address byte)
;

WT6575F	EQU	(0FFH-ICE)

;------------------------------------------------------------------------------
; Standard SFR Symbols
SP		DATA	81H
DPL		DATA	82H
DPH		DATA	83H
P2		DATA	0A0H
IE		DATA	0A8H
IP		DATA	0B8H
PSW		DATA	0D0H
ACC		DATA	0E0H
B		DATA	0F0H
;

;
;------------------------------------------------------------------------------
IF	ICE
		NAME	?C_STARTUP
?C_C51STARTUP	SEGMENT	CODE
?STACK		SEGMENT	IDATA

		RSEG	?STACK
		DS	1

		EXTRN CODE (?C_START)
;		EXTRN CODE (MAIN)
		PUBLIC	?C_STARTUP

		CSEG	AT 00H
?C_STARTUP:	LJMP	STARTUP1

		RSEG	?C_C51STARTUP
STARTUP1:
		;stack point
		MOV	SP,#?STACK-1

	IF IDATALEN <> 0
		MOV	R0,#IDATALEN - 1
		CLR	A
IDATALOOP:	MOV	@R0,A
		DJNZ	R0,IDATALOOP
	ENDIF

	IF XDATALEN <> 0
		MOV	DPTR,#XDATASTART
		MOV	R7,#LOW (XDATALEN)
		IF (LOW (XDATALEN)) <> 0
			MOV	R6,#(HIGH (XDATALEN)) +1
		ELSE
			MOV	R6,#HIGH (XDATALEN)
		ENDIF

		CLR	A
XDATALOOP:	MOVX	@DPTR,A
		INC	DPTR
		DJNZ	R7,XDATALOOP
		DJNZ	R6,XDATALOOP
	ENDIF

	IF PPAGEENABLE <> 0
		MOV	PPAGE_SFR,#PPAGE
	ENDIF

	IF PDATALEN <> 0
		MOV	R0,#LOW (PDATASTART)
		MOV	R7,#LOW (PDATALEN)
		CLR	A
PDATALOOP:	MOVX	@R0,A
		INC	R0
		DJNZ	R7,PDATALOOP
	ENDIF

	IF IBPSTACK <> 0
		EXTRN DATA	(?C_IBP)
		MOV	?C_IBP,#LOW IBPSTACKTOP
	ENDIF

	IF XBPSTACK <> 0
		EXTRN DATA	(?C_XBP)
		MOV	?C_XBP,#HIGH XBPSTACKTOP
		MOV	?C_XBP+1,#LOW XBPSTACKTOP
	ENDIF

	IF PBPSTACK <> 0
		EXTRN DATA	(?C_PBP)
		MOV	?C_PBP,#LOW PBPSTACKTOP
	ENDIF


		LJMP	?C_START
;		LJMP	MAIN
;******************************************************************************
;------------------------------------------------------------------------------
	CSEG AT 3FFAH
	DB	CODE_ENCRYPTION, READ_PROTECT, 55H, 0AAH, 33H, 0CCH
ENDIF

IF	WT6575F
	CSEG AT 3FFAH
	DB 	CODE_ENCRYPTION, READ_PROTECT, 55H, 0AAH, 33H, 0CCH
ENDIF

END
