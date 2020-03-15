#ifndef SESSTINGS_CONFIG_FILE
#define SESSTINGS_CONFIG_FILE

#include <MFRC522.h>

#define NFC_FW_VER "1.0"
#define NFC_HW_VER "1.0"
#define NFC_SHIELD "PN532"

#define BOARD_NAME "Authorizer Reader"
#define BOARD_VENDOR "Raspberry Pi"

#define APP_DEBUG

//Blynk
constexpr char blynk_auth[] = "bE-vYAT-y3C3C7g8MrzPiaYTgeEsZ653";
constexpr unsigned int blynk_serial_speed = 9600;
constexpr unsigned long blynk_config_timeout = 5000L;

constexpr byte CHN_SECURE_KEY = V0; // channel for new key sent from Blynk to arduino
constexpr byte CHN_CARD_UID   = V1; // channel to send UID of detected card to Blynk
constexpr byte CHN_VALID_CARD = V2; // channel to send card validity to Blynk
constexpr byte CHN_AUTH       = V3; // channel to send authentication status based on auth enum

// NFC settings
enum AuthStatus {
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

constexpr unsigned long system_timer_inerval = 1000L;
constexpr unsigned long nfc_timer_inerval = 500L;

constexpr MFRC522::MIFARE_Key nfc_default_key_a = {.keyByte = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}}; // All keys "A" are set to FFFFFFFFFFFFh at delivery from factory.
constexpr MFRC522::MIFARE_Key nfc_default_key_b = {.keyByte = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}}; // All keys "B" are set to FFFFFFFFFFFFh at delivery from factory.
//constexpr MFRC522::MIFARE_Key nfc_default_key_b = {.keyByte = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7}};

constexpr MFRC522::MIFARE_Key nfc_secure_key_a = {.keyByte = {0xCA, 0xFE, 0xDB, 0x91, 0x64, 0x37}}; // Private keys
constexpr MFRC522::MIFARE_Key nfc_secure_key_b = {.keyByte = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7}};

#endif
