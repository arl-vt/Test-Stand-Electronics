#include <stdint.h>
#include<stdbool.h>
#include<stdio.h>
#include<string.h>
#include "slug.h"
#include "utils/uartstdio.h"
void testloadCellAmplifier2(void);
void testMotor2(void);
void testController2(void);

// Load Cell reading in pound
double loadCellOut, error, OutDuty;
char loadCellBuffer[80] = "test";

//Controller variables
// Load cell reading 45.83 pound is nominal
// Set Ref 50 pound
double setPointForce = 50;


int main(void){
    //Setup system clock , 80MHZ
    Clock_set_fastest();

    //Configure Serial monitor
    initConsole();

    //Motor Initialize @ 10KHZ PWM freq
    uint32_t MotorPwmFreq = 10000; //10KHZ
    Motor_Init(MotorPwmFreq); // Initialize with 10KHZ
    UARTprintf("\n Motor Initialized! \n");

    //Load Cell Initialization
    LoadCell_init();
    UARTprintf("\n Load Cell Configured! \n");

    //Initialize debug LEDs
    RGBled_Init(0,1,1);

    //Controller initialize
    setGoalForce(setPointForce);
    uint32_t controllerFreq = 2000; //2 kHZ
    Controller_Init(controllerFreq);
    ControllerEnable();
    UARTprintf("\n Controller Initialized! \n");

    //Initialize the logger
    uint32_t loggerFreq = 100; //10 Hz
    Logger_Init(loggerFreq);

    //Enable system wide interrupts
    EnableInterrupts();

    while(1){
//        //Everything on interrupts
//        loadCellOut = measuredLoad();
//        delayMS(10);

    }
}

