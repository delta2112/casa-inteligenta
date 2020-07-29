// #include <Blynk/BlynkApi.h>

#include "BlynkSimpleStream.h"
#include "debug.h"
#include "setari.h"
#include "stari-mgmt.h"
#include "nfc.h"

NFC nfc;

NFC::NFC() {
  for(byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0;
  }
}

void NFC::begin() {
  SPI.begin();
  mfrc522.PCD_Init(SS_PIN, RST_PIN);
  // Initializare machin de stari
  stare.set(IDLE);
}

void NFC::run(void) { // IDLE running on timer
    switch(stare.get()) {
        case IDLE:
            processNewCard();
            break;
        case UPDATE_KEY:
            update_key_on_card();
            break;
        case SAVE_NEW_KEY:
            //save_new_key();
            break;
        case READ_DATA:
            read_data();
            break;
        case WRITE_DATA:
            write_data();
            break;
        case DETACH:
            detach_current_card();
            break;
        case NFC_ERROR:
            reinit();
            break;
        default:
            error();
    }
}

void NFC::processNewCard(void) {
  if(stare.isState(IDLE)) {
    if ( ! mfrc522.PICC_IsNewCardPresent()) { // If no new card just exit run() and save uC processing time
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) { // Read card serial number and return if that fails to save uC processing time
      return;
    }
    Blynk.virtualWriteBinary(CHN_CARD_UID, mfrc522.uid.uidByte, mfrc522.uid.size); // inform server of new CARD processing UID
    // char UID[15];
    //for(byte uid_index = 0; uid_index< mfrc522.uid.size; uid_index++) {
    //  char[]
    //}
    if( is_valid_card_type() ) {
      Blynk.virtualWrite(V2,1); // inform server CARD is VALID
    } else {
      Blynk.virtualWrite(V2,0); // inform server CARD is NON-VALID
    }
    autentificare();
  }
}

void NFC::autentificare(void) {
  if(received_new_key) {
    if( authenticate_card(READ_KEYA, key, 3) ) { // try new key auth
      DEBUG_PRINTLN("Authenticated with NEW key");
      Blynk.virtualWrite(V3,NEW_AUTH_KEY);
      stare.set(READ_DATA);
    } else {
      detach_current_card(); // re-init communication to try different key
      if ( ! mfrc522.PICC_IsNewCardPresent()) return;
      if ( ! mfrc522.PICC_ReadCardSerial()) return;
      if( authenticate_card(READ_KEYA, nfc_secure_key_a, 3) ) {
        DEBUG_PRINTLN("Authenticated with SECURE key");
        Blynk.virtualWrite(V3,SECURE_AUTH_KEY);
        stare.set(READ_DATA);
      } else {
        detach_current_card();
        if ( ! mfrc522.PICC_IsNewCardPresent()) return;
        if ( ! mfrc522.PICC_ReadCardSerial()) return;
        if( authenticate_card(READ_KEYA, nfc_default_key_a, 3) ) {
          DEBUG_PRINTLN("Authenticated with DEFAULT key");
          Blynk.virtualWrite(V3,DEFAULT_AUTH_KEY);
          stare.set(READ_DATA);
        } else {
          detach_current_card();
          stare.set(IDLE);
        }
      }
    }
  } else {
    if( authenticate_card(READ_KEYA, nfc_secure_key_a, 3) ) {
      DEBUG_PRINTLN("Authenticated with SECURE key");
      Blynk.virtualWrite(V3,SECURE_AUTH_KEY);
      stare.set(READ_DATA);
    } else {
      detach_current_card();
      if ( ! mfrc522.PICC_IsNewCardPresent()) return;
      if ( ! mfrc522.PICC_ReadCardSerial()) return;
      if( authenticate_card(READ_KEYA, nfc_default_key_a, 3) ) {
        DEBUG_PRINTLN("Authenticated with DEFAULT key");
        Blynk.virtualWrite(V3,DEFAULT_AUTH_KEY);
        stare.set(READ_DATA);
      } else {
        detach_current_card();
        stare.set(IDLE);
      }
    }
  }
}

void NFC::set_key_to_update(byte auth) {
  key_to_update = (ListaStariAuth) auth;
}
bool NFC::save_new_key(const unsigned char buffer[], size_t length) {
  byte string_index = 0;
  DEBUG_PRINT("RECEIVED NEW KEY: ")
  if(ascii_to_byte(buffer, length, key.keyByte)) {
    DEBUG_PRINT("Key saved correctly");
    for( string_index = 0; string_index < length; string_index++ ) {
      DEBUG_PRINT(key.keyByte[string_index]);
      DEBUG_PRINT(":");
    }
  }
  DEBUG_PRINTLN("");
/*  byte key_index = 0;
  byte nibble = 0;
  if(12 != new_key.length()) {
    DEBUG_PRINTLN(String("Key string not right length. Should be 12 HEX number == 6 bytes and is: ") + new_key.length());
    return false;
  }
  for(string_index = 0; string_index < 12; string_index+=2) {
    // MSB of next byte
    if(! ascii_to_hex( new_key[string_index], &nibble)) {
      DEBUG_PRINTLN(String("Invalid Key HEX digit: ") + new_key[string_index]);
      return false;
    }
    key.keyByte[key_index  ] = nibble << 4;

    // LSB of current byte
    if(! ascii_to_hex( new_key[string_index+1], &nibble)) {
      DEBUG_PRINTLN(String("Invalid Key HEX digit: ") + new_key[string_index+1]);
      return false;
    }
    key.keyByte[key_index++] |= nibble;
  }*/

  // key was successfuly added, and it will be used afterwards
  received_new_key = true;
  return true;
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
    DEBUG_PRINTLN("\n");
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
  MFRC522::StatusCode result;
  result = mfrc522.PICC_HaltA();
  if(MFRC522::STATUS_OK != result) {
    DEBUG_PRINTLN("Cannot HALT communication with card");
  }
  mfrc522.PCD_StopCrypto1();
  mfrc522.PCD_Init(SS_PIN, RST_PIN);
}

