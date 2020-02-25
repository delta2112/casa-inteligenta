#include <BlynkSimpleStream.h>
#include <SoftwareSerial.h>

#include "settings.h"

inline
void DeviceState::set(State m) {
  if (state != m) {
    DEBUG_PRINT(String(StateStr[state]) + " => " + StateStr[m]);
    state = m;
  }
}

class Initializer {

public:
  void begin();
  void run();
};

Initializer DeviceInitializer;
