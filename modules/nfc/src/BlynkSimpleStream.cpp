#include "BlynkSimpleStream.h"

#ifdef ARDUINO_AVR_ATmega328P
    BlynkTransportStream _blynkTransport;
    BlynkStream Blynk(_blynkTransport);
#endif
