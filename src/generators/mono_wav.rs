//! # WAV
//! 
//! A wave data player.

use super::*;
use crate::tools::mono_resampler::*;

/// Struct for playing wave files.
#[derive(Clone)]
pub struct MonoWav {
	resam: MonoResampler,
}

impl MonoWav {
	/// Constructs from a given path.
	pub fn from_file(s: &str) -> Self {
		let (h, t) = tools::read_wav(std::path::Path::new(s))
		.expect("File could not be read");

		MonoWav::from_track(h.sampling_rate as u64, t.first().expect("No audio data read").clone())
	}

	/// Converts from the given track and source sample rate.
	pub fn from_track(sampling_rate:u64, t:TrackT) -> Self {
		MonoWav{
			resam: MonoResampler::new(t, sampling_rate, 0, 0)
		}
	}

	/// Borrows the resampler object.
	pub fn get(&self) -> &MonoResampler {
		&self.resam
	}

	/// Mutably borrows the resampler object.
	pub fn get_mut(&mut self) -> &mut MonoResampler {
		&mut self.resam
	}
}

impl Generator for MonoWav {
	fn process(&mut self) -> SampleT {
		self.resam.process()
	}
}
