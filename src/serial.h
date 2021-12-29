#ifndef __SERIAL_H_
#define __SERIAL_H_

#include "serialib.h"

class Serial {
 public:
  void Init();

  void SendQuarterPulse();
  char ReceiveCommand();

 private:
  serialib serial;
};

#endif  // __SERIAL_H_
