#include "debug.h"
#include "state-mgmt.h"
#include "nfc.h"

#if defined(APP_DEBUG)
const char* SystemStateStr[MAX_SYSTEM_CONFIG_VALUE] = {
    "SYS:WAIT_CONFIG",   // waiting for configuration from aaplication
    "SYS:CONFIGURING",   // configuration started
    "SYS:RUNNING",       // normal runnig state
    "SYS:RESET_CONFIG",  // configuration initialized 
    "SYS:SYSTEM_ERROR"   // something went wrong, device should reset in this state?
};

const char* NfcStateStr[MAX_NFC_CONFIG_VALUE] = {
    "NFC:CARD_DETECT",
    "NFC:DEFAULT_KEY",
    "NFC:SECURE_KEY",
    "NFC:NEW_KEY_AUTH",
    "NFC:UPDATE_KEY",
    "NFC:SAVE_NEW_KEY",
    "NFC:IDLE",
    "NFC:READ_DATA",
    "NFC:WRITE_DATA",
    "NFC:DETACH",
    "NFC:NFC_ERROR"
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
            break; // idle is run by timer, but we need to account for it here
        case CARD_DETECT:
            nfc.card_detected();
            break;
        case DEFAULT_KEY:
            nfc.default_key_auth();
            break;
        case SECURE_KEY:
            nfc.secure_key_auth();
            break;
        case NEW_KEY_AUTH:
            nfc.new_key_auth();
            break;
        case UPDATE_KEY:
            nfc.update_key_on_card();
            break;
        case SAVE_NEW_KEY:
            systemState.set(RUNNING);
            break;
        case READ_DATA:
            nfc.read_data();
            break;
        case WRITE_DATA:
            nfc.write_data();
            break;
        case DETACH:
            nfc.detach_current_card();
            nfcState.set(IDLE);
            break;
        case NFC_ERROR:
            nfc.reinit();
            break;
        default:
            nfc.error();
    }
}

SystemState systemState;
NfcState nfcState;
#endif
