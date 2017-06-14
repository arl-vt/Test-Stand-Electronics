#include <stdint.h>
#include<stdbool.h>
#include "slug.h"
#include "utils/uartstdio.h"
void testloadCellAmplifier2(void);
void testMotor2(void);
void testController2(void);

// Load cell reading 45.83 pound is nominal
// Set Ref 50 pound

int main(void){

    // Load Cell reading in pound
    double loadCellOut, error, OutDuty;

    //Controller variables
    double setPointForce = 50;

    //Motor
    //static volatile uint32_t dutyCycle = 20;

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

    //Enable system wide interrupts
    EnableInterrupts();


    while(1){
        //Measure Load Cell value
        loadCellOut = measuredLoad()*100;

        error = getError()*100;
        OutDuty = getPIDoutput()*100;
        //Test Motor


        //Turn Motor for some time and then reverse it
        //UARTprintf("Load:%04d\n", (uint32_t)loadCellOut);
        UARTprintf("Load:%04u, Error: %04u, PID_Out: %04u\n", (uint32_t)loadCellOut, (uint32_t)error, (uint32_t)OutDuty);


    }
}

