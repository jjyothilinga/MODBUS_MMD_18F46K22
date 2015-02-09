
/*
*------------------------------------------------------------------------------
* main.c
*
*------------------------------------------------------------------------------
*/


//#pragma config FOSC 	= INTIO67
#pragma config FOSC 	= HSHP
#pragma config PLLCFG 	= ON
#pragma config PRICLKEN = ON
#pragma config FCMEN 	= OFF
#pragma config IESO 	= OFF
#pragma config PWRTEN   = OFF
#pragma config BOREN 	= OFF
#pragma config BORV 	= 285
#pragma config WDTEN    = OFF
#pragma config WDTPS 	= 512
#pragma config CCP2MX 	= PORTC1
#pragma config PBADEN 	= OFF
#pragma config MCLRE 	= EXTMCLR
#pragma config STVREN 	= OFF
#pragma config XINST 	= OFF
#pragma config DEBUG 	= ON
#pragma config LVP      = OFF
#pragma config CP0      = OFF
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#pragma config CPB      = OFF
#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
#pragma config WRT3     = OFF
#pragma config WRTB     = OFF       
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF





/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <timers.h>				// Timer library functions
#include <delays.h>				// Delay library functions
#include <string.h>				// String library functions
#include "board.h"				// board setup
#include "timer.h"
#include "heartbeat.h"
#include "config.h"
#include "mb.h"
#include "mmd.h"
#include "app.h"

/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Processor config bits
*------------------------------------------------------------------------------
*/



/*
*------------------------------------------------------------------------------
* Private Macros
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables
*------------------------------------------------------------------------------
*/


/*

*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Constants
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Constants (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Function Prototypes (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void main(void)

* Summary	: Application specifc main routine. Initializes all port and
*			: pheriperal and put the main task into an infinite loop.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/

#define MMD_REFRESH_PERIOD	(65535 - 10000)

void main(void)
{
	unsigned long temp;

#ifdef MMD_TEST
	MMD_Config mmdConfig= {0};
	//UINT8 line[50] ="Ideonics ideas and electronics, subhrajyoti biswal "; 
	UINT8 line[10] = "ABCDEFGH ";
	UINT8 data[10] = "UVWX ";
	UINT8 test[10] = "IDEONICS ";
#endif

	BRD_init();			//board initialization

	TMR0_init(tickPeriod,0);		//initialize timer0
	TMR1_init(MMD_REFRESH_PERIOD,MMD_refreshDisplay);		//initialize timer1
	//TMR2_init(130 ,MMD_refreshDisplay);		//initialize timer2


	APP_init();


	EnableInterrupts();		//Interrupts initialization

	//Heart Beat to blink at every 500ms
	temp = (500UL *1000UL)/TIMER0_TIMEOUT_DURATION;

#ifdef MMD_TEST
	MMD_clearSegment(0);
	mmdConfig.startAddress = 0;
	mmdConfig.length = MMD_MAX_CHARS;
	mmdConfig.symbolCount = strlen(line);
	mmdConfig.symbolBuffer = line;
	mmdConfig.scrollSpeed = 0;	
	MMD_configSegment( 0 , &mmdConfig);

/*	MMD_clearSegment(1);
	mmdConfig.startAddress = 8;
	mmdConfig.length = MMD_MAX_CHARS;
	mmdConfig.symbolCount = strlen(data);
	mmdConfig.symbolBuffer = data;
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 1 , &mmdConfig);

	MMD_clearSegment(2);
	mmdConfig.startAddress = 16;
	mmdConfig.length = MMD_MAX_CHARS;
	mmdConfig.symbolCount = strlen(data);
	mmdConfig.symbolBuffer = data;
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 2 , &mmdConfig);

	MMD_clearSegment(3);
	mmdConfig.startAddress = 24;
	mmdConfig.length = MMD_MAX_CHARS;
	mmdConfig.symbolCount = strlen(test);
	mmdConfig.symbolBuffer = test;
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 3 , &mmdConfig);
*/
#endif
	
	while(1)
	{

		//Heart Beat to blink at every 500ms
	`	if(heartBeatCount >= temp )
		{			
			HB_task();
			heartBeatCount = 0;
		}

		if( mmdUpdateCount >= 25 )
		{
			MMD_task();
			mmdUpdateCount = 0;
		}

		eMBPoll();	//modbus task
		
	}


}

/*
*  End of main.c
*/
