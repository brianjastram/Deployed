/*****************************************************************************
simple_logging.ino
Written By:  Sara Damiano (sdamiano@stroudcenter.org)
Development Environment: PlatformIO
Hardware Platform: EnviroDIY Mayfly Arduino Datalogger
Software License: BSD-3.
  Copyright (c) 2017, Stroud Water Research Center (SWRC)
  and the EnviroDIY Development Team

This example sketch is written for ModularSensors library version 0.21.4

This sketch is an example of logging data to an SD card

DISCLAIMER:
THIS CODE IS PROVIDED "AS IS" - NO WARRANTY IS GIVEN.
*****************************************************************************/

// ==========================================================================
//    Include the base required libraries
// ==========================================================================
#include <Arduino.h>  // The base Arduino library
#include <EnableInterrupt.h>  // for external and pin change interrupts


// ==========================================================================
//    Data Logger Settings
// ==========================================================================
// The library version this example was written for
const char *libraryVersion = "0.21.4";
// The name of this file
const char *sketchName = "simple_logging.ino";
// Logger ID, also becomes the prefix for the name of the data file on SD card
<<<<<<< HEAD
const char *LoggerID = "datalogger1";
// How frequently (in minutes) to log data
const uint8_t loggingInterval = 1;
=======
const char *LoggerID = "MWMOMF01";
// How frequently (in minutes) to log data
const uint8_t loggingInterval = 2;
>>>>>>> 57e34bd28ad88fda3336a48cf487780b2f1fc9a2
// Your logger's timezone.
const int8_t timeZone = -6;  // Central Standard Time
// NOTE:  Daylight savings time will not be applied!  Please use standard time!


// ==========================================================================
//    Primary Arduino-Based Board and Processor
// ==========================================================================
#include <sensors/ProcessorStats.h>

const long serialBaud = 115200;   // Baud rate for the primary serial port for debugging
const int8_t greenLED = 8;        // MCU pin for the green LED (-1 if not applicable)
const int8_t redLED = 9;          // MCU pin for the red LED (-1 if not applicable)
const int8_t buttonPin = 21;      // MCU pin for a button to use to enter debugging mode  (-1 if not applicable)
const int8_t wakePin = A7;        // MCU interrupt/alarm pin to wake from sleep
// Set the wake pin to -1 if you do not want the main processor to sleep.
// In a SAMD system where you are using the built-in rtc, set wakePin to 1
const int8_t sdCardPwrPin = -1;     // MCU SD card power pin (-1 if not applicable)
const int8_t sdCardSSPin = 12;      // MCU SD card chip select/slave select pin (must be given!)
const int8_t sensorPowerPin = 22;  // MCU pin controlling main sensor power (-1 if not applicable)

// Create the main processor chip "sensor" - for general metadata
const char *mcuBoardVersion = "v0.5b";
ProcessorStats mcuBoard(mcuBoardVersion);


// ==========================================================================
//    Maxim DS3231 RTC (Real Time Clock)
// ==========================================================================
#include <sensors/MaximDS3231.h>  // Includes wrapper functions for Maxim DS3231 RTC

// Create a DS3231 sensor object, using this constructor function:
MaximDS3231 ds3231(1);


// ==========================================================================
//    Settings for Additional Sensors
// ==========================================================================
// Additional sensors can setup here, similar to the RTC, but only if
//   they have been supported with ModularSensors wrapper functions. See:
//   https://github.com/EnviroDIY/ModularSensors/wiki#just-getting-started
// Syntax for the include statement and constructor function for each sensor is at
//   https://github.com/EnviroDIY/ModularSensors/wiki#these-sensors-are-currently-supported
//   or can be copied from the `menu_a_la_carte.ino` example

// ==========================================================================
//    Maxim DS18 One Wire Temperature Sensor
// ==========================================================================
<<<<<<< HEAD
#include <sensors/MaximDS18.h>

// OneWire Address [array of 8 hex characters]
// If only using a single sensor on the OneWire bus, you may omit the address
// DeviceAddress OneWireAddress1 = {0x28, 0xFF, 0xBD, 0xBA, 0x81, 0x16, 0x03, 0x0C};
const int8_t OneWirePower = sensorPowerPin;  // Pin to switch power on and off (-1 if unconnected)
const int8_t OneWireBus = 7;  // Pin attached to the OneWire Bus (-1 if unconnected) (D24 = A0)

// Create a Maxim DS18 sensor objects (use this form for a known address)
// MaximDS18 ds18(OneWireAddress1, OneWirePower, OneWireBus);

