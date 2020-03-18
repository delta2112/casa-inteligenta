#include "debug.h"
#include "state-mgmt.h"
#include "nfc.h"

#if defined(APP_DEBUG)
const char* SystemStateStr[MAX_SYSTEM_CONFIG_VALUE] = {
    "WAIT_CONFIG",   // waiting for configuration from aaplication
    "CONFIGURING",   // configuration started
    "RUNNING",       // normal runnig state
    "RESET_CONFIG",  // configuration initialized 
    "ERROR"          // something went wrong, device should reset in this state?
};
const char* NfcStateStr[MAX_NFC_CONFIG_VALUE] = {
    "WAIT_CONFIG",   // waiting for configuration from aaplication
    "CONFIGURING",   // configuration started
    "RUNNING",       // normal runnig state
    "RESET_CONFIG",  // configuration initialized 
    "ERROR"          // something went wrong, device should reset in this state?
};

void SystemState::run(void) { // this is the actual state machine of the
                              // application
   switch (state) {
      case WAIT_CONFIG:
         break;
      case CONFIGURING:
         state = CONFIGURING;
         break;
      case RUNNING:
         break;
      case RESET_CONFIG:
         break;
      default:
         state = SYSTEM_ERROR;
         break;
   }
}

void NfcState::run(void) {
    switch(state) {
        case IDLE:
            nfc.detach_current_card();
            break; // idle is run by timer, but we need to account for it here
        case CARD_DETECT:
            nfc.card_detected();
            break;
        case DEFAULT_KEY:
            nfc.default_key_auth();
        case SECURE_KEY:
            nfc.secure_key_auth();
        case NEW_KEY_AUTH:
            nfc.new_key_auth();
        case UPDATE_KEY:
            nfc.update_key_on_card();
        case SAVE_NEW_KEY:
            systemState.set(RUNNING);
        case READ_DATA:
            nfc.read_data();
        case WRITE_DATA:
            nfc.write_data();
        case NFC_ERROR:
            nfc.reinit();
        default:
            nfc.error();
    }
}

SystemState systemState;
NfcState nfcState;
#endif
