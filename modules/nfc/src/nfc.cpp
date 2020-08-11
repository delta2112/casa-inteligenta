#include "debug.h"
#include "setari.h"
#include "stari-mgmt.h"
#include "dispozitiv.h"
#include "initializare.h"
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
  // Initializare machina de stari
  stare.set(IDLE);
}

void NFC::run(void) { // IDLE running on timer
    switch(stare.get()) {
        case IDLE:
            if(verifica_card_nou()) {
              autentificare();
            }
            break;
        case UPDATE_KEY:
            // update_key_on_card();
            break;
        case SAVE_NEW_KEY:
            //save_new_key();
            break;
        case DETACH:
            //detach_current_card();
            break;
        case NFC_ERROR:
            reinit();
            break;
        default:
            break;
    }
}

void NFC::autentificare(void) {
  bool autentificat = false;
  if(received_new_key) { // cheia noua, sigura, a fost receptionata
    // incercam autentificarea cu cheia noua (sigura)
    // primita de la aplicatie si salvata in membrul clasei
    // cu numele "key"
    if( authenticate_card(READ_KEYA, key, BLOC_AUTENTIFICARE) ) {
      autentificat = true;
      DEBUG_PRINTLN("Ne-am autentificat cu cheia sigura");
      if( update_key ) {
        update_key_on_card();
      } else {
        digitalWrite(PIN_ZAVOR,HIGH);
        blynk_timer.setTimer(zavor_config_timeout, timeout_zavor, 1);
        Blynk.virtualWrite(CHN_AUTH,NEW_AUTH_KEY); // informam serverul ca autentificarea a fost reusita
      }
    } else {
      detach_current_card(); // trebuie reinitializata comunicatia cu cardul pentru a incerca o cheie noua
      if(! verifica_card_nou()) { // dupa deconectare cardul trebuie sa se re-connecteze (se comporta ca un card nou)
        // daca acest card nu s-a reconectat, nu mai putem face nimic
        config_intarziere_autentificare();
        detach_current_card();
        return;
      }
    }
  }
  if(! autentificat) { // autentificarea cu cheia noua nu a fost posibila
    // incercam autentificarea cu cheia din repository (nfc_secure_key_a din setari.h)
    if( authenticate_card(READ_KEYA, nfc_secure_key_a, BLOC_AUTENTIFICARE) ) {
      if( update_key ) {
        update_key_on_card();
      } else {
        if(debug) {
          autentificat = true;
          DEBUG_PRINTLN("Ne-am autentificat cu cheia din repository");
          Blynk.virtualWrite(CHN_AUTH,SECURE_AUTH_KEY);
          digitalWrite(PIN_ZAVOR,HIGH);
          blynk_timer.setTimer(zavor_config_timeout, timeout_zavor, 1);
          Blynk.virtualWrite(CHN_AUTH,NEW_AUTH_KEY); // informam serverul ca autentificarea a fost reusita
        }
      }
    } else { // autentificarea cu cheia din repository nu a fost posibila
      detach_current_card(); // trebuie reinitializata comunicatia cu cardul pentru a incerca o cheie noua
      if(! verifica_card_nou()) { // dupa deconectare cardul trebuie sa se re-connecteze (se comporta ca un card nou)
        // daca acest card nu s-a reconectat, nu mai putem face nimic
        config_intarziere_autentificare();
        detach_current_card();
        return;
      }
    }
  }
  if(! autentificat) { // autentificarea cu cheia noua sau cea din repository nu a fost posibila
    // incercam autentificarea cu cheia implicita din fabrica (nfc_default_key_a din setari.h)
    if( authenticate_card(READ_KEYA, nfc_default_key_a, BLOC_AUTENTIFICARE) ) {
      autentificat = true;
      DEBUG_PRINTLN("Autentificat cu cheia din fabrica");
      Blynk.virtualWrite(CHN_AUTH,DEFAULT_AUTH_KEY);
      if(update_key) {
        update_key_on_card();
      } else {
        config_intarziere_autentificare();
        detach_current_card();
      }
    } else {
      config_intarziere_autentificare();
      detach_current_card();
    }
  }
}

