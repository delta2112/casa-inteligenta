#include "initializer.h"

void Initializer::begin() {
#if defined(APP_DEBUG)
   DEBUG_PRINT("");
   DEBUG_PRINT("Hardware v" + String(NFC_FW_VER));
   DEBUG_PRINT("Firmware v" + String(NFC_HW_VER));
   DEBUG_PRINT("NFC Shield" + String(NFC_SHIELD));
#endif

   // indicator_init(); // initialize indicators if present (LEDs, etc.) =>
   // separate class with own .cpp / .h files.

   // button_init(); // initialize reset button or "press to exit" buttons =>
   // also separate classes.

   // config_init();    // initialize configuration stored in eeprom => here
   // again separate classes

   if (configStore.flagConfig) {  // if devices has network configuration stored
                                  // in eeprom switch state to connecting else
                                  // wait to be configured by application
#if defined(APP_DEBUG)
      DEBUG_PRINT("Trying to connect");
#endif
      DeviceState::set(MODE_CONNECTING_NET);
   } else {
#if defined(APP_DEBUG)
      DEBUG_PRINT("Waiting for config");
#endif
      DeviceState::set(MODE_WAIT_CONFIG);
   }
}

void Initializer::run() {  // this is the actual state machine of the
                           // application
   switch (DeviceState::get()) {
      case WAIT_CONFIG:
      case CONFIGURING:
         setConfigState();
         break;
      case RUNNING:
         Blynk.run();
         Nfc.run();
         break;
      case RESET_CONFIG:
         setResetState();
         break;
      default:
         setErrorState();
         break;
   }
}
