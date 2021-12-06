#include "bpm_pd.h"

#include "clock_osc.h"

constexpr const double kTargetAdjustment = 1.0;

BpmPd::BpmPd() : clock_(0.0f) {}

double BpmPd::GetPhase(double now) {
  clock_.UpdatePhase(now);
  return clock_.GetPhase();
}
void BpmPd::UpdateTarget(double bmp, double now) {
  double beats_per_second = bmp / 60.0f;

  const double phase = clock_.GetPhase();
  double phase_correction;
  if (phase < 0.5f) {
    phase_correction = -phase;
  } else {
    phase_correction = 1.0f - phase;
  }
  phase_correction /= kTargetAdjustment;
  beats_per_second += phase_correction;
  clock_.SetIncPerSecond(now, beats_per_second);
}
