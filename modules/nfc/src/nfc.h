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
    void run(void);
    bool setKey( String s);

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


    bool programMode = false;  // initialize programming mode to false
    uint8_t successRead;  // Variable integer to keep if we have Successful Read
    byte storedCard[4];  // Stores an ID read from EEPROM
    byte readCard[4];    // Stores scanned ID read from RFID Module
    byte masterCard[4];  // Stores master card's ID read from EEPROM
                          // from Reader
};

extern NFC nfc;

#endif  // NFC_CONFIG_FILE
