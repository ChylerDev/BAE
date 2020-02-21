//! # Sawtooth
//! 
//! A sawtooth sample generator.

use super::*;
use super::FreqMod;
use super::Generator;

/// Struct for generating sawtooth samples.
pub struct Sawtooth {
	irate: MathT,
	inc: SampleT,
}

impl FreqMod for Sawtooth {
	fn new(f: MathT) -> Self {
		Sawtooth {
			irate: 2.0*f*INV_SAMPLE_RATE,
			inc: 0.0,
		}
	}

	fn set_frequency(&mut self, f: MathT) {
		self.irate = 2.0*f*INV_SAMPLE_RATE;
	}

	fn get_frequency(&self) -> MathT {
		self.irate / (2.0 * INV_SAMPLE_RATE)
	}
}

impl Generator<Sawtooth> for Sawtooth {
	fn process(&mut self) -> StereoData {
		let y = self.inc;

		self.inc += self.irate as SampleT;

		if self.inc >= 1.0 {
			self.inc -= 2.0;
		}

		StereoData::from_mono(y)
	}
}

impl Clone for Sawtooth {
	fn clone(&self) -> Self {
		Sawtooth {
			irate: self.irate,
			inc: 0.0
		}
	}
}

#[cfg(test)]
impl Name for Sawtooth {
	fn get_name(&self) -> &str {
		"Generator.Sawtooth"
	}
}