void NFC::update_key_on_card(void) {
  stare.set(DETACH);
  if( update_key) {
    DEBUG_PRINT("Schibam cheia pe card cu: "); DEBUG_PRINTLN(key_to_update);

    if( read_block(BLOC_AUTENTIFICARE, card_data_buffer) ) {
      for( int i=15; i>=0; i--) {
        DEBUG_PRINT(card_data_buffer[i]);
        DEBUG_PRINT(":");
      }
      DEBUG_PRINTLN("");
      for( int i=10; i<16; i++) {
        card_data_buffer[i] = nfc_secure_key_a.keyByte[i-10];
      }
      for( int i=15; i>=0; i--) {
        DEBUG_PRINT(card_data_buffer[i]);
        DEBUG_PRINT(":");
      }
      DEBUG_PRINTLN("");
      write_block(BLOC_AUTENTIFICARE, card_data_buffer);
      //255:255:255:255:255:255:105:128:7:255:0:0:0:0:0:0:
      //55:100:145:219:254:202:105:128:7:255:0:0:0:0:0:0:

      //mfrc522.MIFARE_SetAccessBits(	&(card_data_buffer[6]),(byte)0b000, (byte)0b0000, (byte)0b000, (byte)0b0001 );
    /// g0 < Access bits [C1 C2 C3] for block 0 (for sectors 0-31) or blocks 0-4 (for sectors 32-39)
    /// g1 < Access bits C1 C2 C3] for block 1 (for sectors 0-31) or blocks 5-9 (for sectors 32-39)
    /// g2 < Access bits C1 C2 C3] for block 2 (for sectors 0-31) or blocks 10-14 (for sectors 32-39)
    /// g3 < Access bits C1 C2 C3] for the sector trailer, block 3 (for sectors 0-31) or block 15 (for sectors 32-39)

    } else {
      DEBUG_PRINTLN("READ BLOCK FAILED!!!");
    }
  }
  config_intarziere_autentificare();
  detach_current_card();
}

bool NFC::verifica_card_nou(void) {
  bool result = false;
  if(stare.isState(IDLE)) {
    if ( mfrc522.PICC_IsNewCardPresent()) { // Card nou detectat (sau redetectat in timpul unei autentificari)
      if ( mfrc522.PICC_ReadCardSerial()) { // Numar serial card cititi cu succes
        //Blynk.virtualWrite(CHN_CARD_UID, blynk_buffer, mfrc522.uid.size >> 1);
        if( is_valid_card_type() ) { // verifica daca avem un card pe care stim sa-l procesam
          Blynk.virtualWrite(V2,1); // inform server CARD is VALID
          result = true;
        } else {
          Blynk.virtualWrite(V2,0); // inform server CARD is NON-VALID
          result = false;
        } 
      }
    }
  }
  return result;
}

void NFC::set_key_to_update(byte auth) {
  key_to_update = (ListaStariAuth) auth;
  DEBUG_PRINT("Cheie pe care o schimbam pe card: "); DEBUG_PRINTLN(auth);
}

void NFC::set_permite_update_cheie(byte updatam) {
  update_key = (1 == updatam);
  DEBUG_PRINT("Updatam cheie: "); DEBUG_PRINTLN(updatam);
}

bool NFC::save_new_key(const unsigned char buffer[], size_t length) {
  unsigned int string_index = 0;
  DEBUG_PRINT("Salvam cheia noua: ")
  if(ascii_to_byte(buffer, length, key.keyByte)) {
    DEBUG_PRINT("Cheie salvata corect: ");
    for( string_index = 0; string_index < length/2; string_index++ ) {
      DEBUG_PRINT( key.keyByte[string_index] );
      DEBUG_PRINT(":");
    }
    DEBUG_PRINTLN("");
    received_new_key = true; // Cheie adaugat cu succes
    dispozitiv.configurare_terminata();
  } else {
    DEBUG_PRINTLN("Cheia nu este corecta");
    received_new_key = false;
  }
  return received_new_key;
}

bool NFC::authenticate_card(const enum MFRC522::PICC_Command key_type, MFRC522::MIFARE_Key key, byte block) { //
  MFRC522::StatusCode status;
  byte i;

  status = mfrc522.PCD_Authenticate(key_type, block, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status == MFRC522::STATUS_OK) {
    DEBUG_PRINT("Card autentificat: ");
  } else {
    DEBUG_PRINT(("Autentificare esuata: "));
  }
  for(i = 0; i < mfrc522.uid.size; i++) {
    DEBUG_PRINT( mfrc522.uid.uidByte[i] ); //sak
    DEBUG_PRINT( ":" );
  }
  DEBUG_PRINTLN("")

  if(status != MFRC522::STATUS_OK) {
    DEBUG_PRINT("Eroare autentificare: ");
    DEBUG_PRINTLN(mfrc522.GetStatusCodeName(status));
  }

  return status == MFRC522::STATUS_OK;
}

