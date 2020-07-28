#include "debug.h"
#include "stari-mgmt.h"

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

#endif
