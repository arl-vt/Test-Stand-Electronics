#include <stdint.h>
#include<stdbool.h>
#include "slug.h"
#include "utils/uartstdio.h"
void testloadCellAmplifier2(void);
void testMotor2(void);
void testController2(void);

// Load cell reading 50.9 pound is nominal

int main(void){

    // Load Cell reading in pound
    double loadCellOut;

    // Motor control variables
    float dutyCycle;
    int MotorDirection;

    bool testMotor = 1;

    //Setup system clock , 80MHZ
    Clock_set_fastest();

    //Configure Serial monitor
    initConsole();

    //Motor Initialize @ 10KHZ PWM freq
    uint32_t period = 2000; //clock/freq: 20MHz/10KHZ
    Motor_Init(period); // Initialize with 10KHZ
    UARTprintf("\n Motor Initialized! \n");

    //Load Cell Initialization
    LoadCell_init();
    UARTprintf("\n Load Cell Configured! \n");

    //Initialize debug LEDs
    RGBled_Init(0,1,1);

    //Controller initialize
   // uint32_t controllerFreq = 10000; //10kHZ
    //Controller_Init(controllerFreq);
    //setGoalForce(2600);
    //ControllerEnable();
    //UARTprintf("\n Controller Initialized! \n");

    //Enable system wide interrupts
    EnableInterrupts();

    while(1){
        //Measure Load Cell value
        loadCellOut = measuredLoad();

        //Turn Motor for some time and then reverse it
        UARTprintf("%04u\n", (uint32_t)(loadCellOut*1000));

        if(testMotor){
            dutyCycle = 0.1;
            MotorDirection = 1;
            enableMotor();
            //Motor_SetDuty(dutyCycle);
            motorSendCommand(dutyCycle, MotorDirection);
            delayMS(1000);
           // motorSendCommand(dutyCycle, MotorDirection);
           // delayMS(1000);
            disableMotor();
            testMotor = 0;
        }

        delayMS(1);
    }
}

