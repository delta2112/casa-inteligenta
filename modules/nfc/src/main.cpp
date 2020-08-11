#include <Arduino.h>

#ifdef ARDUINO_AVR_ATmega328P
   #include "BlynkSimpleStream.h"
#endif
#ifdef ARDUINO_ESP8266_WEMOS_D1MINILITE
   #define BLYNK_PRINT Serial
   #include <BlynkSimpleEsp8266.h>
#endif

#include "debug.h"
#include "initializare.h"

BLYNK_CONNECTED() {
   Blynk.syncAll();
}

void setup() {
   deviceInitializare.begin();
   #ifdef ARDUINO_AVR_ATmega328P
      Serial.begin(9600);
      Blynk.begin(Serial, blynk_auth);
   #endif
   #ifdef ARDUINO_ESP8266_WEMOS_D1MINILITE
      Blynk.begin(blynk_auth, ssid, pass);
   #endif
}


void loop() {
   deviceInitializare.run();
   Blynk.run();
}
