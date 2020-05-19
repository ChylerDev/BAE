//! # ADSR

use super::*;
use std::time::Duration;
use crate::utils::*;

/// Enum of the states an ADSR filter exists in.
pub enum ADSRState {
    /// Attack state.
    Attack,
    /// Decay state.
    Decay,
    /// Sustain state.
    Sustain,
    /// Release state.
    Release,
    /// State for when the ADSR has completed it's processing.
    Stopped,
}

/// Attack-decay-sustain-release filter.
/// 
/// Creates a simple envelope for the given signal.
pub struct ADSR {
    a: MathT,
    d: MathT,
    s: MathT,
    r: MathT,

    sample_rate: MathT,

    state: ADSRState,
    g: MathT,
}

impl ADSR {
    /// Constructs an ADSR filter object.
    /// 
    /// # Parameters
    /// 
    /// * `a` - Attack time.
    /// * `d` - Decay time.
    /// * `s` - Sustain level in dBFS. Value is clamped to be less than 0.
    /// * `r` - Release time.
    /// * `sample_rate` - Sample rate of the engine.
    pub fn new(a: Duration, d: Duration, s: MathT, r: Duration, sample_rate: MathT) -> Self {
        let s = s.min(0.0);
        ADSR {
            a: 1.0/(a.as_secs_f64() * sample_rate),
            d: ((db_to_linear(s)-1.0)/(d.as_secs_f64() * sample_rate)),
            s: db_to_linear(s),
            r: ((-db_to_linear(s))/(r.as_secs_f64() * sample_rate)),
            sample_rate,
            state: ADSRState::Attack,
            g: Default::default(),
        }
    }

    /// Sets the attack time.
    pub fn attack(&mut self, a: Duration) {
        self.a = 1.0/(a.as_secs_f64() * self.sample_rate);
    }

    /// Sets the decay time.
    pub fn decay(&mut self, d: Duration) {
        self.d = (self.s as MathT - 1.0)/(d.as_secs_f64() * self.sample_rate);
    }

    /// Sets the sustain level in dBFS.
    pub fn sustain(&mut self, s: MathT) {
        self.d *= (db_to_linear(s) - 1.0)/(self.s as MathT - 1.0);
        self.r *= db_to_linear(s)/self.s as MathT;
        self.s = db_to_linear(s);
    }

    /// Sets the release time.
    pub fn release(&mut self, r: Duration) {
        self.r = -self.s as MathT / (r.as_secs_f64() * self.sample_rate);
    }

    /// Changes state to release
    pub fn trigger_release(&mut self) {
        self.state = ADSRState::Release;
    }
}

impl Modifier for ADSR {
    fn process(&mut self, x: SampleT) -> SampleT {
        match self.state {
            ADSRState::Attack => {
                self.g += self.a;
                if self.g >= 1.0 {
                    self.state = ADSRState::Decay;
                    self.g = 1.0;
                }

                x * self.g as SampleT
            }
            ADSRState::Decay => {
                self.g += self.d;
                if self.g <= self.s {
                    self.state = ADSRState::Sustain;
                    self.g = self.s;
                }

                x * self.g as SampleT
            }
            ADSRState::Sustain => {
                x * self.g as SampleT
            }
            ADSRState::Release => {
                self.g += self.r;
                if self.g <= 0.0 {
                    self.state = ADSRState::Stopped;
                    self.g = 0.0;
                }

                x * self.g as SampleT
            }
            ADSRState::Stopped => SampleT::default()
        }
    }
}

impl Clone for ADSR {
    fn clone(&self) -> Self {
        ADSR {
            a: self.a,
            d: self.d,
            s: self.s,
            r: self.r,

            sample_rate: self.sample_rate,

            state: ADSRState::Attack,
            g: Default::default(),
        }
    }
}
