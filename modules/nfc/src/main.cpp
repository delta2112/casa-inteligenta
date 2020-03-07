#include <Arduino.h>
#include <BlynkSimpleStream.h>

#include "debug.h"
#include "initializer.h"

void setup() {
   deviceInitializer.begin();
   Blynk.begin(Serial, blynk_auth);
}

void loop() {
   Blynk.run();
   deviceInitializer.run();
}
