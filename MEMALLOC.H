/********************************************************************************/
/*	Weltrend Semiconductor, Inc.												*/
/*	Header file for WT6575 USB microcontroller.									*/
/*	Copyright (c) Weltrend Semiconductor, Inc.									*/
/*	All rights reserved.														*/
/********************************************************************************/

#ifndef	_MEMALLOC_H
#define _MEMALLOC_H
#include "GrobalDefine.h"
//=============================================================
// Type Define
//=============================================================
typedef bit	BOOL;

//=============================================================
//WT6575F SFR (8052 Extensions)
//=============================================================
sfr	XICON=0xC0;
sfr SPIDR1=0xDC;
sfr SPIDR2=0xDD;
sfr SPIDR3=0xDE;
sfr SPIDR4=0xDF;
sfr SPICR1=0xE6;
#define IE		(1<<7)
#define SPIEN	(1<<6)
#define LEN(d)	(d<<4)
#define CPOL	(1<<3)
#define CPHA	(1<<2)
#define MSTR	(1<<1)
#define LSB		(1<<0)

sfr SPICR2=0xE7;
#define RXOV	(1<<7)
#define RXDR	(1<<6)
#define TRDE	(1<<5)
#define DRCLR	(1<<4)
#define SWAI	(1<<3)
#define SPR(d)	(d<<0)


//=============================================================
//WT6575F XFR
//=============================================================
extern unsigned char pdata FADDR;	//USB device address register(0x00~0x7f address value).
extern unsigned char pdata USBFI;	//USB function interrupt flag register, after read those flag will be clear.
extern unsigned char pdata USBFIE;	//USB function interrupt enable register.
#define	USB_RST_INT		0x20	// USBFI & USBFIE register USB Reset interrupt flag/enable bit.
#define	USB_EP3_INT		0x10	// USBFI & USBFIE register Endpoint 3 interrupt flag/enable bit.
#define USB_EP2_INT		0x08	// USBFI & USBFIE register Endpoint 2 interrupt flag/enable bit.
#define USB_EP1_INT		0x04	// USBFI & USBFIE register Endpoint 1 interrupt flag/enable bit.
#define USB_EP0_RX_INT	0x02	// USBFI & USBFIE register Endpoint 0 OUT/SETUP TOKEN interrupt flag/enable bit.
#define	USB_EP0_TX_INT	0x01	// USBFI & USBFIE register Endpoint 0 IN TOKEN interrupt flag/enable bit.
extern unsigned char pdata SIEI;	//USB SIE interface control register.
#define	USB_FSEN		0x80	// Full speed enable
#define	USB_RST_L		0x40	// USB RESET FLAG
#define	USB_V33EN		0x20	// USB Upstream Tranceiver Port enable/disable control bit, firmware clear ro set.
#define	USB_DM			0x10	// USB D- pin pull-up 1.5Kohm resistor enable/disable control bit, firmware clear ro set.
#define	USB_RSTEN		0x04	// USB Reset Signal to clear microcontroller enable/disable control bit, firmware clear ro set.
#define USB_DP			0x02	// USB D+ pin pull-up 1.5Kohm resistor enable/disable control bit, firmware clear ro set.
#define	USB_WAKEUP		0x01	// USB WakeUp Signal output control bit, set by firmware clear by hardware.

