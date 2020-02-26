#ifndef STATE_MGMT_CONFIG_FILE
#define STATE_MGMT_CONFIG_FILE

enum State {
   WAIT_CONFIG,
   CONFIGURING,
   RUNNING,
   RESET_CONFIG,
   ERROR,

   MAX_CONFIG_VALUE  // keep this entry the last one to use as "length" of enum
                     // for other defines
};

#if defined(APP_DEBUG)
const char* StateStr[MAX_CONFIG_VALUE] = {
    "WAIT_CONFIG",   // waiting for configuration from aaplication
    "CONFIGURING",   // configuration started
    "RUNNING",       // normal runnig state
    "RESET_CONFIG",  // configuration initialized
    "ERROR"          // something went wrong, device should reset in this state?
};
#endif

namespace DeviceState {
volatile State state;

State get() { return state; }
bool isState(State m) { return (state == m); }
void set(State m);
};  // end of namespace DeviceState

#endif  // STATE_MGMT_CONFIG_FILE