bool NFC::read_block(byte block_number, byte buffer[18]) {
  MFRC522::StatusCode status;

  byte len = 18;

  status = mfrc522.MIFARE_Read(block_number, buffer, &len);
  if (status == MFRC522::STATUS_OK) {
    return true;
  } else {
    DEBUG_PRINT("Citire esuata: ");
    DEBUG_PRINTLN(mfrc522.GetStatusCodeName(status));
  }
  return false;
}

bool NFC::write_block(byte block_number, byte buffer[16]) {
  MFRC522::StatusCode status;

  byte len = 16;

  status = mfrc522.MIFARE_Write(block_number, buffer, len);
  if (status == MFRC522::STATUS_OK) {
    return true;
  } else {
    DEBUG_PRINT("Scriere Esuata: ");
    DEBUG_PRINTLN(mfrc522.GetStatusCodeName(status));
  }
  return false;
}

bool NFC::is_valid_card_type() {
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  DEBUG_PRINT("Tip PICC (Card): ");
  DEBUG_PRINT(mfrc522.PICC_GetTypeName(piccType));
  DEBUG_PRINT(" (SAK ");
  DEBUG_PRINT( mfrc522.uid.sak );
  DEBUG_PRINTLN(")");
  if (  piccType != MFRC522::PICC_TYPE_MIFARE_MINI 
    &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
    &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    DEBUG_PRINTLN("Numai carduri MIFARE Classic pot fi folosite pentru autentificare.");
    return false;
  }
  return true;
}

void NFC::detach_current_card(void) {
  MFRC522::StatusCode result;
  result = mfrc522.PICC_HaltA();
  if(MFRC522::STATUS_OK != result) {
    DEBUG_PRINTLN("Nu pot opri comunicatia cu cardul (HALT)");
  }
  mfrc522.PCD_StopCrypto1();
  mfrc522.PCD_Init(SS_PIN, RST_PIN);
}

void NFC::config_intarziere_autentificare(void) {
  blynk_timer.setTimer(autentificare_timeout, timeout_autentificare, 1);
  stare.set(DETACH);
  DEBUG_PRINT("Dezactivare autentificare pentru: "); DEBUG_PRINTLN(autentificare_timeout);
}

void NFC::configureaza_idle(void) { // timer ca sa inchida zavorul dupa un interval prestabilit
   stare.set(IDLE);
   DEBUG_PRINTLN("Activare autentificare");
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
    DEBUG_PRINTLN("Wrote new UID to card.");
    return true;
  }
  return false;
}

bool NFC::ascii_to_byte(const unsigned char *ascii_string, byte size, byte result[]) {
  bool conversion_result = true;
  byte string_index = 0, result_index = 0;
  byte tmp;
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
    // DEBUG_PRINT("Byte convertit (DEC): "); DEBUG_PRINTLN( String(result[result_index]).c_str() );
    result_index++;
  }
  return conversion_result;
}
bool NFC::string_digit_to_byte(const char digit, byte *destination) {
  // DEBUG_PRINT("Convertim Byte (HEX): "); DEBUG_PRINTLN( String(digit).c_str() );
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
  result[dst_index] = 0; // Terminator sir caractere
  DEBUG_PRINT("HEX_STRING: "); DEBUG_PRINTLN(dst_index);
}

unsigned char NFC::byte_to_HEX_string(byte byte_number) {
  if((byte_number & 0x0F) < 10) return '0' + (byte_number & 0x0F);
  return 'A' + (byte_number & 0x0F) - 10;
}

bool NFC::cheie_noua_primita(void) {
  return received_new_key;
}

//byte = PCD_GetAntennaGain();
//void PCD_SetAntennaGain(byte mask);
//RxGain_18dB    RxGain_23dB    RxGain_18dB_2    RxGain_23dB_2    RxGain_33dB    RxGain_38dB    RxGain_43dB    RxGain_48dB    RxGain_min    RxGain_avg    RxGain_max
//void PCD_AntennaOn();  void PCD_AntennaOff(); bool PCD_PerformSelfTest();void PCD_SoftPowerDown();  void PCD_SoftPowerUp();
// void MIFARE_SetAccessBits(byte *accessBitBuffer, byte g0, byte g1, byte g2, byte g3);  bool MIFARE_OpenUidBackdoor(bool logErrors);
// bool MIFARE_SetUid(byte *newUid, byte uidSize, bool logErrors);  bool MIFARE_UnbrickUidSector(bool logErrors);
