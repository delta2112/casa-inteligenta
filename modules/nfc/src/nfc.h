#ifndef NFC_CONFIG_FILE
#define NFC_CONFIG_FILE

#include <SPI.h>
#include <MFRC522.h>

#include "settings.h"
#include "debug.h"

constexpr enum MFRC522::PICC_Command READ_KEYA = MFRC522::PICC_Command::PICC_CMD_MF_AUTH_KEY_A;
constexpr enum MFRC522::PICC_Command READ_KEYB = MFRC522::PICC_Command::PICC_CMD_MF_AUTH_KEY_B;

class NFC {
  public:
    NFC();
    void begin();
    void run(void);              // idle loop processing - called run for consistency
    bool save_new_key(String s); // store received key from Blynk in internal class "key" attribute
    void update_key(void);       // changed key on the Mifare card
    void card_detected(void);    // process card detection
    void default_key_auth(void);
    void secure_key_auth(void);
    void new_key_auth(void);
    void read_data(void);
    void write_data(void);
    void reinit(void);
    void error(void);

  private:
    bool authenticate_card(const enum MFRC522::PICC_Command, MFRC522::MIFARE_Key, byte);
    bool read_block(byte block_number, byte buffer[18]);
    bool write_block(byte block_number, byte buffer[16]);
    bool is_valid_card_type();
    bool change_uid(byte new_uid[4]);
    void detach_current_card(void);

    bool ascii_to_hex(const char digit, byte *destination);

    MFRC522 mfrc522;
    MFRC522::MIFARE_Key key;

    byte card_data_buffer[18];
    bool updateKey = false;
};

extern NFC nfc;

#endif  // NFC_CONFIG_FILE