#define	USB_ENABLE		0xA0
#define DP_PULL			0xA2
#define	vUSBWAKEUP	(USB_WAKEUP)|( USB_DP)|( USB_FSEN)|( USB_V33EN)
extern unsigned char pdata MISC;	//PLL and Timer base Control register
#define LMOD			(0x01<<2)
#define FTBM			(0x01<<3)
extern unsigned char pdata EPINDEX;	//Endpoint Bank control register(0x00~0x03 value)
#define EP0				0x00	// Endpoint 0 index number
#define EP1				0x01	// Endpoint 1 index number
#define EP2				0x02	// Endpoint 2 index number
#define EP3				0x03	// Endpoint 3 index number
extern unsigned char pdata EPCON;	//Endpoint transfer type control register, index by EPINDEX.
#define RXSTL			0x80	// Endpoint stall receive control bit, after STALL handahake this bit will be clear for EP0 only.
#define TXSTL			0x40	// Endpoint stall transmit control bit, after STALL handahake this bit will be clear for EP0 only.
#define CTLEP			0x20	// Control Endpoint bit, EP0 index only. Set this bit to capable and receiving SETUP token.
#define RXIE			0x08	// Receive Input Enable bit, set this bit to receive endpoint's data to FIFO.
#define RXEPEN			0x04	// Receive Endpoint Enable bit, set this bit to enable endpoint receive.
#define TXOE			0x02	// Transmit Output Enable bit, set this bit to transmit endpoint's data from FIFO.
#define TXEPEN			0x01	// Transmit Endpoint Enable bit, set this bit to enable endpoint transmit.
#define	STALL_EP		(RXSTL|TXSTL)	// Define Control Endpoint stall type operation configures.
#define	CRTL_EP			(CTLEP|RXIE|RXEPEN|TXOE|TXEPEN)	// Define Control Endpoint control type operation configures.
#define	CRTL_EP_STALL	(RXSTL|TXSTL|CTLEP|RXIE|RXEPEN|TXOE|TXSPEN)	// Define Control Endpoint control type stall configures.
#define IN_EP			(TXOE|TXEPEN)	// Define IN Endpoint bulk/interrupt type operation configures.
#define	OUT_EP			(RXIE|RXEPEN)	// Define OUT Endpoint bulk/interrupt type operation configures.
#define	IN_EP_STALL		(TXSTL|TXOE|TXEPEN)	// Define IN Endpoint bulk/interrupt type stall configures.
#define	OUT_EP_STALL	(RXSTL|RXIE|RXEPEN)	// Define OUT Endpoint bulk/interrupt type stall configures.
extern unsigned char pdata WDTRST;	//Watch-Dog-Reset enable/disable/clear control register, write only.
#define	WDT_ENABLE		0xaa	// Write this value to enable Watch-Dog-Reset timer.
#define	WDT_DISABLE		0x00	// Write this value to disable Watch-Dog-Reset timer.
#define WDT_CLEAR		0xaa	// Write this value to clear Watch-Dog-Reset timer.
extern unsigned char pdata TXDAT;	//Endpoint transmit FIFO data register, index by EPINDEX. Max.64bytes, write only.
										//Write data to this register, Ther write point is incremented automatically after write.
extern unsigned char pdata TXCON;	//Endpoint transmit FIFO control register, index by EPINDEX, write only.
#define TX_FIFO_CLEAR	0x80	// Endpoint transmit FIFO clear bit, set this to clear FIFO clear by hardware.
extern unsigned char pdata TXFLG;	//Endpoint transmit FIFO flag register, index by EPINDEX.
#define	TX_FIFO_EMPTY	0x08	// Transmit FIFO empty flag, read only.
#define	TX_FIFO_FULL	0x04	// Transmit FIFO full flag, read only.
#define	TX_FIFO_UNDER	0x02	// Transmit FIFO under run flag, read/clear only.
#define	TX_FIFO_OVER	0x01	// Transmit FIFO over run flag, read/clear only.
#define	TX_FIFO_NULL	0x00	// Transmit FIFO null status.
extern unsigned char pdata TXCNT;	//Endpoint transmit FIFO length count register, index by EPINDEX. Max.64bytes, write only.
										// Setting transmit data length to this register, after writed data to TXDAT.
extern unsigned char pdata TXSTAT;	//Endpoint transmit FIFO status register, index by EPINDEX.
#define	TX_SEQUENCE		0x80	// Transmit current sequence bit, read/clear only.
#define	TX_VOID			0x04	// Transmit void bit, read only.
#define	TX_ERROR		0x02	// Transmit error bit, read only.
#define	TX_ACK			0x01	// Transmit acknowledge bit, read only.
extern unsigned char pdata NOTEBOOK;	//Notebook state register, for firmware set or clear state. Reset state by power-on reset.
extern unsigned char pdata USBFI2;	//USB function interrupt flag register, read and clear by firmware.
#define EXINT			(1<<3)
#define	USB_RESUME_INT	0x40	// USB Resume signal interrupt flag.
#define	USB_SUSPEND_INT	0x20	// USB Suspend signal interrupt flag.
#define	USB_REMOTE_INT	0x10	// USB Remote Wakeup interrupt flag.
extern unsigned char pdata USBFIE2;	//USB function interrupt enable register.
#define	USB_NAKINT_IE	0x80	// USB reply NAK interrupt enable.
#define	USB_RESUME_IE	0x40	// USB Resume signal interrupt enable.
#define	USB_SUSPEND_IE	0x20	// USB Suspend signal interrupt enable.
#define	USB_STALL_IE	0x10	// USB reply STALL interrupt enable.
#define	USB_TGERROR_IE	0x08	// USB Data toggle interrupt enable.
extern unsigned char pdata EPREP;	//Endpoint replace register
extern unsigned char pdata TXEN;	//
extern unsigned char pdata TRIM33;
extern unsigned char pdata RXDAT;	//Endpoint receive FIFO data register, index by EPINDEX. Max.64bytes, read only.
											//Read data from this register, Ther read point is incremented automatically after read.
