/*****************************************************************************
Based on logging_to MMW.ino and DRWI_SIM7080LTE.ino
Adapted by Anthony and Brian
from source by:  Sara Damiano (sdamiano@stroudcenter.org)
Development Environment: PlatformIO
Mayfly v1.1 board
EnviroDIY SIM7080 LTE module (with Hologram SIM card)
author Sara Geleskie Damiano <sdamiano@stroudcenter.org>
copyright (c) 2017-2022 Stroud Water Research Center (SWRC)
                          and the EnviroDIY Development Team
            This example is published under the BSD-3 license.

Hardware Platform: EnviroDIY Mayfly Arduino Datalogger

DISCLAIMER:
THIS CODE IS PROVIDED "AS IS" - NO WARRANTY IS GIVEN.
*****************************************************************************/

// ==========================================================================
//    Include the base required libraries
// ==========================================================================
/** Start [includes] */
#include <Arduino.h>  // The base Arduino library needed for every Arduino Program

// EnableInterrupt is used by ModularSensors for external and pin change
#include <EnableInterrupt.h>  // for external and  change interrupts

// Include the main header for ModularSensors
#include <ModularSensors.h>
/** End [includes] */


// ==========================================================================
//    Data Logger Settings
// ==========================================================================
/** Start [logging_options] */
// The name of this program file
const char *sketchName = "RG02.cpp";
// Logger ID, also becomes the prefix for the name of the data file on SD card
const char *LoggerID = "RG02";
// How frequently (in minutes) to log data
const uint8_t loggingInterval = 15;
// Your logger's timezone.
const int8_t timeZone = -6;  // Central Standard Time
// NOTE:  Daylight savings time will not be applied!  Please use standard time!

// ==========================================================================
//    Primary Arduino-Based Board and Processor
// ==========================================================================
#include <sensors/ProcessorStats.h>

const long serialBaud   = 57600;   // Baud rate for the primary serial port for debugging
const int8_t greenLED   = 8;        // MCU pin for the green LED (-1 if not applicable)
const int8_t redLED     = 9;          // MCU pin for the red LED (-1 if not applicable)
const int8_t buttonPin  = 21;      // MCU pin for a button to use to enter debugging mode  (-1 if not applicable)
const int8_t wakePin    = 31;        // MCU interrupt/alarm pin to wake from sleep
// Mayfly 0.x D31 = A7
// Set the wake pin to -1 if you do not want the main processor to sleep.
// In a SAMD system where you are using the built-in rtc, set wakePin to 1
const int8_t sdCardPwrPin   = -1;     // MCU SD card power pin (-1 if not applicable)
const int8_t sdCardSSPin    = 12;      // MCU SD card chip select/slave select pin (must be given!)
const int8_t sensorPowerPin = 22;  // MCU pin controlling main sensor power (-1 if not applicable)
/** End [logging_options] */

// Create the main processor chip "sensor" - for general metadata
const char *mcuBoardVersion = "v1.1";
ProcessorStats mcuBoard(mcuBoardVersion);


// ==========================================================================
//    Wifi/Cellular Modem Settings
// ==========================================================================

/** Start [sim_com_sim7080] */
// For almost anything based on the SIMCom SIM7080G
#include <modems/SIMComSIM7080.h>

// Create a reference to the serial port for the modem
HardwareSerial& modemSerial = Serial1;  // Use hardware serial if possible
const int32_t   modemBaud = 9600;  //  SIM7080 does auto-bauding by default, but
                                   //  for simplicity we set to 9600

// Modem Pins - Describe the physical pin connection of your modem to your board
// NOTE:  Use -1 for pins that do not apply
// AltSoftSerial &modemSerial = altSoftSerial;  // For software serial if needed
// NeoSWSerial &modemSerial = neoSSerial1;  // For software serial if needed

// Modem Pins - Describe the physical  connection of your modem to your board
// NOTE:  Use -1 for pins that do not apply

const int8_t modemVccPin = 18;
// MCU pin controlling modem power --- Pin 18 is the power enable pin for the
// bee socket on Mayfly v1.0, use -1 if using Mayfly 0.5b or if the bee socket
// is constantly powered (ie you changed SJ18 on Mayfly 1.x to 3.3v)
const int8_t modemStatusPin  = 19;  // MCU pin used to read modem status
const int8_t modemSleepRqPin = 23;  // MCU pin for modem sleep/wake request
const int8_t modemLEDPin = redLED;  // MCU pin connected an LED to show modem
                                    // status

// Network connection information
const char* apn =
    "hologram";  // APN connection name, typically Hologram unless you have a
                 // different provider's SIM card. Change as needed

// Create the modem object
SIMComSIM7080 modem7080(&modemSerial, modemVccPin, modemStatusPin,
                        modemSleepRqPin, apn);
