//! # Noise
//! 
//! A white noise generator.

use super::*;
use rand;

#[derive(Default,Clone)]
/// Struct for generating white noise audio samples.
pub struct Noise {
	engine: rand::rngs::ThreadRng,
}

impl Noise {
	/// Creates a new white noise generator.
	pub fn new() -> Self {
		Noise{
			engine: rand::thread_rng()
		}
	}
}

impl Generator for Noise {
	fn process(&mut self) -> StereoData {
		use rand::Rng;

		StereoData::from_mono(
			self.engine.gen::<SampleT>()*2.0-1.0
		)
	}
}
