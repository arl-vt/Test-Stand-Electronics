//----------------------------------------
// BIOS header files
//----------------------------------------
#include <xdc/std.h>  						//mandatory - have to include first, for BIOS types
#include <ti/sysbios/BIOS.h> 				//mandatory - if you call APIs like BIOS_start()
#include <xdc/runtime/Log.h>				//needed for any Log_info() call
#include <xdc/cfg/global.h> 				//header file for statically defined objects/handles


//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>

#include "slug.h"
//#include "inc/hw_types.h"
//#include "inc/hw_memmap.h"
//#include "driverlib/sysctl.h"
//#include "driverlib/gpio.h"
//#include "inc/hw_ints.h"
//#include "driverlib/interrupt.h"
//#include "driverlib/timer.h"


//----------------------------------------
// Prototypes
//----------------------------------------
void hardware_init(void);
void ledToggle(void);


//---------------------------------------
// Globals
//---------------------------------------
volatile int16_t i16ToggleCount = 0;


//---------------------------------------------------------------------------
// main()
//---------------------------------------------------------------------------
void main(void)
{
    uint32_t ui32Period;

    // Setup system clock to 80 MHZ
   Clock_set_80MHz();

   // Initialize the LED
   RGBled_Init(0, 0, 1); // Blue

   // Initialize the timer
   ui32Period = (SysCtlClockGet() /2);
   initTimer2(ui32Period);

   BIOS_start();
}


//---------------------------------------------------------------------------
// ledToggle()
//
// toggles LED on Tiva-C LaunchPad
//---------------------------------------------------------------------------
void ledToggle(void)
{
    RGBled_Toggle(0, 0,1); //Toggle blue LED

}

void hwi_led(void){
    // clear the interrupt flag
    Timer0IntHandler();

    // Post a software interrupt
    Swi_post(LEDswi);
}

