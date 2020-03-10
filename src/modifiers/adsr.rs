//! # ADSR

use super::*;
use std::time::Duration;

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
	a:SampleT,
	d:SampleT,
	s:SampleT,
	r:SampleT,
	state:ADSRState,
	g:SampleT,
}

impl ADSR {
	/// Constructs an ADSR filter object.
	/// 
	/// # Parameters
	/// 
	/// * `a` - Attack time.
	/// * `d` - Decay time.
	/// * `s` - Sustain level in decibels. Value is clamped to be less than 0.
	/// * `r` - Release time.
	pub fn new(a:Duration,d:Duration,s:MathT,r:Duration) -> Self {
		let s = s.min(0.0);
		ADSR {
			a: 1.0/(a.as_secs_f64() * SAMPLE_RATE as MathT) as SampleT,
			d: ((db_linear(s)-1.0)/(d.as_secs_f64() * SAMPLE_RATE as MathT)) as SampleT,
			s: db_linear(s) as SampleT,
			r: ((-db_linear(s))/(r.as_secs_f64() * SAMPLE_RATE as MathT)) as SampleT,
			state: ADSRState::Attack,
			g: 0.0
		}
	}

	/// Changes adsr state to release
	pub fn release(&mut self) {
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
			},
			ADSRState::Decay => {
				self.g += self.d;
				if self.g <= self.s {
					self.state = ADSRState::Sustain;
					self.g = self.s;
				}
			},
			ADSRState::Sustain => {
			},
			ADSRState::Release => {
				self.g += self.r;
				if self.g <= 0.0 {
					self.state = ADSRState::Stopped;
					self.g = 0.0;
				}
			},
			ADSRState::Stopped => return SampleT::default(),
		}

		x * self.g
	}
}

#[cfg(test)]
impl Name for ADSR {
	fn get_name(&self) -> &str {
		"Modifiers.ADSR"
	}
}

impl Clone for ADSR {
	fn clone(&self) -> Self {
		ADSR {
			a: self.a,
			d: self.d,
			s: self.s,
			r: self.r,
			state: ADSRState::Attack,
			g: 0.0
		}
	}
}
