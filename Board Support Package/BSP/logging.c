//
//// logging
//// Application 1
//// Logs data
//
//// Author : Shriya Shah
//
//#include "slugTest.h"
//
//double setPointForce = 50;
//uint32_t volOut, rawLoadCellVal;
//
//int main(void){
//
//    // Initialize clock at 80 MHZ frequency
//    Clock_set_80MHz();
//
//    // Initialize Console
//    int BaudRate  = 115200;
//    uint32_t loggerFreq = 1000; //1 KHz
//    Logger_Init(loggerFreq, BaudRate);
//
//    // Initialize Load Cell
//    int hardwareAveraging, ADCsampleFreq;
//    hardwareAveraging = 4;
//    ADCsampleFreq = 1000; //1 KHz
//    LoadCell_init(hardwareAveraging, ADCsampleFreq);
//
//    // Initialize additional ADC
//    addADC_Init(hardwareAveraging, ADCsampleFreq);
//
//    // Enable all interrupts and channels
//    EnableInterrupts();
//
//    // For debugging
//    RGBled_Init(0, 0, 1); //Blue
//
//
//    while(1){
//        volOut = getaddADCVal();
//        rawLoadCellVal = getLoadCellValue();
//        delayMS(1);
//    }
//
//}
//
