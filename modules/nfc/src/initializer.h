#ifndef INITIALIZER_CONFIG_FILE
#define INITIALIZER_CONFIG_FILE

#include "settings.h"
#include "state-mgmt.h"
#include "nfc.h"

class Initializer {
   public:
      void begin(void);  // configure everything and start processes
      void run(void);    // runs all framework loops
};

extern Initializer deviceInitializer;

#endif  // INITIALIZER_CONFIG_FILE
