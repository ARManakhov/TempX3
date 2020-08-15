#ifndef SCREEN_TEST_DS18B20_H
#define SCREEN_TEST_DS18B20_H

#define STARTCONVO 0x44      // Tells device to take a temperature reading and put it on the scratchpad
#define COPYSCRATCH 0x48     // Copy scratchpad to EEPROM
#define READSCRATCH 0xBE     // Read from scratchpad
#define WRITESCRATCH 0x4E    // Write to scratchpad
#define RECALLSCRATCH 0xB8   // Recall from EEPROM to scratchpad
#define READPOWERSUPPLY 0xB4 // Determine if device needs parasite power
#define ALARMSEARCH 0xEC     // Query bus for devices with an alarm condition

#define TEMP_9_BIT 0x1F  //  9 bit
#define TEMP_10_BIT 0x3F // 10 bit
#define TEMP_11_BIT 0x5F // 11 bit
#define TEMP_12_BIT 0x7F // 12 bit

#define sensors_scan_period 200
#define sensor_before_read_period 15

#endif //SCREEN_TEST_DS18B20_H
