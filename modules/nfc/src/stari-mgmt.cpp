#include "debug.h"
#include "stari-mgmt.h"
#include "nfc.h"

#if defined(APP_DEBUG)
const char* StareSystemStr[MAX_SYSTEM_CONFIG_VALUE] = {
    "SYS:WAIT_CONFIG",   // waiting for configuration from aplication
    "SYS:CONFIGURING",   // configuration started
    "SYS:RUNNING",       // normal runnig stare
    "SYS:RESET_CONFIG",  // configuration initialized 
    "SYS:SYSTEM_ERROR"   // something went wrong, device should reset in this stare?
};

const char* StareNfcStr[MAX_NFC_CONFIG_VALUE] = {
    "NFC:IDLE",
    "NFC:UPDATE_KEY",
    "NFC:SAVE_NEW_KEY",
    "NFC:READ_DATA",
    "NFC:WRITE_DATA",
    "NFC:DETACH",
    "NFC:NFC_ERROR"
};

void StareSystem::run(void) { // this is the actual stare machine of the
                              // application
   switch (stare) {
      case WAIT_CONFIG:
         break;
      case CONFIGURING:
         stare = CONFIGURING;
         break;
      case RUNNING:
         break;
      case RESET_CONFIG:
         break;
      default:
         stare = SYSTEM_ERROR;
         break;
   }
}

void StareNfc::run(void) {
    switch(stare) {
        case IDLE:
            break; // idle is run by timer, but we need to account for it here
        case UPDATE_KEY:
            nfc.update_key_on_card();
            break;
        case SAVE_NEW_KEY:
            stareSystem.set(RUNNING);
            break;
        case READ_DATA:
            nfc.read_data();
            break;
        case WRITE_DATA:
            nfc.write_data();
            break;
        case DETACH:
            nfc.detach_current_card();
            stareNfc.set(IDLE);
            break;
        case NFC_ERROR:
            nfc.reinit();
            break;
        default:
            nfc.error();
    }
}

StareSystem stareSystem;
StareNfc stareNfc;
#endif
