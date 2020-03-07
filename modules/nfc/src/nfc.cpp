#include "debug.h"
#include "settings.h"
#include "nfc.h"

NFC nfc;

NFC::NFC() {
  for(byte i = 0; i < 6; i++) {
    key.keyByte[i] = nfc_secure_key_a.keyByte[i];
  }
}

void NFC::begin() {
  SPI.begin();
  mfrc522.PCD_Init(SS_PIN, RST_PIN);
}

void NFC::run(void) {
  // If no new card just exit run() and save uC processing time
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Read card serial number and return if that fails to save uC processing time
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // TODO: update card serial on blynk NFC::mfrc522.uid.size => NFC::mfrc522.uid.uidByte[i]

  byte data_buffer[18];
  if( is_valid_card_type() ) {
    if( authenticate_card(READ_KEYA, nfc_default_key_a, 3) ) {
      if( read_block(3, data_buffer) ) {
mfrc522.MIFARE_SetAccessBits(	&(data_buffer[6]),(byte)0b000, (byte)0b0000, (byte)0b000, (byte)0b0001 );
/// g0 < Access bits [C1 C2 C3] for block 0 (for sectors 0-31) or blocks 0-4 (for sectors 32-39)
/// g1 < Access bits C1 C2 C3] for block 1 (for sectors 0-31) or blocks 5-9 (for sectors 32-39)
/// g2 < Access bits C1 C2 C3] for block 2 (for sectors 0-31) or blocks 10-14 (for sectors 32-39)
/// g3 < Access bits C1 C2 C3] for the sector trailer, block 3 (for sectors 0-31) or block 15 (for sectors 32-39)

        for(byte i = 0; i < 16; i++) {
          DEBUG_PRINT(data_buffer[i], HEX);
          DEBUG_PRINT(" ");
        }
        DEBUG_PRINTLN("\n");
        // TODO: notify card uses default key (not secure!!)
      }
    } else {
      if( ! authenticate_card(READ_KEYA, nfc_secure_key_a, 3) ) {
        DEBUG_PRINT("Cannot read using either default or secure key");
      }
    }
  }
  detach_current_card();
}

bool NFC::authenticate_card(const enum MFRC522::PICC_Command key_type, MFRC522::MIFARE_Key key, byte block) { //
  MFRC522::StatusCode status;
  byte i;

  status = mfrc522.PCD_Authenticate(key_type, block, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status == MFRC522::STATUS_OK) {
    DEBUG_PRINT("Card detected: ");
    for(i = 0; i < mfrc522.uid.size; i++) {
      DEBUG_PRINT(mfrc522.uid.uidByte[i], HEX) //sak;
    }
    DEBUG_PRINTLN("\n")
    return true;
  } else {
    DEBUG_PRINT(F("Authentication failed for: "));
    for(i = 0; i < mfrc522.uid.size; i++) {
      DEBUG_PRINT(mfrc522.uid.uidByte[i], HEX) //sak;
    }
    DEBUG_PRINTLN(mfrc522.GetStatusCodeName(status));
  }
  return false;
}

bool NFC::read_block(byte block_number, byte buffer[18]) {
  MFRC522::StatusCode status;

  byte len = 18;

  status = mfrc522.MIFARE_Read(block_number, buffer, &len);
  if (status == MFRC522::STATUS_OK) {
    return true;
  } else {
    DEBUG_PRINT(F("Reading failed: "));
    DEBUG_PRINTLN(mfrc522.GetStatusCodeName(status));
  }
  return false;
}

bool NFC::is_valid_card_type() {
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  DEBUG_PRINT(F("PICC type: "));
  DEBUG_PRINT(mfrc522.PICC_GetTypeName(piccType));
  DEBUG_PRINT(F(" (SAK "));
  DEBUG_PRINT(mfrc522.uid.sak);
  DEBUG_PRINT(")\r\n");
  if (  piccType != MFRC522::PICC_TYPE_MIFARE_MINI 
    &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
    &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    DEBUG_PRINTLN(F("This sample only works with MIFARE Classic cards."));
    return false;
  }
  return true;
}

void NFC::detach_current_card(void) {
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

bool NFC::change_uid(byte new_uid[4]) {
  if ( mfrc522.MIFARE_SetUid(new_uid, (byte)4, true) ) {
    DEBUG_PRINTLN(F("Wrote new UID to card."));
    return true;
  }
  return false;
}

bool NFC::ascii_to_hex(const char digit, byte *destination) {
  if(digit >= 'A' && digit <= 'Z') {
    *destination = digit - 'A' + 10;
    return true;
  }
  if(digit >= 'a' && digit <= 'z') {
    *destination = digit - 'a' + 10;
    return true;
  }
  if(digit >= '0' && digit <= '9') {
    *destination = digit -'0';
    return true;
  }
  return false;
}

bool NFC::setKey(String s) {
  byte string_index = 0;
  byte key_index = 0;
  byte nibble = 0;
  if(12 != s.length()) {
    DEBUG_PRINTLN("Key string not right length. Should be 12 HEX number == 6 bytes");
    return false;
  }
  for(string_index = 0; string_index < 12; string_index+=2) {
    if(! ascii_to_hex( s[string_index], &nibble))  return false;
    key.keyByte[key_index  ] = nibble << 4;
    if(! ascii_to_hex( s[string_index+1], &nibble))  return false;
    key.keyByte[key_index++] |= nibble;
  }
  return true;
}

//byte = PCD_GetAntennaGain();
//void PCD_SetAntennaGain(byte mask);
//RxGain_18dB    RxGain_23dB    RxGain_18dB_2    RxGain_23dB_2    RxGain_33dB    RxGain_38dB    RxGain_43dB    RxGain_48dB    RxGain_min    RxGain_avg    RxGain_max
//void PCD_AntennaOn();  void PCD_AntennaOff(); bool PCD_PerformSelfTest();void PCD_SoftPowerDown();  void PCD_SoftPowerUp();
// void MIFARE_SetAccessBits(byte *accessBitBuffer, byte g0, byte g1, byte g2, byte g3);  bool MIFARE_OpenUidBackdoor(bool logErrors);
// bool MIFARE_SetUid(byte *newUid, byte uidSize, bool logErrors);  bool MIFARE_UnbrickUidSector(bool logErrors);
