#ifndef NFC_CONFIG_FILE
#define NFC_CONFIG_FILE

#define COMMON_ANODE  // LEDs have their anode connected to VCC (+5V or +3V3)
                      // through an appropriate sized resistor and the
                      // microcontroller is lighting them by switch their
                      // cathode to GND (0V == - terminal)

#ifdef COMMON_ANODE  // For common anode LED is ON if controlling pin is low
#define LED_ON LOW
#define LED_OFF HIGH
#else
#define LED_ON HIGH
#define LED_OFF LOW
#endif

#define RELAY_ACTIVE_LOW  // Relay activated (door locked) when controlling pin
                          // is low

#ifdef RELAY_ACTIVE_LOW
#define RELAY_ON LOW
#define RELAY_OFF HIGH
#else
#define RELAY_ON HIGH
#define RELAY_OFF LOW
#endif

// Create MFRC522 instance.
class NFC {
  public:
   void NFC();

  private:
   MFRC522 mfrc522(SS_PIN, RST_PIN);

   bool programMode = false;  // initialize programming mode to false

   uint8_t successRead;  // Variable integer to keep if we have Successful Read

   byte storedCard[4];  // Stores an ID read from EEPROM
   byte readCard[4];    // Stores scanned ID read from RFID Module
   byte masterCard[4];  // Stores master card's ID read from EEPROM
                        // from Reader

   constexpr uint8_t RST_PIN = 9;  // Configurable, see typical pin layout above
   constexpr uint8_t SS_PIN = 10;  // Configurable, see typical pin layout above

   constexpr uint8_t interrupt_pin = 8;
   constexpr uint8_t erase_button_pin = 4;
};

NFC Nfc;

#endif  // NFC_CONFIG_FILE
