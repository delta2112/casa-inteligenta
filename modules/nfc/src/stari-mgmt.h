#ifndef STARI_MGMT_CONFIG_FILE
#define STARI_MGMT_CONFIG_FILE

#include "debug.h"
enum ListaStariSystem {
    WAIT_CONFIG,
    CONFIGURING,
    RUNNING,
    RESET_CONFIG,
    SYSTEM_ERROR,

    MAX_SYSTEM_CONFIG_VALUE // keep this entry the last one to use as "length" of enum
                            // for other defines
};
enum ListaStariNfc {
    IDLE,        // waiting for things to happen
    UPDATE_KEY,  // and the new presented card
    SAVE_NEW_KEY,// save it inside the class "key" attribute
    READ_DATA,   // read data from card
    WRITE_DATA,  // write data to card
    DETACH,      // communication finished, no other actions needed, detach card

    NFC_ERROR,   // nfc working error

    MAX_NFC_CONFIG_VALUE // keep this entry the last one to use as "length" of enum
                         // for other defines
};

#if defined(APP_DEBUG)
extern const char* StareSystemStr[];
extern const char* StareNfcStr[];
#endif

template <typename T>
class Stare {
    protected:
        volatile T stare;
    #if defined(APP_DEBUG)
        const char** StareStr;
    #endif

    public:
        T get() { return stare; }
        bool isState(T stare_noua) { return (stare == stare_noua); }
        void set(T stare_noua) {
            if (stare != stare_noua) {  // change state only if different from current one
                DEBUG_PRINTLN(String(StareStr[stare]) + " => " + StareStr[stare_noua]);
                stare = stare_noua;
            }
        }
        virtual void run(void);
};

class StareSystem: public Stare<ListaStariSystem> {
    public:
        #if defined(APP_DEBUG)
        StareSystem(void) {
            StareStr = StareSystemStr;
        }
        #endif
        void run(void);
};
class StareNfc: public Stare<ListaStariNfc> {
    public:
        #if defined(APP_DEBUG)
        StareNfc(void) {
            StareStr = StareNfcStr;
        }
        #endif
        void run(void);
};

extern StareSystem stareSystem;
extern StareNfc stareNfc;

#endif  // STARI_MGMT_CONFIG_FILE
