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
   Blynk.begin(Serial, blynk_auth);
   Serial.begin(9600);
   Blynk.begin(auth, ssid, pass);

}


void loop() {
   deviceInitializare.run();
   Blynk.run();
}
