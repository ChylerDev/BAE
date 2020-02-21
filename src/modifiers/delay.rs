//! # Delay

use super::*;
use std::time::Duration;
use std::collections::VecDeque;

/// Delay modifier, delays a signal by a given amount of time rounded to the
/// nearest sample.
pub struct Delay {
	delay: VecDeque<StereoData>,
}

impl Delay {
	/// Creates a new Delay object from the given duration rounded to the
	/// nearest sample.
	pub fn new(d: Duration) -> Self {
		let mut v = VecDeque::new();

		for _ in 0..((d.as_secs_f64()*SAMPLE_RATE as MathT).round() as usize) {
			v.push_back(StereoData::default());
		}

		Delay {
			delay:v
		}
	}

	/// Returns the delay of the Modifier in a Duration value.
	pub fn get_delay(&self) -> Duration {
		Duration::from_secs_f64(self.delay.len() as MathT / SAMPLE_RATE as MathT)
	}
}

impl Modifier<Delay> for Delay {
	fn process(&mut self, x: StereoData) -> StereoData {
		self.delay.push_back(x);

		self.delay.pop_front().unwrap()
	}
}

#[cfg(test)]
impl Name for Delay {
	fn get_name(&self) -> &str {
		"Modifiers.Delay"
	}
}

impl Clone for Delay {
	fn clone(&self) -> Self {
		Delay {
			delay: {
				let mut v = VecDeque::new();

				for _ in 0..(self.delay.len() * SAMPLE_RATE as usize) {
					v.push_back(StereoData::default());
				}

				v
			}
		}
	}
}
