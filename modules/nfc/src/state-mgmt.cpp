#include "debug.h"
#include "state-mgmt.h"

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

SystemState systemState;
NfcState nfcState;
#endif
