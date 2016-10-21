;------------------------------------------------------------------------------
; ICE6573.ASM - WT6573F Soft-ICE module
;------------------------------------------------------------------------------
$NOMOD51
$include (GrobalDefine.inc)

;------------------------------------------------------------------------------
; Standard SFR Symbols
SP              DATA    81H
DPL             DATA    82H
DPH             DATA    83H
P2              DATA    0A0H
IE              DATA    0A8H
IP              DATA    0B8H
PSW             DATA    0D0H
ACC             DATA    0E0H
B               DATA    0F0H
;
ICE_PCL         DATA    0F1H
ICE_PCH         DATA    0F2H
ICE_CTL         DATA    0F8H
ICE_DAT         DATA    0F9H

; ISR Status bits
FB              EQU     07H
RD              EQU     06H
WR              EQU     05H
EXIT            EQU     04H
RY              EQU     03H
ENTICE          EQU     02H

;
;------------------------------------------------------------------------------
WDTRST          EQU     07H
WDTEXT          EQU     22H

IF	ICE
DISABLE_WATCHDOG MACRO

; WT6573F : After device reset, the hardware watchdog is cleared and disabled.
;               MOV     R0,#WDTRST
;               MOV     A,#0AAH         ; clean and enable the WDT
;               MOVX    @R0,A
                MOV     R0,#WDTEXT
                MOV     A,#40H
                MOVX    @R0,A
                MOV     A,#80H
                MOVX    @R0,A
                ENDM
;------------------------------------------------------------------------------
                CSEG    AT 07BH
                LJMP    NMI

E5_OP_CODE:
                NOP
                MOV     A, ICE_DAT              ; GET E5 OPERAND, E5_OP_CODE_ADR = 0X0080
                LJMP    E5_OP_CODE_CONT
F5_OP_CODE:
                NOP
                MOV     ICE_DAT, A              ; GET F5 OPERAND, F5_OP_CODE_ADR = 0X0086
                LJMP    F5_OP_CODE_CONT

;------------------------------------------------------------------------------
;               CSEG    AT ICE_ISR_STRAT        ; 1000H
                PUBLIC  NMI
NMI_ISR SEGMENT CODE
        RSEG NMI_ISR
NMI:
                PUSH    ACC
                PUSH    B
                PUSH    DPH
                PUSH    DPL
                PUSH    PSW
                PUSH    00H                     ; POP R0

PUSHED_REG_SIZE EQU ($ - NMI) / 2
                
                MOV     PSW,#00H                ; SELECT REGISTER BANK0

                ; modify PC in STACK if needed ( pc_stack != user interrupt vector )
                MOV     A,SP
                CLR     C
                SUBB    A,#(PUSHED_REG_SIZE + 0) ; &PCH
                MOV     R0,A
                CJNE    @R0,#0,NMI_NOT_VECTOR   ; check whether or not INT before NMI
                DEC     R0                      ; &PCL
                MOV     A,@R0
                INC     R0
                ANL     A,#87H          ; 03,0B,13,1B,...73 are vectors
                CJNE    A,#03H,NMI_NOT_VECTOR
                JMP     NMI_AFTER_INT
NMI_NOT_VECTOR:         
                ; RESTORE PCH
                MOV     @R0,ICE_PCH
                DEC     R0
                ; RESTORE PCL
                MOV     @R0,ICE_PCL
NMI_AFTER_INT:

                ; DISABLE WATCHDOG
                DISABLE_WATCHDOG

                ; SET ICE READY
                SETB    ICE_CTL.RY
                ; WAIT FOR ICE COMMAND

;******************************************************************************

MAIN_LOOP:
                ; CHECK FB & WR BITS 
                MOV     A, #10100000B
                ANL     A, ICE_CTL
                CJNE    A, #10100000B, MAIN_LOOP

                ; DECODER COMMANDS
                MOV     A, ICE_DAT
                CJNE    A, #000H, ICE_COMMAND_IS_NOT_00
                JMP     GO_END

ICE_COMMAND_IS_NOT_00:
                CJNE    A, #001H, ICE_COMMAND_IS_NOT_01
                JMP     DO_COMMAND_01

ICE_COMMAND_IS_NOT_01:
                CJNE    A, #002H, ICE_COMMAND_IS_NOT_02
                JMP     DO_COMMAND_02

ICE_COMMAND_IS_NOT_02:
                CJNE    A, #003H, ICE_COMMAND_IS_NOT_03
                JMP     DO_COMMAND_03

ICE_COMMAND_IS_NOT_03:
                CJNE    A, #004H, ICE_COMMAND_IS_NOT_04
                JMP     DO_COMMAND_04

ICE_COMMAND_IS_NOT_04:
                CJNE    A, #005H, ICE_COMMAND_IS_NOT_05
                JMP     DO_COMMAND_05

ICE_COMMAND_IS_NOT_05:
                CJNE    A, #006H, ICE_COMMAND_IS_NOT_06
                JMP     DO_COMMAND_06

ICE_COMMAND_IS_NOT_06:
                CJNE    A, #007H, ICE_COMMAND_IS_NOT_07
                JMP     DO_COMMAND_07

ICE_COMMAND_IS_NOT_07:
                CJNE    A, #016H, ICE_COMMAND_IS_NOT_16
                JMP     DO_COMMAND_16

ICE_COMMAND_IS_NOT_16:
                CJNE    A, #017H, ICE_COMMAND_IS_NOT_17
                JMP     DO_COMMAND_17

ICE_COMMAND_IS_NOT_17:
                JMP     MAIN_LOOP

