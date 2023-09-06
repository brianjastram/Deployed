# Example using the Modular Sensors Library to save data to an SD card

This shows the simplest use of a "logger" object.  That is, creating an array of variable objects and then creating a logger object that utilizes those variables to update all of the variable results together and save the data to a SD card.  The processor then goes to sleep between readings.  This example has a placeholder to show users where to insert sensors, which can be found in menu_a_la_carte.ino.

This is the example you should use to deploy a logger somewhere where you don't want or have access to a way of streaming live data and you won't want to upload data to the WikiWatershed data portal.
```cpp
const char *REGISTRATION_TOKEN = "d02f68c0-2ed7-44e4-b912-d27c5aec74b0";   // Device registration token
const char *SAMPLING_FEATURE = "85e1d0c8-40dd-49ad-9a3a-6619a5500a6f";     // Sampling feature UUID
const char *UUIDs[]                                            // UUID array for device sensors
{
    "e463f1bd-45cd-4da2-95de-6e9ee665f9f2",   // Temperature (Maxim_DS3231_Temp)
    "317f1fa3-6e90-4cf6-8ba6-f0491aeeeff5",   // Battery voltage (EnviroDIY_Mayfly_Batt)
    "cba2f422-feaa-4a4f-93ae-e61cb56a66c2",   // Temperature (Maxim_DS3231_Temp)
    "fb6c6d89-f329-414e-b188-8ee829bb287f",   // Electrical conductivity (Decagon_CTD-10_Cond)
    "63aeba44-4a1d-4690-b08c-607b8be13777"    // Water depth (Decagon_CTD-10_Depth)
};
