#include <stdint.h>
#include "slug.h"
#include "utils/uartstdio.h"

uint32_t raw, tempC, tempF, i = 0;

void testBlueLED(void);
void testBYellowLED(void);
void testTivaLED_Switch(void);
void testTimerToggle(void);
void testConsole(void);
void testLogger(void);



void testTempSensor(void);
void testSerialMonitor(void);
void testSerialMonitor2(void);
void testloadCellAmplifier(void);
void testMotor(void);
void testController(void);

int main(void){

    // Initialize clock at 40 MHZ frequency
    Clock_set_40MHz();

    // Test LED - Blue
    //testBlueLED();

    // Test LED - Yellow
    //testBYellowLED();

    // Test TIVA Launchpad LED and Button1 - Input and Output
    //testTivaLED_Switch();

    // Test Timer and Interrupts
    //testTimerToggle();

    // Test Console
    //testConsole();

    // test logger
    //testLogger();

    // 3. Test Temperature Sensor
    testTempSensor();






    // 4. Test UART functionality
    //testSerialMonitor();
    //testSerialMonitor2();

    // 5. Test Load Cell Amplifier
    //testloadCellAmplifier();

    // 6. Test DC Motor
    //testMotor();

    //7. Test Controller
    //testController();

}


//----------------------------------------------------------------------------
// Test Toggle behavior of Blue LED
// Toggle behavior
//----------------------------------------------------------------------------
void testBlueLED(){
    BlueLED_Init();

    while(1){

        // Toggle 1
        BlueLED_Set();
        delayMS(100);
        BlueLED_Clear();
        delayMS(100);

        // Toggle 2
        BlueLED_Toggle();
        delayMS(500);
        BlueLED_Toggle();
        delayMS(500);
    }

}

//----------------------------------------------------------------------------
// Test Toggle behavior of Yellow LED
// Toggle behavior
//----------------------------------------------------------------------------
void testBYellowLED(){
    YellowLED_Init();

    while(1){

        // Toggle 1
        YellowLED_Set();
        delayMS(100);
        YellowLED_Clear();
        delayMS(100);

        // Toggle 2
        YellowLED_Toggle();
        delayMS(500);
        YellowLED_Toggle();
        delayMS(500);
    }

}


//----------------------------------------------------------------------------
// Test Turn ON/OFF behavior of TIVA Launchpad
// Button press
// Toggle behavior
//----------------------------------------------------------------------------
void testTivaLED_Switch(void){
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
void testTimerToggle(){
    // Initialize LED
    RGBled_Init(0, 0, 1); //Initialize blue led

    // Define toggle frequency
    int freq = 10; //10Hz frequency of toggling

    // Initialize Timer0A
    initTimer0(freq);

    // Enable system wide interrupts
    EnableInterrupts();

    while(1){
    }
}

//----------------------------------------------------------------------------
// Test the Serial Monitor Connection with the computer
// Uses the microUSB port on TIVA
// Use the printf facility
//----------------------------------------------------------------------------
void testConsole(){
    int BaudRate  = 115200;
    bool flag = 1;
    int i = 0;
    initConsole(BaudRate);

    UARTprintf("System Startup");

    while(1){
        if(flag==1){
            UARTprintf("%d \n", i);
            i++;
        }

        if(i==10){
            flag = 0;
        }
    }
}

//----------------------------------------------------------------------------
// Test Logger - Serial Monitor and Timer2
// Uses the microUSB port on TIVA
// Use the printf facility
//----------------------------------------------------------------------------
void testLogger(){
    int BaudRate  = 115200;

    uint32_t loggerFreq = 10; //10 Hz

    Logger_Init(loggerFreq, BaudRate);

    UARTprintf("System Startup");

    while(1){
        // Do nothing
    }

}

//----------------------------------------------------------------------------
// Test Temperature sensor on ADC0
// On Interrupt with Serial Monitor connected
//----------------------------------------------------------------------------
void testTempSensor(){

    int BaudRate  = 115200;
    int hardwareAverage = 8;
    initConsole(BaudRate);
    tempSensor_Init(hardwareAverage);
    EnableInterrupts();

    UARTprintf("\n Recording internal Temp Sensor: \n");

    while(1){
        //Trigger temp read
        tempSensor_startConversion();

        //print temperature every 2 second
        //if(++i == 2000){
            i = 0;
            raw = getAvgTemp();
            tempC = convert2C(raw);
            tempF = convert2F(raw);
            UARTprintf("Avg temp is: %4d, *C is: %4d, *F is: %4d\n", raw, tempC, tempF);
        //}
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


void testloadCellAmplifier(){
    uint32_t sensorVal;
    float vol;
    //initConsole();
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
   // initConsole();
    uint32_t period = 2000; //clock/freq: 20MHz/10KHZ
    Motor_Init(period); // Initialize with 10KHZ

    UARTprintf("\n Test Motor \n");

    while(1){
        enableMotor();
        Motor_SetDuty(.01);
        delayMS(1000);
        disableMotor();
        delayMS(1000);
    }

}

void testController(){
   // initConsole();
    //uint32_t period = 2000; //clock/freq: 20MHz/10KHZ
    //Motor_Init(period); // Initialize with 10KHZ

    RGBled_Init(1,1,1);
    uint32_t controllerFreq = 10; //10kHZ
    Controller_Init(controllerFreq);
    EnableInterrupts();
    ControllerEnable();

    UARTprintf("\n Test Controller \n");

    while(1){

    }

}

