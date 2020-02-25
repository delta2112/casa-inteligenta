#define COMMON_ANODE

#ifdef COMMON_ANODE
#define LED_ON LOW
#define LED_OFF HIGH
#else
#define LED_ON HIGH
#define LED_OFF LOW
#endif

bool programMode = false;  // initialize programming mode to false

uint8_t successRead;    // Variable integer to keep if we have Successful Read from Reader

byte storedCard[4];   // Stores an ID read from EEPROM
byte readCard[4];   // Stores scanned ID read from RFID Module
byte masterCard[4];   // Stores master card's ID read from EEPROM

// Create MFRC522 instance.
constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);





class NFC {
  public:
    void NFC();
  private:
    int id;

    constexpr uint8_t red_led = 7;
    constexpr uint8_t green_led = 6;
    constexpr uint8_t blue_led = 5;
    
    constexpr uint8_t relay = 4;     // Set Relay Pin
    constexpr uint8_t wipe_button = 3;     // Button pin for WipeMode
};

NFC Nfc;