void NFC::update_key_on_card(void) {
  if( updateKey) {
    // if( use new key )
    if( read_block(3, card_data_buffer) ) {
    mfrc522.MIFARE_SetAccessBits(	&(card_data_buffer[6]),(byte)0b000, (byte)0b0000, (byte)0b000, (byte)0b0001 );
    /// g0 < Access bits [C1 C2 C3] for block 0 (for sectors 0-31) or blocks 0-4 (for sectors 32-39)
    /// g1 < Access bits C1 C2 C3] for block 1 (for sectors 0-31) or blocks 5-9 (for sectors 32-39)
    /// g2 < Access bits C1 C2 C3] for block 2 (for sectors 0-31) or blocks 10-14 (for sectors 32-39)
    /// g3 < Access bits C1 C2 C3] for the sector trailer, block 3 (for sectors 0-31) or block 15 (for sectors 32-39)

        for(byte i = 0; i < 16; i++) {
          DEBUG_PRINT(card_data_buffer[i], HEX);
          DEBUG_PRINT(" ");
        }
        DEBUG_PRINTLN("\n");
        // TODO: notify card uses default key (not secure!!)
      }
  }
  stare.set(DETACH);
}

void NFC::read_data(void) {
  stare.set(IDLE);
  detach_current_card();
}
void NFC::write_data(void) {
  stare.set(IDLE);
}
void NFC::reinit(void) {
}
void NFC::error(void) {
}

bool NFC::change_uid(byte new_uid[4]) {
  if ( mfrc522.MIFARE_SetUid(new_uid, (byte)4, true) ) {
    DEBUG_PRINTLN(F("Wrote new UID to card."));
    return true;
  }
  return false;
}

bool NFC::ascii_to_byte(const unsigned char *ascii_string, byte size, byte result[]) {
  bool conversion_result = true;
  byte string_index = 0, result_index = 0;
  byte tmp;
  DEBUG_PRINTLN(String("Received size: ") + size);
  for(string_index = 0; string_index < size; string_index++) {
    conversion_result = string_digit_to_byte(ascii_string[string_index++], &tmp);
    if(conversion_result) result[result_index] = tmp << 4;
    else {
      conversion_result = false;
      break;
    }
    conversion_result = string_digit_to_byte(ascii_string[string_index], &tmp);
    if(conversion_result) result[result_index] |= tmp;
    else {
      conversion_result = false;
      break;
    }
    DEBUG_PRINT("Converted byte: "); DEBUG_PRINTLN(result[result_index], HEX);
    result_index++;
  }
  return conversion_result;
}
bool NFC::string_digit_to_byte(const char digit, byte *destination) {
  DEBUG_PRINT("Converting byte: "); DEBUG_PRINTLN(digit);
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

void NFC::hex_to_ascii(const unsigned char *hex_array, byte size, char *result) {
  byte src_index = 0, dst_index = 0;
  for(src_index = 0; src_index < size; src_index ++ ) {
    result[dst_index++] = byte_to_HEX_string(hex_array[src_index] >> 4);
    result[dst_index++] = byte_to_HEX_string(hex_array[src_index]);
  }
  result[dst_index] = 0; // String terminator
}

unsigned char NFC::byte_to_HEX_string(byte byte_number) {
  if((byte_number & 0x0F) < 10) return '0' + (byte_number & 0x0F);
  return 'A' + (byte_number & 0x0F) - 10;
}
//byte = PCD_GetAntennaGain();
//void PCD_SetAntennaGain(byte mask);
//RxGain_18dB    RxGain_23dB    RxGain_18dB_2    RxGain_23dB_2    RxGain_33dB    RxGain_38dB    RxGain_43dB    RxGain_48dB    RxGain_min    RxGain_avg    RxGain_max
//void PCD_AntennaOn();  void PCD_AntennaOff(); bool PCD_PerformSelfTest();void PCD_SoftPowerDown();  void PCD_SoftPowerUp();
// void MIFARE_SetAccessBits(byte *accessBitBuffer, byte g0, byte g1, byte g2, byte g3);  bool MIFARE_OpenUidBackdoor(bool logErrors);
// bool MIFARE_SetUid(byte *newUid, byte uidSize, bool logErrors);  bool MIFARE_UnbrickUidSector(bool logErrors);
