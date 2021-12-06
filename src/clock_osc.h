#ifndef __CLOCK_VCO_
#define __CLOCK_VCO_

#include <cmath> 

constexpr const double kLatencyCompensationMs = -200;

class ClockOsc {
    public:
    ClockOsc(const double now_sec):last_update_sec_(now_sec) {}

    void SetIncPerSecond(const double now_sec, double inc_per_sec) {
         UpdatePhase(now_sec);
         inc_per_sec_ = inc_per_sec;
    }

    void UpdatePhase(const double now_sec) {
        const double delta_sec = now_sec-last_update_sec_;
        last_update_sec_ = now_sec;
        phase_ += inc_per_sec_*delta_sec;
        phase_ = std::fmod(phase_, 1.0f);
    }

    double GetPhase() const {
        const double latency_phase = kLatencyCompensationMs/1000.0*inc_per_sec_;
        double latency_compensated_phase = phase_ + latency_phase;
        while (latency_compensated_phase<0) {
            latency_compensated_phase+= 1.0;
        }
        return std::fmod(latency_compensated_phase, 1.0f);
    }
    
private:
    double phase_ = 0.0;
    double inc_per_sec_= 0.0;
    double last_update_sec_ = 0.0;
};

#endif  // __CLOCK_VCO_