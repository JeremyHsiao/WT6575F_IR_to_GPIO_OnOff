//=============================================================================//
// Copyright:															       //
//											      	        				   //
//       Copyright(C) 2008                          						   //
//	All rights reserved.						      						   //
//									      									   //
// Filename:								      							   //
//	XFR.H								      							   //
//									      									   //
// History:								      								   //
//									       									   //
//=============================================================================//

#ifndef _XFR_H
#define _XFR_H

#include <absacc.h>
#include <REG52.H>
#include <INTRINS.H>

///=======================================================================
// XFR Special Registers
//=======================================================================
#ifdef _DEFINE_XFR_VARIABLE

  volatile unsigned char pdata FADDR		_at_ 0x00;	//USB device address register(0x00~0x7f address value).
  volatile unsigned char pdata USBFI		_at_ 0x01;	//USB function interrupt flag register, after read those flag will be clear.
  volatile unsigned char pdata USBFIE		_at_ 0x02;	//USB function interrupt enable register.
  volatile unsigned char pdata SIEI			_at_ 0x03;	//USB SIE interface control register.
  volatile unsigned char pdata MISC			_at_ 0x04;	//PLL and Timer base Control register
  volatile unsigned char pdata EPINDEX		_at_ 0x05;	//Endpoint Bank control register(0x00~0x03 value)
  volatile unsigned char pdata EPCON		_at_ 0x06;	//Endpoint transfer type control register, index by EPINDEX.
  volatile unsigned char pdata WDTRST		_at_ 0x07;	//Watch-Dog-Reset enable/disable/clear control register, write only.
  volatile unsigned char pdata TXDAT		_at_ 0x08;	//Endpoint transmit FIFO data register, index by EPINDEX. Max.64bytes, write only.
  							//Write data to this register, Ther write point is incremented automatically after write.
  volatile unsigned char pdata TXCON		_at_ 0x09;	//Endpoint transmit FIFO control register, index by EPINDEX, write only.
  volatile unsigned char pdata TXFLG		_at_ 0x0a;	//Endpoint transmit FIFO flag register, index by EPINDEX.
  volatile unsigned char pdata TXCNT		_at_ 0x0b;	//Endpoint transmit FIFO length count register, index by EPINDEX. Max.64bytes, write only.
  							// Setting transmit data length to this register, after writed data to TXDAT.
  volatile unsigned char pdata TXSTAT		_at_ 0x0c;	//Endpoint transmit FIFO status register, index by EPINDEX.
  volatile unsigned char pdata NOTEBOOK		_at_ 0x0f;	//Notebook state register, for firmware set or clear state. Reset state by power-on reset.
  volatile unsigned char pdata USBFI2		_at_ 0x11;	//USB function interrupt flag register, read and clear by firmware.
  volatile unsigned char pdata USBFIE2		_at_ 0x12;	//USB function interrupt enable register.
  volatile unsigned char pdata EPREP		_at_ 0x13;	//Endpoint replace register
  volatile unsigned char pdata TXEN			_at_ 0x14;	//
  volatile unsigned char pdata TRIM33		_at_ 0x15;	//
  
  volatile unsigned char pdata RXDAT		_at_ 0x18;	//Endpoint receive FIFO data register, index by EPINDEX. Max.64bytes, read only.
  							                                  //Read data from this register, Ther read point is incremented automatically after read.
  volatile unsigned char pdata RXCON		_at_ 0x19;	//Endpoint receive FIFO control register, index by EPINDEX, write only.
  volatile unsigned char pdata RXFLG		_at_ 0x1a;	//Endpoint receive FIFO flag register, index by EPINDEX.
  volatile unsigned char pdata RXCNT		_at_ 0x1b;	//Endpoint receive FIFO length count register, index by EPINDEX. Max.64bytes, read only.
  						                                    //This register store data length of receive FIFO, after receive data rady.
  volatile unsigned char pdata RXSTAT		_at_ 0x1c;	//Endpoint receive FIFO status register, index by EPINDEX.
  volatile unsigned char pdata PA			_at_ 0x1d;	//I/O Port A data, read/write.
  volatile unsigned char pdata EXTINTEN		_at_ 0x21;	//External I/O Port (PC) interrupt rising/falling edge trigger enable register.
  volatile unsigned char pdata WDTEXT		_at_ 0x22;	//Extension Watch-Dog timer control register(0x00~0x3f value), 10.922ms unit.
  volatile unsigned char pdata RMPEN		_at_ 0x23;	//I/O Port remote-wakeup enable/disable control register.
  volatile unsigned char pdata PWM0			_at_ 0x24;	//PWM0 Duty Setting
  volatile unsigned char pdata PWM1			_at_ 0x25;	//PWM1 Duty Setting
  volatile unsigned char pdata PWM2			_at_ 0x26;	//PWM2 Duty Setting
  volatile unsigned char pdata PWM_CTL		_at_ 0x28;	//PWM Control register
  volatile unsigned char pdata PUPCTRLB		_at_ 0x2c;	//PB Programable Pull-up
  volatile unsigned char pdata PAOE			_at_ 0x31;	//PA output/input (1/0) control register.
  volatile unsigned char pdata TBCR			_at_ 0x35;	//Time base control register
  volatile unsigned char pdata ENCALIB		_at_ 0x38;	//RC auto calibration Enable register
  volatile unsigned char pdata SYSCR1		_at_ 0x3b;	//System control register1
  volatile unsigned char pdata SYSCR2		_at_ 0x3c;	//System control register2
  volatile unsigned char pdata PLL_CTL		_at_ 0x3d;	//PLL Control register
  volatile unsigned char pdata EER_EN1		_at_ 0x40;	//EEPROM function enable data1(0x0A)
  volatile unsigned char pdata EER_EN2		_at_ 0x41;	//EEPROM function enable data1(0x05)
  volatile unsigned char pdata EER_ADDRL	_at_ 0x42;	//EEPROM Address[7:0]
  volatile unsigned char pdata EER_ADDRH	_at_ 0x43;	//EEPROM Address[10:8]
  volatile unsigned char pdata EER_CTRL		_at_ 0x44;	//EEPROM function enable
  volatile unsigned char pdata EER_DATA0	_at_ 0x48;	//EEPROM Data0
  volatile unsigned char pdata EER_DATA1	_at_ 0x49;	//EEPROM Data1
  volatile unsigned char pdata EER_DATA2	_at_ 0x4A;	//EEPROM Data2
  volatile unsigned char pdata EER_DATA3	_at_ 0x4B;	//EEPROM Data3
  volatile unsigned char pdata EER_DATA4	_at_ 0x4C;	//EEPROM Data4
  volatile unsigned char pdata EER_DATA5	_at_ 0x4D;	//EEPROM Data5
  volatile unsigned char pdata EER_DATA6	_at_ 0x4E;	//EEPROM Data6
  volatile unsigned char pdata EER_DATA7	_at_ 0x4F;	//EEPROM Data7

