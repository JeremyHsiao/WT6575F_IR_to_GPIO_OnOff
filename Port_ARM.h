#ifndef _PORT_ARM_
#define _PORT_ARM_

// Jeremy Added
//
// Debug definition
//
//#define DEBUG_GENERAL
//#define DEBUG_ERROR_MSG
//#define DEBUG_GETKEY
//#define DEBUG_DECODER

#ifdef DEBUG_GENERAL
  #define	UARTputchar(x)			putchar(x)
  #define	UARTprintf(x,y)			printf(x,y)
  #define	UARTputstr(x)			puts(x)
#else
  #define	UARTputchar(x)	
  #define	UARTprintf(x,y)	
  #define	UARTputstr(x)	
  #undef	DEBUG_ERROR_MSG
  #undef	DEBUG_GETKEY
  #undef	DEBUG_DECODER
#endif //

#ifdef DEBUG_ERROR_MSG
  #define	UARTputcharErr(x)		putchar(x)
  #define	UARTprintfErr(x,y)		printf(x,y)
  #define	UARTputstrErr(x)		puts(x)
#else
  #define	UARTputcharErr(x)		
  #define	UARTprintfErr(x,y)	
  #define	UARTputstrErr(x)		
#endif // 

#ifdef DEBUG_GETKEY
  #define	UARTputcharGetKey(x)	putchar(x)
  #define	UARTprintfGetKey(x,y)	printf(x,y)
  #define	UARTputstrGetKey(x)		puts(x)
#else
  #define	UARTputcharGetKey(x)		
  #define	UARTprintfGetKey(x,y)	
  #define	UARTputstrGetKey(x)		
#endif // 

#ifdef DEBUG_DECODER
  #define	UARTputcharDebug(x)		putchar(x)
  #define	UARTprintfDebug(x,y)	printf(x,y)
  #define	UARTputstrDebug(x)		puts(x)
#else
  #define	UARTputcharDebug(x)		
  #define	UARTprintfDebug(x,y)	
  #define	UARTputstrDebug(x)		
#endif // 



#define EXINT			(1<<3)

//#define TIMER0_TIME	  (32)		// 32 us
#define TIMER0_TIME	  (8192)		// 8192 ms is max
#define RELOAD_CNT    (65536-(TIMER0_TIME*(1000/125)))	
#define Covert_us_to_WidthBase(x) 	(U32)(((U32)x)/(U32)32)  								 

typedef bit 			Bool;
typedef Bool 			Boolean;
typedef unsigned char 	uint8_t;
typedef unsigned int 	uint16_t;
typedef unsigned long 	uint32_t;
typedef char 			int8_t;
typedef int 			int16_t;
typedef long 			int32_t;
typedef uint8_t			U8;
typedef uint16_t		U16;
typedef uint32_t 		U32;
typedef U8				u8;
typedef U16				u16;
typedef U32 			u32;
#define	FALSE			0
#define TRUE			1
#define	false			0
#define true			1

#define _EOS_			(0x00)

#define inline

// Get function
#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))
// Set function
#define BYTELOW(v)   (*(((unsigned char *) (&v) + 1)))
#define BYTEHIGH(v)  (*((unsigned char *) (&v)))

#endif // #ifndef _PORT_ARM_
