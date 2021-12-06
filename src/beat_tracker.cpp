#include "beat_tracker.h"

#include <cstddef>
#include <mutex>

#include "logger.h"

const size_t kBtrackBufferSize = 512;
const size_t kStereoChannels = 2;

BeatTracker::BeatTracker()
    : bt_frame_(kBtrackBufferSize, 0), audio_time_smp_(0) {
  btrack_.reset(new BTrack(kBtrackBufferSize, 2 * kBtrackBufferSize));
  onset_detection_.reset(
      new OnsetDetectionFunction(kBtrackBufferSize, 2 * kBtrackBufferSize,
                                 ComplexSpectralDifferenceHWR, HanningWindow));
}

void BeatTracker::Init() {
  btrack_->setTempo(120);
  btrack_->doNotFixTempo();
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
  const double bpm = btrack_->getCurrentTempoEstimate();
  static double last_update = 0.0;
  const double now = GetAudioSeconds();
  if (bpm != last_bpm_ || now - last_update > 1.0) {
    printf("New BPM: %lf\n", bpm);
    last_bpm_ = bpm;
    last_update = now;
  }
  audio_time_smp_ += frames;
  return btrack_->beatDueInCurrentFrame();
}

double BeatTracker::GetAudioSeconds() const {
  return static_cast<double>(audio_time_smp_) / 44100.0;
}
