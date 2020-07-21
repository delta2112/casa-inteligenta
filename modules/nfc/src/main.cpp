#include <Arduino.h>
#include "BlynkSimpleStream.h"

#include "debug.h"
#include "initializare.h"

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

void setup() {
   deviceInitializare.begin();
   Blynk.begin(Serial, blynk_auth);
}


void loop() {
   deviceInitializare.run();
   Blynk.run();
}
