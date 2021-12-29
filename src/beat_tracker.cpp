#include "beat_tracker.h"

#include <cstddef>
#include <mutex>

#include "bpm_pd.h"
#include "logger.h"

const size_t kBtrackBufferSize = 512;
const size_t kStereoChannels = 2;

BeatTracker::BeatTracker()
    : bt_frame_(kBtrackBufferSize, 0), audio_time_smp_(0) {}

void BeatTracker::Init() {
  btrack_.reset(new BTrack(kBtrackBufferSize, 2 * kBtrackBufferSize));
  onset_detection_.reset(
      new OnsetDetectionFunction(kBtrackBufferSize, 2 * kBtrackBufferSize,
                                 ComplexSpectralDifferenceHWR, HanningWindow));
  btrack_->setTempo(120);
  btrack_->doNotFixTempo();
  serial_.Init();
  init_ = true;
}

bool BeatTracker::ProcessFloatAudio(const float* data, const size_t channels,
                                    const size_t frames) {
  std::lock_guard<std::mutex> lck(mutex_);
  if (!init_) return false;

  CHECK_EQ(channels, kStereoChannels);
  CHECK_EQ(frames, kBtrackBufferSize);
  for (size_t i = 0; i < frames; ++i) {
    const float left = *data++;
    const float right = *data++;
    bt_frame_[i] = (left + right) / 2.0;
  }
  // calculate the onset detection function sample for the frame
  const double onset_sample =
      onset_detection_->calculateOnsetDetectionFunctionSample(bt_frame_);
  btrack_->processOnsetDetectionFunctionSample(onset_sample);
  static double last_update = 0.0;
  const double now = GetAudioSeconds();

  const bool current_beat = btrack_->beatDueInCurrentFrame();
  const double bpm = btrack_->getCurrentTempoEstimate();
  if (current_beat) {
    serial_.SendQuarterPulse();
    bpm_pd_.UpdateTarget(bpm, now);
  }

  const double phase = bpm_pd_.GetPhase(now);

  if (bpm != last_bpm_ || now - last_update > 1.0) {
    printf("New BPM: %lf\n", bpm);
    last_bpm_ = bpm;
    last_update = now;
  }
  audio_time_smp_ += frames;

  const char serial_command = serial_.ReceiveCommand();
  if (serial_command != 0) {
    if (serial_command == 'V') {
      printf("Variable tempo detection: %lf\n", bpm);
      //btrack_->doNotFixTempo();
    } else if (serial_command == 'F') {
      printf("Fixed tempo detection: %lf\n", bpm);
      //btrack_->fixTempo(bpm);
    }
  }

  return ProcessMidi(phase);
}

double BeatTracker::GetAudioSeconds() const {
  return static_cast<double>(audio_time_smp_) / 44100.0;
}

bool BeatTracker::ProcessMidi(double phase) {
  const int current_midi_clock_count = phase * 24.0 + 1;

  const bool phase_click = phase < last_phase_;

  if (phase_click) {
    while (midi_clock_counter_ < 24) {
      midi_.SendClock();
      ++midi_clock_counter_;
    }
    midi_clock_counter_ = 0;
  }

  while (midi_clock_counter_ < current_midi_clock_count) {
    midi_.SendClock();
    ++midi_clock_counter_;
  }
  last_phase_ = phase;
  return phase_click;
}