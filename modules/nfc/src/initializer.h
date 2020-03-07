#ifndef INITIALIZER_CONFIG_FILE
#define INITIALIZER_CONFIG_FILE

#include "settings.h"
#include "state-mgmt.h"
#include "nfc.h"

class Initializer {
   public:
      void begin(void);  // configure everything and start processes
      void run(void);    // runs all framework loops

      void setWaitConfigState(void)  { systemState.set(WAIT_CONFIG); }
      void setConfiguringState(void) { systemState.set(CONFIGURING); }
      void setRunningState(void)     { systemState.set(RUNNING); }
      void setResetConfigState(void) { systemState.set(RESET_CONFIG); }
      void setSystemErrorState(void) { systemState.set(SYSTEM_ERROR); }

      void setCardDetectState(void)  { nfcState.set(CARD_DETECT); }
      void setDefaultKeyState(void)  { nfcState.set(DEFAULT_KEY); }
      void setSecureKeyState(void)   { nfcState.set(SECURE_KEY); }
      void setUpdateKeyState(void)   { nfcState.set(UPDATE_KEY); }
      void setIdleState(void)        { nfcState.set(IDLE); }
      void setReadDataState(void)    { nfcState.set(READ_DATA); }
      void setWriteDataState(void)   { nfcState.set(WRITE_DATA); }
      void setNfcErrorState(void)    { nfcState.set(NFC_ERROR); }

   private:
      SystemState systemState;
      NfcState nfcState;
};

extern Initializer deviceInitializer;

#endif  // INITIALIZER_CONFIG_FILE
