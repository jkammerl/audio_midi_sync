#include "serial.h"

#define SERIAL_PORT0 "/dev/ttyUSB0"
#define SERIAL_PORT1 "/dev/ttyUSB1"
#define SERIAL_PORT2 "/dev/ttyUSB2"

#define SERIAL_SPEED 115200

bool Serial::OpenSerialDevice(const char* device) {
  const bool success = serial.openDevice(device, SERIAL_SPEED) != 1;
  if (!success) {
    printf("Opening %s failed\n", device);
  }
  return success;
}

void Serial::Init() {
  if (!OpenSerialDevice(SERIAL_PORT0) && !OpenSerialDevice(SERIAL_PORT1) &&
      !OpenSerialDevice(SERIAL_PORT2)) {
    printf("Could not open serial device\n");
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
