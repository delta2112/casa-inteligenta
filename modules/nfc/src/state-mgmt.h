#ifndef STATE_MGMT_CONFIG_FILE
#define STATE_MGMT_CONFIG_FILE

#include "debug.h"
enum SystemStatesList {
    WAIT_CONFIG,
    CONFIGURING,
    RUNNING,
    RESET_CONFIG,
    SYSTEM_ERROR,

    MAX_SYSTEM_CONFIG_VALUE // keep this entry the last one to use as "length" of enum
                            // for other defines
};
enum NfcStatesList {
    CARD_DETECT,
    DEFAULT_KEY,
    SECURE_KEY,
    UPDATE_KEY,
    IDLE,
    READ_DATA,
    WRITE_DATA,

    NFC_ERROR,

    MAX_NFC_CONFIG_VALUE // keep this entry the last one to use as "length" of enum
                         // for other defines
};

#if defined(APP_DEBUG)
extern const char* SystemStateStr[];
extern const char* NfcStateStr[];
#endif

template <typename T>
class State {
    private:
        volatile T state;
    #if defined(APP_DEBUG)
    protected:
        const char** StateStr;
    #endif
    public:
        T get() { return state; }
        bool isState(T new_state) { return (state == new_state); }
        void set(T new_state) {
            if (state != new_state) {  // change state only if different from current one
                DEBUG_PRINTLN(String(StateStr[state]) + " => " + StateStr[new_state]);
                state = new_state;
            }
        }
};

class SystemState: public State<SystemStatesList> {
    public:
        #if defined(APP_DEBUG)
        SystemState(void) {
            StateStr = SystemStateStr;
        }
        #endif
};
class NfcState: public State<NfcStatesList> {
    public:
        #if defined(APP_DEBUG)
        NfcState(void) {
            StateStr = NfcStateStr;
        }
        #endif
};

extern SystemState systemState;
extern NfcState    nfcState;

#endif  // STATE_MGMT_CONFIG_FILE
