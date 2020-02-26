#ifndef INITIALIZER_CONFIG_FILE
#define INITIALIZER_CONFIG_FILE

#include <BlynkSimpleStream.h>
#include <SoftwareSerial.h>

#include "settings.h"

inline void DeviceState::set(State m) {
   if (state != m) {  // change state only if different from current one
#if defined(APP_DEBUG)
      DEBUG_PRINT(String(StateStr[state]) + " => " + StateStr[m]);
#endif
      state = m;
   }
}

class Initializer {
  public:
   void begin();  // configure everything and start processes
   void run();    // runs all framework loops
};

Initializer deviceInitializer;  // create Initializer instance

#endif  // INITIALIZER_CONFIG_FILE
