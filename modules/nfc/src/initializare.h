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
extern BlynkTimer blynk_timer;
extern char blynk_buffer[];

// Time related functions
void system_timer(void);
void nfc_timer(void);
void timeout_config(void);
void timeout_zavor(void);
void timeout_autentificare(void);

#endif  // INITIALIZARE_CONFIG_FILE
