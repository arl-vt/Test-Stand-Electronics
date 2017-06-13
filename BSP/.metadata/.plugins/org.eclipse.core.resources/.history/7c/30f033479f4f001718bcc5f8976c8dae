#include <stdint.h>
#include "slug.h"
#include "utils/uartstdio.h"
void testloadCellAmplifier2(void);
void testMotor2(void);
void testController2(void);

// Load cell reading 2550 (nominal)

int main(void){

    uint32_t sensorVal;

    //Setup system clock , 40MHZ
    Clock_set_fastest();

    //Configure Serial monitor
    initConsole();

    //Motor Initialize
    uint32_t period = 2000; //clock/freq: 20MHz/10KHZ
    Motor_Init(period); // Initialize with 10KHZ
    UARTprintf("\n Motor Initialized! \n");

    //Load Cell Initialization
    LoadCell_init();
    UARTprintf("\n Load Cell Configured! \n");

    //Initialize debug LEDs
    RGBled_Init(0,1,1);

    //Controller initialize
    Controller_Init();
    //setRefForce(2600);
    ControllerEnable();
    UARTprintf("\n Controller Initialized! \n");

    //Enable system wide interrupts
    EnableInterrupts();

    while(1){
        sensorVal = getLoadCellValue();

        UARTprintf("Temp Sensor reading is: %04u\n", sensorVal);
        delayMS(1);
    }
}

