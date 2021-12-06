#include "audio_io.h"

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (512)

AudioIo::AudioIo() : stream(0) {
  printf("PortAudio Test: output AudioIo wave. SR = %d, BufSize = %d\n",
         SAMPLE_RATE, FRAMES_PER_BUFFER);
  beat_tracker_.reset(new BeatTracker());
  beat_tracker_->Init();
}

bool AudioIo::Open(PaDeviceIndex input_index, PaDeviceIndex output_index) {
  PaStreamParameters inputParameters;
  PaStreamParameters outputParameters;

  inputParameters.device = input_index;
  if (inputParameters.device == paNoDevice) {
    return false;
  }
  inputParameters.channelCount = 2;         /* stereo output */
  inputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
  inputParameters.suggestedLatency =
      Pa_GetDeviceInfo(inputParameters.device)->defaultLowOutputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  outputParameters.device = output_index;
  if (outputParameters.device == paNoDevice) {
    return false;
  }
  outputParameters.channelCount = 2;         /* stereo output */
  outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
  outputParameters.suggestedLatency =
      Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  PaError err = Pa_OpenStream(
      &stream, &inputParameters, &outputParameters, SAMPLE_RATE,
      FRAMES_PER_BUFFER, paClipOff, /* we won't output out of range samples so
                                       don't bother clipping them */
      &AudioIo::PaCallback, this    /* Using 'this' for userData so we can cast
                                       to AudioIo* in paCallback method */
  );

  if (err != paNoError) {
    /* Failed to open stream to device !!! */
    return false;
  }

  err = Pa_SetStreamFinishedCallback(stream, &AudioIo::PaStreamFinished);
  if (err != paNoError) {
    Pa_CloseStream(stream);
    stream = 0;

    return false;
  }

  return true;
}

bool AudioIo::Close() {
  if (stream == 0) return false;
  PaError err = Pa_CloseStream(stream);
  stream = 0;

  return (err == paNoError);
}

bool AudioIo::Start() {
  if (stream == 0) return false;
  PaError err = Pa_StartStream(stream);
  return (err == paNoError);
}

bool AudioIo::Stop() {
  if (stream == 0) return false;
  PaError err = Pa_StopStream(stream);
  return (err == paNoError);
}

/* The instance callback, where we have access to every method/variable in
 * object of class AudioIo */
int AudioIo::PaCallbackMethod(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo *timeInfo,
                              PaStreamCallbackFlags statusFlags) {
  std::lock_guard<std::mutex> lck (mutex_);
  (void)statusFlags;

  const bool current_beat = beat_tracker_->ProcessFloatAudio(
      (float *)inputBuffer, /*num_channels=*/2, framesPerBuffer);

  float *in = (float *)inputBuffer;
  float *out = (float *)outputBuffer;

  unsigned long i;
  if (!current_beat) {
    for (i = 0; i < framesPerBuffer; i++) {
      *out++ = *in++;
      *out++ = *in++;
    }
  } else {
    for (i = 0; i < framesPerBuffer; i++) {
      const float rand_float =
          static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f;
      *out++ = rand_float;  
      *out++ = rand_float;   
    }
  }
  return paContinue;
}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
int AudioIo::PaCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags, void *userData) {
  /* Here we cast userData to AudioIo* type so we can call the instance method
     paCallbackMethod, we can do that since we called Pa_OpenStream with
     'this' for userData */
  return ((AudioIo *)userData)
      ->PaCallbackMethod(inputBuffer, outputBuffer, framesPerBuffer, timeInfo,
                         statusFlags);
}

void AudioIo::PaStreamFinishedMethod() {
}

/*
 * This routine is called by portaudio when playback is done.
 */
void AudioIo::PaStreamFinished(void *userData) {
  return ((AudioIo *)userData)->PaStreamFinishedMethod();
}
