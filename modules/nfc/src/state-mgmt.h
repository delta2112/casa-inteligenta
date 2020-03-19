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
    CARD_DETECT, // card detected near sensor
    DEFAULT_KEY, // authenticated with default Mifare key
    SECURE_KEY,  // authenticated with secure key stored inside project
    NEW_KEY_AUTH,// authenticated with the new key stored inside class "key" attribute
    UPDATE_KEY,  // and the new presented card
    SAVE_NEW_KEY,// save it inside the class "key" attribute
    IDLE,        // waiting for things to happen
    READ_DATA,   // read data from card
    WRITE_DATA,  // write data to card
    DETACH,      // communication finished, no other actions needed, detach card

    NFC_ERROR,   // nfc working error

    MAX_NFC_CONFIG_VALUE // keep this entry the last one to use as "length" of enum
                         // for other defines
};

#if defined(APP_DEBUG)
extern const char* SystemStateStr[];
extern const char* NfcStateStr[];
#endif

template <typename T>
class State {
    protected:
        volatile T state;
    #if defined(APP_DEBUG)
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
        virtual void run(void);
};

class SystemState: public State<SystemStatesList> {
    public:
        #if defined(APP_DEBUG)
        SystemState(void) {
            StateStr = SystemStateStr;
        }
        #endif
        void run(void);
};
class NfcState: public State<NfcStatesList> {
    public:
        #if defined(APP_DEBUG)
        NfcState(void) {
            StateStr = NfcStateStr;
        }
        #endif
        void run(void);
};

extern SystemState systemState;
extern NfcState    nfcState;

#endif  // STATE_MGMT_CONFIG_FILE
