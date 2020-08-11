#include "BlynkSimpleEsp8266.h"

#ifdef ARDUINO_ESP8266_WEMOS_D1MINILITE
    static WiFiClient _blynkWifiClient;
    static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
    BlynkWifi Blynk(_blynkTransport);
#endif
