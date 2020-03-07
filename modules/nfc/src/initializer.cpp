#include "debug.h"
#include "initializer.h"

#include <SoftwareSerial.h>
#include <Blynk/BlynkTimer.h>
#include <Blynk/BlynkApi.h>

// Variables - Objects Declaration
Initializer deviceInitializer;
#if defined(APP_DEBUG)
SoftwareSerial debug_serial(debug_serial_rx,debug_serial_tx);
#endif
BlynkTimer blynk_timer;
void nfcTimerEvent(void);
void timeoutConfig(void);

// Functions - Methods
void Initializer::begin() {
#if defined(APP_DEBUG)
   debug_serial.begin(debug_speed);
   DEBUG_PRINTLN("Hardware v"  + String(NFC_FW_VER));
   DEBUG_PRINTLN("Firmware v"  + String(NFC_HW_VER));
   DEBUG_PRINTLN("NFC Shield " + String(NFC_SHIELD));

   #define BLYNK_PRINT debug_serial
#endif
   Serial.begin(blynk_serial_speed);
   // Initialize state machine
   systemState.set(WAIT_CONFIG);

   nfc.begin();
   blynk_timer.setInterval(nfc_timer_inerval, nfcTimerEvent);
   blynk_timer.setTimer(blynk_config_timeout, timeoutConfig, 1);
}

void nfcTimerEvent(void) {
   // Blynk.virtualWrite(V5, millis() / 1000);
   nfc.run();
}

void timeoutConfig(void) { // config period finished, but device did not receive
                           // configuration from server, so run with default config
   DEBUG_PRINTLN("Relying on default configuration");
   deviceInitializer.setRunningState();
}

BLYNK_WRITE(V0) { // Card secure Key A
   deviceInitializer.setConfiguringState();
   DEBUG_PRINTLN(param.asStr());
   nfc.setKey(param.asStr());
}

void Initializer::run() {  // this is the actual state machine of the
                           // application
   switch (systemState.get()) {
      case WAIT_CONFIG:
         break;
      case CONFIGURING:
         setConfiguringState();
         break;
      case RUNNING:
         setRunningState();
         break;
      case RESET_CONFIG:
         setResetConfigState();
         break;
      default:
         setSystemErrorState();
         break;
   }
   blynk_timer.run();
}
