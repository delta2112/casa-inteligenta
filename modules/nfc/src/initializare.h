#ifndef INITIALIZARE_CONFIG_FILE
#define INITIALIZARE_CONFIG_FILE

#include "setari.h"
#include "stari-mgmt.h"
#include "dispozitiv.h"
#include "nfc.h"

class Initializare {
   public:
      void begin(void);  // configure everything and start processes
      void run(void);    // runs all framework loops
};

extern Initializare deviceInitializare;

#endif  // INITIALIZARE_CONFIG_FILE