// Create an extra reference to the modem by a generic name
SIMComSIM7080 modem = modem7080;
/** End [sim_com_sim7080] */


// ==========================================================================
//    Maxim DS3231 RTC (Real Time Clock)
// ==========================================================================
#include <sensors/MaximDS3231.h>

// Create a DS3231 sensor object
MaximDS3231 ds3231(1);


// ==========================================================================
//    External I2C Rain Tipping Bucket Counter
// ==========================================================================
#include <sensors/RainCounterI2C.h>

const uint8_t RainCounterI2CAddress = 0x08;  // I2C Address for external tip counter
const float depthPerTipEvent = 0.01;  // rain depth in *INCHES* per tip event

// Create a Rain Counter sensor object
RainCounterI2C tbi2c(RainCounterI2CAddress, depthPerTipEvent);


// ==========================================================================
//  Creating the Variable Array[s] and Filling with Variable Objects
// ==========================================================================
/** Start [variable_arrays] */
Variable* variableList[] = {
    new RainCounterI2C_Depth(&tbi2c),
    new ProcessorStats_Battery(&mcuBoard),
    new MaximDS3231_Temp(&ds3231),
    new Modem_RSSI(&modem),
    new Modem_SignalPercent(&modem),
};

// All UUID's, device registration, and sampling feature information can be
// pasted directly from Monitor My Watershed.
// To get the list, click the "View  token UUID list" button on the upper right
// of the site page.

// *** CAUTION --- CAUTION --- CAUTION --- CAUTION --- CAUTION ***
// Check the order of your variables in the variable list!!!
// Be VERY certain that they match the order of your UUID's!
// Rearrange the variables in the variable list ABOVE if necessary to match!
// Do not change the order of the variables in the section below.
// *** CAUTION --- CAUTION --- CAUTION --- CAUTION --- CAUTION ***

// Replace all of the text in the following section with the UUID array from
// MonitorMyWatershed

/* clang-format off */
// ---------------------   Beginning of Token UUID List   ---------------------

const char *UUIDs[] =                                                      // UUID array for device sensors
{
    "d3179602-7c60-456b-b904-63b31e528f32",   // Precipitation (TE_TR-525USW_Precip_5minTotal)
    "c35e3049-1b78-43a6-bb98-b22b97621ac6",   // Battery voltage (EnviroDIY_Mayfly_Batt)
    "7c5a6e82-6e67-4ccc-9ee6-30c83f58a0b3",   // Temperature (Maxim_DS3231_Temp)
    "948d533b-d9e1-4ace-a299-f234547c86ff",   // Received signal strength indication (EnviroDIY_LTEB_RSSI)
    "044a4ee3-41f0-4271-b901-35126bc80db9"    // Percent full scale (EnviroDIY_LTEB_SignalPercent)
};
const char *registrationToken = "d65f4405-1d5e-4271-bbcd-325facbe2612";   // Device registration token
const char *samplingFeature = "8704ed67-84d8-4603-93f8-1eaf666b49aa";     // Sampling feature UUID

// -----------------------   End of Token UUID List  -----------------------
/* clang-format on */

// Count up the number of pointers in the array
int variableCount = sizeof(variableList) / sizeof(variableList[0]);

// Create the VariableArray object
VariableArray varArray(variableCount, variableList, UUIDs);
/** End [variable_arrays] */

// ==========================================================================
//     The Logger Object[s]
// ==========================================================================

// Create a new logger instance
Logger dataLogger(LoggerID, loggingInterval, &varArray);


// ==========================================================================
//    A Publisher to Monitor My Watershed / EnviroDIY Data Sharing Portal
// ==========================================================================
// Device registration and sampling feature information can be obtained after
// registration at https://monitormywatershed.org or https://data.envirodiy.org

//const char *registrationToken = "2d0e83d2-434e-4d84-b81b-6555f362694b";   // Device registration token
//const char *samplingFeature = "1fc2441d-3e01-4081-8edf-8720eb9e8603";     // Sampling feature UUID

// Create a data publisher for the EnviroDIY/WikiWatershed POST endpoint
#include <publishers/EnviroDIYPublisher.h>
EnviroDIYPublisher EnviroDIYPOST(dataLogger, &modem.gsmClient, registrationToken, samplingFeature);


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


// Read's the battery voltage
// NOTE: This will actually return the battery level from the previous update!
float getBatteryVoltage()
{
    if (mcuBoard.sensorValues[0] == -9999) mcuBoard.update();
    return mcuBoard.sensorValues[0];
}