// Create a Maxim DS18 sensor object (use this form for a single sensor on bus with an unknown address)
MaximDS18 ds18(OneWirePower, OneWireBus);
=======
  #include <sensors/MaximDS18.h>

  // OneWire Address [array of 8 hex characters]
  // If only using a single sensor on the OneWire bus, you may omit the address
  // DeviceAddress OneWireAddress1 = {0x28, 0xFF, 0xBD, 0xBA, 0x81, 0x16, 0x03, 0x0C};
  const int8_t OneWirePower = sensorPowerPin;  // Pin to switch power on and off (-1 if unconnected)
  const int8_t OneWireBus = 7;  // Pin attached to the OneWire Bus (-1 if unconnected) (D24 = A0)

  // Create a Maxim DS18 sensor objects (use this form for a known address)
  // MaximDS18 ds18(OneWireAddress1, OneWirePower, OneWireBus);

  // Create a Maxim DS18 sensor object (use this form for a single sensor on bus with an unknown address)
  MaximDS18 ds18(OneWirePower, OneWireBus);

  // ==========================================================================
  //    Decagon CTD Conductivity, Temperature, and Depth Sensor
  // ==========================================================================
  #include <sensors/DecagonCTD.h>

  const char *CTDSDI12address = "1";  // The SDI-12 Address of the CTD
  const uint8_t CTDnumberReadings = 6;  // The number of readings to average
  const int8_t SDI12Power = sensorPowerPin;  // Pin to switch power on and off (-1 if unconnected)
  const int8_t SDI12Data = 7;  // The SDI12 data pin

  // Create a Decagon CTD sensor object
  DecagonCTD ctd(*CTDSDI12address, SDI12Power, SDI12Data, CTDnumberReadings);

  // Create conductivity, temperature, and depth variable pointers for the CTD
  // Variable *ctdCond = new DecagonCTD_Cond(&ctd, "12345678-abcd-1234-efgh-1234567890ab");
  // Variable *ctdTemp = new DecagonCTD_Temp(&ctd, "12345678-abcd-1234-efgh-1234567890ab");
  // Variable *ctdDepth = new DecagonCTD_Depth(&ctd, "12345678-abcd-1234-efgh-1234567890ab");
>>>>>>> 57e34bd28ad88fda3336a48cf487780b2f1fc9a2

// ==========================================================================
//    Creating the Variable Array[s] and Filling with Variable Objects
// ==========================================================================
#include <VariableArray.h>

Variable *variableList[] = {
    new ProcessorStats_SampleNumber(&mcuBoard),
    new ProcessorStats_FreeRam(&mcuBoard),
    new ProcessorStats_Batt(&mcuBoard),
    new MaximDS3231_Temp(&ds3231),
<<<<<<< HEAD
    new MaximDS18_Temp(&ds18)
=======
    new MaximDS18_Temp(&ds18),
    new DecagonCTD_Depth(&ctd, "12345678-abcd-1234-efgh-1234567890ab"),

>>>>>>> 57e34bd28ad88fda3336a48cf487780b2f1fc9a2
    // Additional sensor variables can be added here, by copying the syntax
    //   for creating the variable pointer (FORM1) from the `menu_a_la_carte.ino` example
    // The example code snippets in the wiki are primarily FORM2.
};
// Count up the number of pointers in the array
int variableCount = sizeof(variableList) / sizeof(variableList[0]);

// Create the VariableArray object
VariableArray varArray;


// ==========================================================================
//     The Logger Object[s]
// ==========================================================================
#include <LoggerBase.h>

// Create a logger instance
Logger dataLogger;


// ==========================================================================
//    Working Functions
// ==========================================================================

// Flashes the LED's on the primary board
void greenredflash(uint8_t numFlash = 4, uint8_t rate = 75)
{
  for (uint8_t i = 0; i < numFlash; i++) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    delay(rate);
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    delay(rate);
  }
  digitalWrite(redLED, LOW);
}


// ==========================================================================
// Main setup function
// ==========================================================================
void setup()
{
    // Start the primary serial connection
    Serial.begin(serialBaud);

    // Print a start-up note to the first serial port
    Serial.print(F("Now running "));
    Serial.print(sketchName);
    Serial.print(F(" on Logger "));
    Serial.println(LoggerID);
    Serial.println();

    Serial.print(F("Using ModularSensors Library version "));
    Serial.println(MODULAR_SENSORS_VERSION);

    if (String(MODULAR_SENSORS_VERSION) !=  String(libraryVersion))
        Serial.println(F(
            "WARNING: THIS EXAMPLE WAS WRITTEN FOR A DIFFERENT VERSION OF MODULAR SENSORS!!"));

    // Set up pins for the LED's
    pinMode(greenLED, OUTPUT);
    digitalWrite(greenLED, LOW);
    pinMode(redLED, OUTPUT);
    digitalWrite(redLED, LOW);
    // Blink the LEDs to show the board is on and starting up
    greenredflash();

    // Set the timezone and offsets
    // Logging in the given time zone
    Logger::setTimeZone(timeZone);
    // Offset is the same as the time zone because the RTC is in UTC
    Logger::setTZOffset(timeZone);

    // Set information pins
    dataLogger.setLoggerPins(wakePin, sdCardSSPin, sensorPowerPin, buttonPin, greenLED);

    // Begin the variable array[s], logger[s], and publisher[s]
    varArray.begin(variableCount, variableList);
    dataLogger.begin(LoggerID, loggingInterval, &varArray);

    // Set up the sensors
    Serial.println(F("Setting up sensors..."));
    varArray.setupSensors();

    // Create the log file, adding the default header to it
    // Do this last so we have the best chance of getting the time correct and
    // all sensor names correct
    dataLogger.createLogFile(true);  // true = write a new header

    // Call the processor sleep
    dataLogger.systemSleep();
}


// ==========================================================================
// Main loop function
// ==========================================================================
void loop()
{
    dataLogger.logData();
}
