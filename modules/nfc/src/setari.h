#ifndef SETARI_CONFIG_FILE
#define SETARI_CONFIG_FILE

#include <MFRC522.h>

#ifdef ARDUINO_AVR_ATmega328P
    #include "BlynkSimpleStream.h"
#endif

#ifdef ARDUINO_ESP8266_WEMOS_D1MINILITE
    #include <BlynkSimpleEsp8266.h>
    #include "secrete.h"
#endif


#define NFC_FW_VER "1.0"
#define NFC_HW_VER "1.0"
#define NFC_SHIELD "PN532"

#define BOARD_NAME "Authorizer Reader"
#define BOARD_VENDOR "Raspberry Pi"

#define APP_DEBUG

#ifdef APP_DEBUG
    constexpr bool debug = true;
#else
    constexpr bool debug = false;
#endif

// Blynk
constexpr char blynk_auth[] = "bE-vYAT-y3C3C7g8MrzPiaYTgeEsZ653";
constexpr unsigned int blynk_serial_speed = 9600;
constexpr unsigned long blynk_config_timeout = 10000L;
constexpr unsigned int zavor_config_timeout = 5000;
constexpr unsigned int autentificare_timeout = 3000;

#define CHN_SECURE_KEY           V0 // canal pentru receptie cheie sigura de la server Blynk
#define CHN_UPDATE_OR_NO_KEY     V1 // canal pentru receptie daca sa updatam cheia de pe card sau nu (1/0)
#define CHN_CONFIG_KEY_TO_UPDATE V2 // canal pentru receptie care cheie sa o punem pe card (1/2/3 = cheie fabrica, cheie repository, cheie sigura)
#define CHN_ZAVOR                V3 // canal pentru receptie deschiderea manuala a zavorului

#define CHN_CARD_UID   V10 // canal pentru transmitere UID card la server Blynk
#define CHN_VALID_CARD V11 // canal pentru transmitere daca cardul e valid sau nu la server Blynk (1/0)
#define CHN_AUTH       V12 // canal pentru transmiterea starii de autentificare la server Blynk (1/0)

// Dispozitiv
constexpr byte PIN_ZAVOR = 2; // pin unde releul pentru zavor este conectat

// NFC settings
constexpr byte BLOC_AUTENTIFICARE = 3; // acest bloc e folosit pentru verificarea autentificarii

enum ListaStariAuth {
    NOT_AUTH,
    DEFAULT_AUTH_KEY,
    SECURE_AUTH_KEY,
    NEW_AUTH_KEY,

    MAX_AUTH_CONFIG_VALUE // keep this entry the last one to use as "length" of enum
                            // for other defines
};

constexpr unsigned char RST_PIN = 9;
constexpr unsigned char SS_PIN = 10;

constexpr unsigned char debug_serial_rx = 3;
constexpr unsigned char debug_serial_tx = 4;
constexpr unsigned int  debug_speed     = 38400;

constexpr unsigned long system_timer_interval = 1000L;
constexpr unsigned long nfc_timer_interval = 500L;

constexpr MFRC522::MIFARE_Key nfc_default_key_a = {.keyByte = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}}; // All keys "A" are set to FFFFFFFFFFFFh at delivery from factory.
constexpr MFRC522::MIFARE_Key nfc_default_key_b = {.keyByte = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}}; // All keys "B" are set to FFFFFFFFFFFFh at delivery from factory.
//constexpr MFRC522::MIFARE_Key nfc_default_key_b = {.keyByte = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7}};

constexpr MFRC522::MIFARE_Key nfc_secure_key_a = {.keyByte = {0xCA, 0xFE, 0xDB, 0x91, 0x64, 0x37}}; // Private keys
//constexpr MFRC522::MIFARE_Key nfc_secure_key_a = {.keyByte = {0x37, 0x64, 0x91, 0xDB, 0xFE, 0xCA}}; // Private keys
constexpr MFRC522::MIFARE_Key nfc_secure_key_b = {.keyByte = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7}};

#endif
