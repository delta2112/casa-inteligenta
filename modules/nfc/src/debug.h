#ifndef DEBUG_CONFIG_FILE
#define DEBUG_CONFIG_FILE

#include "setari.h"

#ifdef APP_DEBUG
#include <SoftwareSerial.h>
extern SoftwareSerial debug_serial;
// #define DEBUG_PRINT(...)   debug_serial.print(__VA_ARGS__);
// #define DEBUG_PRINTLN(...) debug_serial.println(__VA_ARGS__);

#define DEBUG_PRINT(MESAJ_DEBUG) Blynk.virtualWriteBinary(CHN_DEBUG, MESAJ_DEBUG, 10 );
#define DEBUG_PRINTLN(MESAJ_DEBUG) DEBUG_PRINT(MESAJ_DEBUG); Blynk.virtualWriteBinary(CHN_DEBUG, "\r\n", 2);

#else
#define DEBUG_PRINT(...)   ;
#define DEBUG_PRINTLN(...) ;
#endif

#endif