;============================================================================
                ; DO COMMANDS
DO_COMMAND_16:
                ; DO BLOCK READ XDATA
                JNB     ICE_CTL.WR, $
                MOV     DPH, ICE_DAT            ; SET DPH
                JNB     ICE_CTL.WR, $
                MOV     DPL, ICE_DAT            ; SET DPL
DO_COMMAND_16_CALL:
                MOVX    A, @DPTR
                MOV     ICE_DAT, A
                INC     DPTR
DO_COMMAND_16_CHK_RD:
                JB      ICE_CTL.RD, DO_COMMAND_16_CKH_EXIT
                SJMP    DO_COMMAND_16_CALL
DO_COMMAND_16_CKH_EXIT:
                JB      ICE_CTL.EXIT, DO_COMMAND_EXIT 
                SJMP    DO_COMMAND_16_CHK_RD

DO_COMMAND_17:
                ; DO BLOCK WRITE XDATA
                JNB     ICE_CTL.WR, $
                MOV     DPH, ICE_DAT            ; SET DPH
                JNB     ICE_CTL.WR, $
                MOV     DPL, ICE_DAT            ; SET DPL
DO_COMMAND_17_CHK_WR:
                JNB     ICE_CTL.WR, DO_COMMAND_17_CHK_EXIT
                MOV     A, ICE_DAT
                MOVX    @DPTR, A
                INC     DPTR
                SJMP    DO_COMMAND_17_CHK_WR
DO_COMMAND_17_CHK_EXIT:
                JB      ICE_CTL.EXIT, DO_COMMAND_EXIT
                SJMP    DO_COMMAND_17_CHK_WR

DO_COMMAND_EXIT:
                JMP     MAIN_LOOP

DO_COMMAND_01:
                ; DO READ PDATA
                JNB     ICE_CTL.WR, $
                MOV     DPH, ICE_DAT            ; SET DPH
                JNB     ICE_CTL.WR, $
                MOV     DPL, ICE_DAT            ; SET DPL
                MOV     A, #000H
                MOVC    A, @A+DPTR
                MOV     ICE_DAT, A
                JMP     MAIN_LOOP

DO_COMMAND_02:
                ; DO READ IDATA
                JNB     ICE_CTL.WR, $
                MOV     R0, ICE_DAT
                MOV     ICE_DAT, @R0
                JMP     MAIN_LOOP

DO_COMMAND_03:
                ; DO WRITE IDATA
                JNB     ICE_CTL.WR, $
                MOV     R0, ICE_DAT
                JNB     ICE_CTL.WR, $
                MOV     @R0, ICE_DAT
                JMP     MAIN_LOOP

DO_COMMAND_04:
                ; DO READ SFR
                JNB     ICE_CTL.WR, $
                LJMP    E5_OP_CODE
E5_OP_CODE_CONT:
                MOV     ICE_DAT, A              ; GET SFR DATA
                JMP     MAIN_LOOP

DO_COMMAND_05:
                ; DO WRITE SFR
                JNB     ICE_CTL.WR, $
                
                MOV     A, ICE_DAT              ; READ DATA
                JNB     ICE_CTL.WR, $
                LJMP    F5_OP_CODE
F5_OP_CODE_CONT:
                JMP     MAIN_LOOP

DO_COMMAND_06:
                ; DO READ XDATA
                JNB     ICE_CTL.WR, $
                MOV     DPH, ICE_DAT            ; SET DPH
                JNB     ICE_CTL.WR, $
                MOV     DPL, ICE_DAT            ; SET DPL
                MOVX    A, @DPTR
                MOV     ICE_DAT, A
                JMP     MAIN_LOOP

DO_COMMAND_07:
                ; DO WRITE XDATA
                JNB     ICE_CTL.WR, $
                MOV     DPH, ICE_DAT            ; SET DPH
                JNB     ICE_CTL.WR, $
                MOV     DPL, ICE_DAT            ; SET DPL
                JNB     ICE_CTL.WR, $
                MOV     A, ICE_DAT
                MOVX    @DPTR, A
                JMP     MAIN_LOOP

GO_END:
                CLR     ICE_CTL.RY
                
                ; POP BACKUP
                POP     00H                     ; POP R0
                POP     PSW
                POP     DPL
                POP     DPH
                POP     B
                POP     ACC
END_ICE_ROUTINE:
                RETI
;------------------------------------------------------------------------------
;       CSEG AT 3
;       DS      (7BH-3)         ; reserve code space for ICE
R_N     equ     2               ; number of reserve bytes in front of any vector
        CSEG AT 0BH-R_N
        DS      R_N
        CSEG AT 13H-R_N
        DS      R_N
        CSEG AT 1BH-R_N
        DS      R_N
        CSEG AT 23H-R_N
        DS      R_N
        CSEG AT 2BH-R_N
        DS      R_N
        CSEG AT 33H-R_N
        DS      R_N
        CSEG AT 3BH-R_N
        DS      R_N
        CSEG AT 43H-R_N
        DS      R_N
        CSEG AT 4BH-R_N
        DS      R_N
        CSEG AT 53H-R_N
        DS      R_N
        CSEG AT 5BH-R_N
        DS      R_N
        CSEG AT 63H-R_N
        DS      R_N
        CSEG AT 6BH-R_N
        DS      R_N
        CSEG AT 73H-R_N
        DS      R_N
        CSEG AT 7BH-R_N
        DS      R_N       
;------------------------------------------------------------------------------
ELSE
	CSEG	AT	0080h
	NOP
ENDIF
        END


