#ifndef __SERIAL_H_
#define __SERIAL_H_

#include "serialib.h"

class Serial {
 public:
  void Init();

  void SendQuarterPulse();
  char ReceiveCommand();

 private:
  void WaitForSerialConnection();
  bool OpenSerialDevice(const char* device);
  serialib serial;
};

#endif  // __SERIAL_H_
