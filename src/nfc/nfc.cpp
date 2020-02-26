#include "nfc.h"

NFC : NFC() {
   // Arduino Pin Configuration
   pinMode(interrupt_pin, INPUT_PULLUP);
   pinMode(relay_pin, OUTPUT);
   pinMode(exit_button_pin, INPUT_PULLUP);
   pinMode(erase_button_pin, INPUT_PULLUP);
   pinMode(door_open, INPUT_PULLUP);

   // Setting initial conditions.
   digitalWrite(relay, RELAY_ON);  // Make sure door is locked

   // Configure hardware communication
   SPI.begin();         // MFRC522 Hardware uses SPI protocol
   mfrc522.PCD_Init();  // Initialize MFRC522 Hardware

   // If you set Antenna Gain to Max it will increase reading distance
   if (config.antennaGain) {
#if defined(APP_DEBUG)
      DEBUG_PRINT(F("Setting antenna gain to: " + config.antennaGain))
#endif
      mfrc522.PCD_SetAntennaGain(config.antennaGain);  // mfrc522.RxGain_max
   }

#if defined(APP_DEBUG)
   DEBUG_PRINT(F("Access Control Example v0.1"))
#endif
}