extern unsigned char pdata RXCON;	//Endpoint receive FIFO control register, index by EPINDEX, write only.
#define	RX_FIFO_CLEAR	0x90	// Endpoint receive FIFO clear bit, set this to clear FIFO clear by hardware.
#define	RXFFRC		0x10
extern unsigned char pdata RXFLG;	//Endpoint receive FIFO flag register, index by EPINDEX.
#define	RX_NAK			0x80	// This flag indicates a NAK is returned to host while USB_NAKINT_IE was set, read/clear only.
#define	RX_STALL		0x40	// This flag indicates a STALL is returned to host while USB_STALL_IE was set, read/clear only.
#define	RX_TGERROR		0x20	// This flag indicates data toggle error is found while USB_TGERROR_IE was set, read/clear only.
#define	RX_EMPTY		0x08	// Receive FIFO empty flag, read only.
#define	RX_FULL			0x04	// Receive FIFO full flag, read only.
#define	RX_UNDER		0x02	// Receive FIFO under-run flag, read/clear only.
#define	RX_OVER			0x01	// Receive FIFO over-run flag, read/clear only.
extern unsigned char pdata RXCNT;	//Endpoint receive FIFO length count register, index by EPINDEX. Max.64bytes, read only.
										//This register store data length of receive FIFO, after receive data rady.
extern unsigned char pdata RXSTAT;	//Endpoint receive FIFO status register, index by EPINDEX.
#define	RX_SEQUENCE		0x80	// Receive endpoint sequence bit, read/clear only.
#define	RX_SETUP		0x40	// Receive SETUP Token bit, read/clear only.
#define	RX_STOVW		0x20	// Receive start over-write bit, read/clear only.
#define	RX_EDOVW		0x10	// Receive end over-write bit, read/clear only.
#define	RX_VOID			0x04	// Receive void bit, read only.
#define	RX_ERROR		0x02	// Receive error bit, read only.
#define	RX_ACK			0x01	// Receive acknowledge bit, read only.
extern unsigned char pdata PA;	//I/O Port A data, read/write.
extern unsigned char pdata EXINTEN;	//External I/O Port (PC) interrupt rising/falling edge trigger enable register.
extern unsigned char pdata WDTEXT;	//Extension Watch-Dog timer control register(0x00~0x3f value), 10.922ms unit.
extern unsigned char pdata RMPEN;	//I/O Port remote-wakeup enable/disable control register.
#define RMP_DISABLE			0x00
#define RMP_PAEN			0x01
#define RMP_PBEN			0x02
#define RMP_PCEN			0x04
extern unsigned char pdata PWM0;		//PWM0 Duty Setting
extern unsigned char pdata PWM1;		//PWM1 Duty Setting
extern unsigned char pdata PWM2;		//PWM2 Duty Setting
extern unsigned char pdata PWM_CTL;		//PWM Control register
extern unsigned char pdata PUPCTRLB;	//PB Programable Pull-up
extern unsigned char pdata PAOE;	//PA output/input (1/0) control register.
extern unsigned char pdata TBCR;		//Time base control register
extern unsigned char pdata ENCALIB;		//RC auto calibration Enable register
extern unsigned char pdata SYSCR1;		//System control register1
extern unsigned char pdata SYSCR2;		//System control register2
extern unsigned char pdata PLL_CTL;	//Reserved register can't read/write
extern unsigned char pdata EER_EN1;		//EEPROM function enable data1(0x0A)
extern unsigned char pdata EER_EN2;		//EEPROM function enable data1(0x05)
extern unsigned char pdata EER_ADDRL;	//EEPROM Address[7:0]
extern unsigned char pdata EER_ADDRH;	//EEPROM Address[10:8]
extern unsigned char pdata EER_CTRL;	//EEPROM function enable
extern unsigned char pdata EER_DATA0;	//EEPROM Data0
extern unsigned char pdata EER_DATA1;	//EEPROM Data1
extern unsigned char pdata EER_DATA2;	//EEPROM Data2
extern unsigned char pdata EER_DATA3;	//EEPROM Data3
extern unsigned char pdata EER_DATA4;	//EEPROM Data4
extern unsigned char pdata EER_DATA5;	//EEPROM Data5
extern unsigned char pdata EER_DATA6;	//EEPROM Data6
extern unsigned char pdata EER_DATA7;	//EEPROM Data7



#endif
