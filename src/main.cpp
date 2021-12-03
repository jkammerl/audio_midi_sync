#include "audio_io.h"
#include "portaudio.h"

void PaDeviceScan() {
  PaDeviceIndex pa_count = Pa_GetDeviceCount();
  printf("Audio devices found: %d\n", pa_count);
  printf("Default input device: %d\n", Pa_GetDefaultInputDevice());
  printf("Default output device: %d\n", Pa_GetDefaultOutputDevice());

  for (int i = 0; i < pa_count; ++i) {
    const PaDeviceInfo *device_info = Pa_GetDeviceInfo(i);
    printf("Id: %d, name: %s, in: %d, out: %d\n", i, device_info->name,
           device_info->maxInputChannels, device_info->maxOutputChannels);
  }
}

/*******************************************************************/
int main(void) {
  PaError err;
  AudioIo audioIo;

  err = Pa_Initialize();
  if (err != paNoError) goto error;

  PaDeviceScan();

  if (audioIo.Open(Pa_GetDefaultInputDevice(), Pa_GetDefaultOutputDevice())) {
    if (audioIo.Start()) {
      while (1) {
        // AudioIo.update();
        Pa_Sleep(100);
      }

      audioIo.Stop();
    }

    audioIo.Close();
  }

  Pa_Terminate();
  printf("Test finished.\n");

  return err;

error:
  Pa_Terminate();
  fprintf(stderr, "An error occured while using the portaudio stream\n");
  fprintf(stderr, "Error number: %d\n", err);
  fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
  return err;
}