#else 

  extern volatile unsigned char pdata FADDR		;  //USB device address register(0x00~0x7f address value).
  extern volatile unsigned char pdata USBFI		;  //USB function interrupt flag register, after read those flag will be clear.
  extern volatile unsigned char pdata USBFIE	;  	//USB function interrupt enable register.
  extern volatile unsigned char pdata SIEI		;  //USB SIE interface control register.
  extern volatile unsigned char pdata MISC		;  	//PLL and Timer base Control register
  extern volatile unsigned char pdata EPINDEX	;  	//Endpoint Bank control register(0x00~0x03 value)
  extern volatile unsigned char pdata EPCON		;  //Endpoint transfer type control register, index by EPINDEX.
  extern volatile unsigned char pdata WDTRST	;  	//Watch-Dog-Reset enable/disable/clear control register, write only.
  extern volatile unsigned char pdata TXDAT		;  //Endpoint transmit FIFO data register, index by EPINDEX. Max.64bytes, write only.
                                               			//Write data to this register, Ther write point is incremented automatically after write.
  extern volatile unsigned char pdata TXCON		;  //Endpoint transmit FIFO control register, index by EPINDEX, write only.
  extern volatile unsigned char pdata TXFLG		;  //Endpoint transmit FIFO flag register, index by EPINDEX.
  extern volatile unsigned char pdata TXCNT		;  //Endpoint transmit FIFO length count register, index by EPINDEX. Max.64bytes, write only.
                                                   							// Setting transmit data length to this register, after writed data to TXDAT.
  extern volatile unsigned char pdata TXSTAT	;  	//Endpoint transmit FIFO status register, index by EPINDEX.
  extern volatile unsigned char pdata NOTEBOOK;  	//Notebook state register, for firmware set or clear state. Reset state by power-on reset.
  extern volatile unsigned char pdata USBFI2	;  	//USB function interrupt flag register, read and clear by firmware.
  extern volatile unsigned char pdata USBFIE2	;  	//USB function interrupt enable register.
  extern volatile unsigned char pdata EPREP		;  //Endpoint replace register
  extern volatile unsigned char pdata TXEN		;  //
  extern volatile unsigned char pdata TRIM33	;  //
                                                
  extern volatile unsigned char pdata RXDAT		;  //Endpoint receive FIFO data register, index by EPINDEX. Max.64bytes, read only.
              						                        //Read data from this register, Ther read point is incremented automatically after read.
  extern volatile unsigned char pdata RXCON		;  //Endpoint receive FIFO control register, index by EPINDEX, write only.
  extern volatile unsigned char pdata RXFLG		;  //Endpoint receive FIFO flag register, index by EPINDEX.
  extern volatile unsigned char pdata RXCNT		;  //Endpoint receive FIFO length count register, index by EPINDEX. Max.64bytes, read only.
                					                          //This register store data length of receive FIFO, after receive data rady.
  extern volatile unsigned char pdata RXSTAT	;  	//Endpoint receive FIFO status register, index by EPINDEX.
  extern volatile unsigned char pdata PA			;  //I/O Port A data, read/write.
  extern volatile unsigned char pdata EXTINTEN; 	//External I/O Port (PC) interrupt rising/falling edge trigger enable register.
  extern volatile unsigned char pdata WDTEXT	; 	//Extension Watch-Dog timer control register(0x00~0x3f value), 10.922ms unit.
  extern volatile unsigned char pdata RMPEN		; //I/O Port remote-wakeup enable/disable control register.
  extern volatile unsigned char pdata PWM0		; 	//PWM0 Duty Setting
  extern volatile unsigned char pdata PWM1		; 	//PWM1 Duty Setting
  extern volatile unsigned char pdata PWM2		; 	//PWM2 Duty Setting
  extern volatile unsigned char pdata PWM_CTL	; 	//PWM Control register
  extern volatile unsigned char pdata PUPCTRLB;  	//PB Programable Pull-up
  extern volatile unsigned char pdata PAOE		; //PA output/input (1/0) control register.
  extern volatile unsigned char pdata TBCR		; 	//Time base control register
  extern volatile unsigned char pdata ENCALIB	; 	//RC auto calibration Enable register
  extern volatile unsigned char pdata SYSCR1	;  	//System control register1
  extern volatile unsigned char pdata SYSCR2	;  	//System control register2
  extern volatile unsigned char pdata PLL_CTL	; 	//PLL Control register
  extern volatile unsigned char pdata EER_EN1	; 	//EEPROM function enable data1(0x0A)
  extern volatile unsigned char pdata EER_EN2	; 	//EEPROM function enable data1(0x05)
  extern volatile unsigned char pdata EER_ADDRL;	//EEPROM Address[7:0]
  extern volatile unsigned char pdata EER_ADDRH;	//EEPROM Address[10:8]
  extern volatile unsigned char pdata EER_CTRL	;	//EEPROM function enable
  extern volatile unsigned char pdata EER_DATA0;	//EEPROM Data0
  extern volatile unsigned char pdata EER_DATA1;	//EEPROM Data1
  extern volatile unsigned char pdata EER_DATA2;	//EEPROM Data2
  extern volatile unsigned char pdata EER_DATA3;	//EEPROM Data3
  extern volatile unsigned char pdata EER_DATA4;	//EEPROM Data4
  extern volatile unsigned char pdata EER_DATA5;	//EEPROM Data5
  extern volatile unsigned char pdata EER_DATA6;	//EEPROM Data6
  extern volatile unsigned char pdata EER_DATA7;	//EEPROM Data7

#endif // _DEFINE_XFR_VARIABLE

//============================================================================
#endif // _XFR_H
