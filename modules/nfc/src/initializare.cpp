#include "debug.h"
#include "initializare.h"

#include <SoftwareSerial.h>
#include <Blynk/BlynkTimer.h>
#include <Blynk/BlynkApi.h>

// Variables - Objects Declaration
Initializare deviceInitializare;
#if defined(APP_DEBUG)
SoftwareSerial debug_serial(debug_serial_rx,debug_serial_tx);
#endif

BlynkTimer blynk_timer;

// Time related functions
void systemTimerEvent(void);
void nfcTimerEvent(void);
void timeoutConfig(void);

// Class Functions - Methods
void Initializare::begin() {
#if defined(APP_DEBUG)
   debug_serial.begin(debug_speed);
   DEBUG_PRINT("Hardware v "); DEBUG_PRINTLN( String(NFC_FW_VER).c_str() );
   DEBUG_PRINT("Firmware v "); DEBUG_PRINTLN( String(NFC_HW_VER).c_str() );
   DEBUG_PRINT("NFC Shield "); DEBUG_PRINTLN( String(NFC_SHIELD).c_str() );

   #define BLYNK_PRINT debug_serial
#endif
   Serial.begin(blynk_serial_speed);
   dispozitiv.begin();
   nfc.begin();

   blynk_timer.setInterval(system_timer_interval, systemTimerEvent);
   blynk_timer.setInterval(nfc_timer_interval, nfcTimerEvent);
   blynk_timer.setTimer(blynk_config_timeout, timeoutConfig, 1);
}

void systemTimerEvent(void) {
   if(dispozitiv.isRunning()) {
      nfc.run();
   }
}

void nfcTimerEvent(void) {
      // Blynk.virtualWrite(V5, millis() / 1000);
}

void timeoutConfig(void) { // config period finished, but device did not receive
                           // configuration from server, so run with default config
   DEBUG_PRINTLN("Config not received in timeout");
   DEBUG_PRINTLN("Relying on default configuration");
   dispozitiv.configurareNePrimita();
}

BLYNK_WRITE(V0) { // Receiving New Card Secure Key A from server...
   DEBUG_PRINT("New A key received: "); DEBUG_PRINTLN(param.asStr() );
   dispozitiv.primitConfigurare((const unsigned char *)param.getBuffer(), param.getLength()); // stop reading cards, until new key configured
}

BLYNK_WRITE(V4) {
   DEBUG_PRINT("Update key with: "); DEBUG_PRINTLN( param.asStr() );
   nfc.set_key_to_update(param.asInt());
}

void Initializare::run() {
   dispozitiv.run();
   nfc.run();
   blynk_timer.run();
}
