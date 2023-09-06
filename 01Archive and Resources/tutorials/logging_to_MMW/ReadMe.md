# Example using the Modular Sensors Library to log data to EnviroDIY/Monitor My Watershed

This sketch reduces menu_a_la_carte.ino to provide an example of how to log to https://monitormywatershed.org/ from two sensors, the BME280 and DS18. To complete the set up for logging to the web portal, the UUIDs for the site and each variable would need to be added to the sketch.

The settings for other data portals were removed from the example.

The modem settings were left unchanged because the sketch will test successfully without modem connection (wait patiently, it takes a few minutes).

This is the example you should use to deploy a logger with a modem to stream live data to the Monitor My Watershed data portal.

```cpp
const char *REGISTRATION_TOKEN = "8af47943-9093-4994-a3cb-24961214c7de";   // Device registration token
const char *SAMPLING_FEATURE = "4309a8b8-13df-4f66-8c92-42b31e3285ab";     // Sampling feature UUID
const char *UUIDs[] =                                                      // UUID array for device sensors
{
    "a0fa9be3-e88a-4157-b435-96bf72655b3e",   // Battery voltage (EnviroDIY_Mayfly_Batt)
    "53f571ad-3613-40e0-a7d9-1905232019ee",   // Temperature (Maxim_DS3231_Temp)
    "d85b3c6c-2d7e-48ba-813c-4ac873538a83"    // Temperature (Maxim_DS3231_Temp)
};
```
