#include <stdint.h>
#include "slug.h"
#include "utils/uartstdio.h"

uint8_t Red = 0, Green = 0, Blue = 0;

void testGPIO(void);
void testGPIOTimers(void);
void testTempSensor(void);
void  testSerialMonitor(void);
void  testSerialMonitor2(void);
void testloadCellAmplifier(void);
void testMotor(void);
void testController(void);

int main3(void){

    Clock_set_fastest();

    // 1. test GPIO - Input and Output
    //testGPIO();

    // 2. Test Timer and Interrupts
    //testGPIOTimers();

    // 3. Test Temperature Sensor
    //testTempSensor();

    // 4. Test UART functionality
    //testSerialMonitor();
    testSerialMonitor2();

    // 5. Test Load Cell Amplifier
    //testloadCellAmplifier();

    // 6. Test DC Motor
    //testMotor();

    //7. Test Controller
    //testController();
}

//----------------------------------------------------------------------------
// Test Turn ON/OFF behavior
// Button press
// Toggle behavior
//----------------------------------------------------------------------------
void testGPIO(void){
    RGBled_Init(1, 1, 1); // Initialize all
    Button1_Init();

    while(1){
                // Test RGB LEDS
//                RGBled_Set(1, 1, 0);
//                Delay_cycle(2000000);
//                RGBled_Set(1, 0, 0);
//                Delay_cycle(2000000);


                // Test Toggle Behavior
//                RGBled_Toggle(1, 0, 0);
//                Delay_cycle(2000000);
//                RGBled_Toggle(0, 1, 0);
//                Delay_cycle(2000000);


                //Test Button input
//                if(Button1_Input()==0){
//                    RGBled_Set(0, 1, 0); //Set green on Press
//                }else{
//                    RGBled_Set(0, 0, 1); // Set Blue on Release
//                }
    }
}

//----------------------------------------------------------------------------
// Test Timer module and Interrupt generation
// Toggle LED at 10 HZ
//----------------------------------------------------------------------------
void testGPIOTimers(){
    int freq = 10; //10Hz frequency of toggling
    RedledTimer_Init(freq);
    while(1){
    }
}

//----------------------------------------------------------------------------
// Test Temperature sensor on ADC0
// On Interrupt with Serial Monitor connected
//----------------------------------------------------------------------------
void testTempSensor(){
    tempSensor_init();
    initConsole();
    EnableInterrupts();

    UARTprintf("\n Recording internal Temp Sensor: \n");
    uint32_t raw, tempC, tempF, i = 0;

    while(1){
        //Trigger temp read
        tempSensor_startConversion();

        //print temp every 2 second
        if(++i == 2000){
            i = 0;
            raw = getAvgTemp();
            tempC = convert2C(raw);
            tempF = convert2F(raw);
            UARTprintf("Avg temp is: %4d, *C is: %4d, *F is: %4d\n", raw, tempC, tempF);
        }
        delayMS(1);
    }

}

//----------------------------------------------------------------------------
// Test the Serial Monitor Connection with the computer
// Uses the microUSB port on TIVA, has an ISR
//----------------------------------------------------------------------------
void testSerialMonitor(){
    SerialMonitor_Init();

    while(1){
        //SerialMonitor_Loop();
    }
}

//----------------------------------------------------------------------------
// Test the Serial Monitor Connection with the computer
// Uses the microUSB port on TIVA
// Use the printf facility
//----------------------------------------------------------------------------
void testSerialMonitor2(){
    initConsole();
    SerialMonitor_Send("Wohooo");
    while(1){
       // SerialMonitor_Send();
    }
}


void testloadCellAmplifier(){
    uint32_t sensorVal;
    float vol;
    initConsole();
    LoadCell_init();
    EnableInterrupts();

    UARTprintf("\n Recording Load cell Sensor: \n");

    while(1){
        sensorVal = getLoadCellValue();
        vol = adc2Vol(sensorVal);
        UARTprintf("Temp Sensor reading is: %04u, vol is: %d\n", sensorVal, (uint32_t)vol);
        delayMS(1);
    }
}

void testMotor(){
    initConsole();
    uint32_t period = 2000; //clock/freq: 20MHz/10KHZ
    Motor_Init(period); // Initialize with 10KHZ

    UARTprintf("\n Test Motor \n");

    while(1){
        enableMotor();
        Motor_SetDuty(.01, period);
        delayMS(1000);
        disableMotor();
        delayMS(1000);
    }

}

void testController(){
    initConsole();
    //uint32_t period = 2000; //clock/freq: 20MHz/10KHZ
    //Motor_Init(period); // Initialize with 10KHZ

    RGBled_Init(1,1,1);
    uint32_t controllerFreq = 10000; //10kHZ
    Controller_Init(controllerFreq);
    EnableInterrupts();
    ControllerEnable();

    UARTprintf("\n Test Controller \n");

    while(1){

    }

}
