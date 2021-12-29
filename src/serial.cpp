#include "serial.h"

#define SERIAL_PORT "/dev/tty.wchusbserial1413240"

void Serial::Init() {
  if (serial.openDevice(SERIAL_PORT, 115200) != 1) {
    printf("Opening %s failed\n", SERIAL_PORT);
    return;
  }
}

void Serial::SendQuarterPulse() { 
    serial.writeChar('X'); 
    printf("X\n");
}

char Serial::ReceiveCommand() {
  char command;
  if (serial.available() && serial.readChar(&command, 0) == 1) {
    return command;
  }
  return 0;
}
