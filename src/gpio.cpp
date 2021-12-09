#include "gpio.h"

#ifdef RPI
#include <wiringPi.h>
#endif

constexpr const int kQuarterPulseLedGpioId = 5;
constexpr const int kStartStopButtpnGpioId = 7;

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

bool Gpio::StartStopButtonPressed() {
#ifdef RPI
  static bool last_state = false;
  const bool state = !digitalWrite(kStartStopButtpnGpioId);
  const bool button_pressed = !last_state && state;
  last_state = state;
  return button_pressed;
#else
  return false;
#endif
}
