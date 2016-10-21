;-----------------------------------------------------------------------------;
; Copyright:								      ;
;									      ;
;       Copyright(C) 2009                        ;
;	All rights reserved.						      ;
;									      ;
; Filename:								      ;
;	GrobalDefine.inc							      ;
;									      ;
; History:								      ;
;									      ;
;-----------------------------------------------------------------------------;

;===========================================================
; SET ICE ENABLE/DISABLE
;===========================================================
ICE	EQU	0FFH	;ff: ENABLE ICE
;ICE	EQU	00H	;00: DISABLE ICE

;===========================================================
; SET FLASH ROM READ PROTECT
;===========================================================
;READ_PROTECT	EQU	00H	;ENABLE READ PROTECT
READ_PROTECT	EQU	0FFH	;DISABLE READ PROTECT

;===========================================================
; SET CODE ENCRYPTION
;===========================================================
;CODE_ENCRYPTION	EQU	0AAH	;ENABLE CODE ENCRYPTION
CODE_ENCRYPTION	EQU	0FFH	;DISABLE CODE ENCRYPTION