// ==========================================================================
// Main setup function
// ==========================================================================
/** Start [setup] */
void setup(){
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
    Serial.print(F("TinyGSM Library version "));
    Serial.println(TINYGSM_VERSION);
    Serial.println();

// Allow interrupts for software serial
//#if defined SoftwareSerial_ExtInts_h
 //   enableInterrupt(softSerialRx, SoftwareSerial_ExtInts::handle_interrupt, CHANGE);
//#endif
//#if defined NeoSWSerial_h
//    enableInterrupt(neoSSerial1Rx, neoSSerial1ISR, CHANGE);
//#endif

    // Start the serial connection with the modem
    modemSerial.begin(modemBaud);

    // Set up pins for the LED's
    pinMode(greenLED, OUTPUT);
    digitalWrite(greenLED, LOW);
    pinMode(redLED, OUTPUT);
    digitalWrite(redLED, LOW);
    // Blink the LEDs to show the board is on and starting up
    greenredflash();

    // Set up some of the power pins so the board boots up with them off
    // NOTE:  This isn't necessary at all.  The logger begin() function
    // should leave all power pins off when it finishes.
    if (modemVccPin >= 0)
    {
        pinMode(modemVccPin, OUTPUT);
        digitalWrite(modemVccPin, LOW);
    }
    if (sensorPowerPin >= 0)
    {
        pinMode(sensorPowerPin, OUTPUT);
        digitalWrite(sensorPowerPin, LOW);
    }

    // Set the timezones for the logger/data and the RTC
    // Logging in the given time zone
    Logger::setLoggerTimeZone(timeZone);
    // It is STRONGLY RECOMMENDED that you set the RTC to be in UTC (UTC+0)
    Logger::setRTCTimeZone(0);

    // Attach the modem and information pins to the logger
    dataLogger.attachModem(modem);
    modem.setModemLED(modemLEDPin);
    dataLogger.setLoggerPins(wakePin, sdCardSSPin, sdCardPwrPin, buttonPin, greenLED);

    // Begin the logger
    dataLogger.begin();

    // Note:  Please change these battery voltages to match your battery
    // Set up the sensors, except at lowest battery level
    if (getBatteryVoltage() > 3.4)
    {
        Serial.println(F("Setting up sensors..."));
        varArray.setupSensors();
    }

    /** Start [setup_sim7080] */
    modem.setModemWakeLevel(HIGH);  // ModuleFun Bee inverts the signal
    modem.setModemResetLevel(HIGH); // ModuleFun Bee inverts the signal
    Serial.println(F("Waking modem and setting Cellular Carrier Options..."));
    modem.modemWake();                  // NOTE:  This will also set up the modem
    modem.gsmModem.setBaud(modemBaud);  // Make sure we're *NOT* auto-bauding!
    modem.gsmModem.setNetworkMode(38);  // set to LTE only
                                        // 2 Automatic
                                        // 13 GSM only
                                        // 38 LTE only
                                        // 51 GSM and LTE only
    modem.gsmModem.setPreferredMode(1); // set to CAT-M
                                        // 1 CAT-M
                                        // 2 NB-IoT
                                        // 3 CAT-M and NB-IoT
    /** End [setup_sim7080] */

    // Sync the clock if it isn't valid or we have battery to spare
    if (getBatteryVoltage() > 3.55 || !dataLogger.isRTCSane())
    {
        // Synchronize the RTC with NIST
        // This will also set up the modem
        dataLogger.syncRTC();
    }

    // Create the log file, adding the default header to it
    // Do this last so we have the best chance of getting the time correct and
    // all sensor names correct
    // Writing to the SD card can be power intensive, so if we're skipping
    // the sensor setup we'll skip this too.
    if (getBatteryVoltage() > 3.4)
    {
        Serial.println(F("Setting up file on SD card"));
        dataLogger.turnOnSDcard(
            true);                      // true = wait for card to settle after power up
        dataLogger.createLogFile(true); // true = write a new header
        dataLogger.turnOffSDcard(
            true); // true = wait for internal housekeeping after write
    }

    // Call the processor sleep
    Serial.println(F("Putting processor to sleep\n"));
    dataLogger.systemSleep();
}
/** End [setup] */

// ==========================================================================
// Arduino Loop Function
// ==========================================================================

/** Start [loop] */
// Use this short loop for simple data logging and sending
void loop()
{
    // Note:  Please change these battery voltages to match your battery
    // At very low battery, just go back to sleep
    if (getBatteryVoltage() < 3.4)
    {
        dataLogger.systemSleep();
    }
    // At moderate voltage, log data but don't send it over the modem
    else if (getBatteryVoltage() < 3.55)
    {
        dataLogger.logData();
    }
    // If the battery is good, send the data to the world
    else
    {
        dataLogger.logDataAndPublish();
    }
}
/** End [loop] */
