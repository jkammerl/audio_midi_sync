#ifndef __AUDIO_IO_
#define __AUDIO_IO_

#include <memory>

#include "beat_tracker.h"
#include "portaudio.h"

class AudioIo {
 public:
  AudioIo();

  bool Open(PaDeviceIndex input_index, PaDeviceIndex output_index);
  bool Close() ;
  bool Start();
  bool Stop() ;

  AudioIo(AudioIo&) = delete;

 private:
  int PaCallbackMethod(const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags) ;

   
  static int PaCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags, void *userData);

  void PaStreamFinishedMethod();

  static void PaStreamFinished(void *userData);

  std::unique_ptr<BeatTracker> beat_tracker_;
  PaStream *stream;
};

#endif  // __AUDIO_IO_