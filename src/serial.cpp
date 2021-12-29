#include "serial.h"

#include <chrono>
#include <thread>

#define SERIAL_PORT0 "/dev/ttyUSB0"
#define SERIAL_PORT1 "/dev/ttyUSB1"
#define SERIAL_PORT2 "/dev/ttyUSB2"

#define SERIAL_SPEED 115200

bool Serial::OpenSerialDevice(const char* device) {
  const bool success = serial.openDevice(device, SERIAL_SPEED) == 1;
  if (!success) {
    printf("Opening %s failed\n", device);
    serial.closeDevice();
  }
  return success;
}

void Serial::Init() {
  for (int c = 0; c < 10; ++c) {
    printf("Serial init, attempt: %d\n", c);

    if (!OpenSerialDevice(SERIAL_PORT0) && !OpenSerialDevice(SERIAL_PORT1) &&
        !OpenSerialDevice(SERIAL_PORT2)) {
      printf("Could not open serial device\n");
      return;
    }
    const char send_char = 'Y';
    for (int i = 0; i < 10; ++i) {
      serial.writeChar(send_char);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      char read_char;
      if (serial.available() && serial.readChar(&read_char, 0) == 1 &&
          read_char == send_char) {
        printf("Echo received, all good");
        return;
      }
    }
    serial.closeDevice();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  exit(-1);
}

void Serial::WaitForSerialConnection() { serial.writeChar('Y'); }

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
