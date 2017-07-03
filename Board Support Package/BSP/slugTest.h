#include <stdint.h>
#include "slug.h"
#include "utils/uartstdio.h"

// temperature sensor
uint32_t raw, tempC, tempF, i = 0;

// Load Cell
uint32_t rawLoadCellVal;
uint32_t rawADC2Val;
float LoadCellVol, LoadCellN;

//----------------------------------------------------------------------------
// Test Toggle behavior of Blue LED
// Toggle behavior
//----------------------------------------------------------------------------
void testBlueLED(void);

//----------------------------------------------------------------------------
// Test Toggle behavior of Yellow LED
// Toggle behavior
//----------------------------------------------------------------------------
void testBYellowLED(void);

//----------------------------------------------------------------------------
// Test Turn ON/OFF behavior of TIVA Launchpad
// Button press
// Toggle behavior
//----------------------------------------------------------------------------
void testTivaLED_Switch(void);

//----------------------------------------------------------------------------
// Test Timer module and Interrupt generation
// Toggle LED at 10 HZ
//----------------------------------------------------------------------------
void testTimerToggle(void);

//----------------------------------------------------------------------------
// Test the Serial Monitor Connection with the computer
// Uses the microUSB port on TIVA
// Use the printf facility
//----------------------------------------------------------------------------
void testConsole(void);

//----------------------------------------------------------------------------
// Test Logger - Serial Monitor and Timer2
// Uses the microUSB port on TIVA
// Use the printf facility
//----------------------------------------------------------------------------
void testLogger(void);

//----------------------------------------------------------------------------
// Test the Serial Monitor Connection with the computer
// Uses the microUSB port on TIVA, has an ISR
//----------------------------------------------------------------------------
void testSerialMonitor(void);

//----------------------------------------------------------------------------
// Test Temperature sensor on ADC0
// On Interrupt with Serial Monitor connected
//----------------------------------------------------------------------------
void testTempSensor(void);

//----------------------------------------------------------------------------
// Test Motor
// On a while loop
//----------------------------------------------------------------------------
void testMotor(void);

//----------------------------------------------------------------------------
// Test LoadCell
// RRecord the data from load cell
//----------------------------------------------------------------------------
void testloadCell(void);

//----------------------------------------------------------------------------
// Test Controller
// Record the data from load cell and apply feedforward trajectory to motor
//----------------------------------------------------------------------------
void testController(void);
