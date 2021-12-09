#include "midi.h"

#include <stdio.h>

#include "portmidi.h"
#include "porttime.h"

#define OUT_QUEUE_SIZE 1024
#define TIME_PROC ((int32_t(*)(void*))Pt_Time)

#define MIDI_TIME_CLOCK 0xf8
#define MIDI_START 0xfa
#define MIDI_CONTINUE 0xfb
#define MIDI_STOP 0xfc

void Midi::Init() {
  PmError err;
  err = Pm_Initialize();
  if (err != pmNoError) {
    printf("Midi Error: %s\n", Pm_GetErrorText(err));
    return;
  };

  for (int i = 0; i < Pm_CountDevices(); i++) {
    const PmDeviceInfo* info = Pm_GetDeviceInfo(i);
    if ((info->input) || (info->output)) {
      printf("%d: %s, %s", i, info->interf, info->name);
      if (info->input) {
      }
      printf(" (input)");
    }

    if (info->output) {
      printf(" (output)");
    }
    printf("\n");
  }

  midi_out_id_ = 2; //Pm_GetDefaultOutputDeviceID();
  const PmDeviceInfo* midi_out_info = Pm_GetDeviceInfo(midi_out_id_);
  if (midi_out_info == NULL) {
    printf("Could not open default output device (%d).", midi_out_id_);
    return;
  }
  printf("Opening output device %s %s\n", midi_out_info->interf,
         midi_out_info->name);

  /* use zero latency because we want output to be immediate */
  err = Pm_OpenOutput(&midi_out_, midi_out_id_, NULL /* driver info */,
                      OUT_QUEUE_SIZE, TIME_PROC, NULL /* time info */,
                      0 /* Latency */);
  if (err) {
    printf("Midi Error: %s\n", Pm_GetErrorText(err));
    return;
  }
}

void Midi::SendStart() { Pm_WriteShort(midi_out_, 0, MIDI_START); }
void Midi::SendClock() { Pm_WriteShort(midi_out_, 0, MIDI_TIME_CLOCK); }
void Midi::SendStop() { Pm_WriteShort(midi_out_, 0, MIDI_STOP); }

void Midi::Finalize() {
  Pt_Stop(); /* stop the timer */
  Pm_Close(midi_out_);

  Pm_Terminate();
}