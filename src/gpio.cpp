#include "gpio.h"

#ifdef RPI
#include <wiringPi.h>
#endif

constexpr const int kQuarterPulseLedGpioId = 3;

void Gpio::Init() {
#ifdef RPI
  wiringPiSetup();
  pinMode(kQuarterPulseLedGpioId, OUTPUT);
#endif
}

void Gpio::SetQuarterPulseLed(bool active) {
#ifdef RPI
  digitalWrite(kQuarterPulseLedGpioId, active);
#endif
}
