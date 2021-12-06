#ifndef __MIDI_H_
#define __MIDI_H_

#include "portmidi.h"

class Midi {
 public:
  void Init();
  void Finalize();

  void SendStart();
  void SendClock();
  void SendStop();

 private:
  PmStream *midi_out_;
  int midi_out_id_ = -1;
};

#endif  // __MIDI_H_