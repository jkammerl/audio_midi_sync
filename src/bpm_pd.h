#ifndef __BPM_PD_
#define __BPM_PD_

#include "clock_osc.h"

class BpmPd {
 public:
  BpmPd();

  double GetPhase( double now);
  void UpdateTarget(double bmp, double now);

 private:
  ClockOsc clock_;
};

#endif  // __BPM_PD_