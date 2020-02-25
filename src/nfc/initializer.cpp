#include "initializer.h"

void Initializer::begin() {
  DEBUG_PRINT("");
  DEBUG_PRINT("Hardware v" + String(NFC_FW_VER));
  DEBUG_PRINT("Firmware v" + String(NFC_HW_VER));
  DEBUG_PRINT("NFC Shield" + String(NFC_SHIELD));

  indicator_init();
  button_init();
  config_init();

  if (configStore.flagConfig) {
    BlynkState::set(MODE_CONNECTING_NET);
  } else {
    BlynkState::set(MODE_WAIT_CONFIG);
  }
}
void Initializer::run() {
  switch (DeviceState::get()) {
  case WAIT_CONFIG:
  case CONFIGURING:  enterConfigMode();    break;
  case RUNNING:      Blynk.run();          break;
  case RESET_CONFIG: enterResetConfig();   break;
  default:           enterError();         break;
  }
}
