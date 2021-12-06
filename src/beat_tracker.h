#ifndef __BEAT_TRACKER_H
#define __BEAT_TRACKER_H

#include <array>
#include <cstddef>
#include <memory>
#include <mutex>

#include "BTrack.h"

class BeatTracker {
 public:
  BeatTracker();

  void Init();

  bool ProcessFloatAudio(const float* data, const size_t channels,
                         const size_t frames);

  BeatTracker(const BeatTracker&) = delete;

 private:
  double GetAudioSeconds() const;

  std::mutex mutex_;

  std::unique_ptr<BTrack> btrack_;
  std::unique_ptr<OnsetDetectionFunction> onset_detection_;
  std::vector<double> bt_frame_;

  double last_bpm_ = 0.0;
  uint64_t audio_time_smp_ = 0;
  bool init_ = false;
};

#endif