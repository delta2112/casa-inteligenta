#include <Arduino.h>
#include "BlynkSimpleStream.h"

#include "debug.h"
#include "initializer.h"

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

void setup() {
   deviceInitializer.begin();
   Blynk.begin(Serial, blynk_auth);
}

void loop() {
   Blynk.run();
   deviceInitializer.run();
}
