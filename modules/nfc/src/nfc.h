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
    NFC();                         // constructor to initialize internal "key" with 0x00
    void begin();                  // initialize nfc module
    void run(void);                // idle loop processing - named "run" for consistency
    void card_detected(void);      // process card detection and push card UID to Blynk server
    void new_key_auth(void);       // try to authenticate card with new key received from
                                   // Blynk server
    void secure_key_auth(void);    // try to authenticate card with secure Mifare key
                                   // stored in software
    void default_key_auth(void);   // try to authenticate card with default Mifare key (6 x 0xFF)
    bool save_new_key(char[], size_t);   // store received key from Blynk in internal class
                                   // attribute: "key"
    void set_key_to_update(byte);
    void update_key_on_card(void); // changed key on the Mifare card with "new key"
                                   // stored in "key" attribute
    void read_data(void);          // read data from card
    void write_data(void);         // write data to card
    void reinit(void);             // 
    void detach_current_card(void);
    void error(void);              // 

  private:
    bool authenticate_card(const enum MFRC522::PICC_Command, MFRC522::MIFARE_Key, byte);
    bool read_block(byte block_number, byte buffer[18]);
    bool write_block(byte block_number, byte buffer[16]);
    bool is_valid_card_type();
    bool change_uid(byte new_uid[4]);

    bool ascii_to_hex(const char digit, byte *destination);
    unsigned char byte_to_HEX_string(byte byte_number);
    void hex_to_ascii(const unsigned char[], byte, char[]);

    MFRC522 mfrc522;
    MFRC522::MIFARE_Key key;
    bool received_new_key = false;
    char uid[15];
    AuthStatus key_to_update; 

    byte card_data_buffer[18];
    bool updateKey = false;
};

extern NFC nfc;

#endif  // NFC_CONFIG_FILE
