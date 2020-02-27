//! # MonoResampler
//! 
//! Trans codes the given audio signal from it's source sampling rate to the
//! sampling rate BAE_Audio runs at.

use super::*;

/// Type used for fractional indexing.
type IndexT = SampleT;

/// Struct tracking all of the data required for resampling, with some extra
/// features like playback speed and looping.
pub struct MonoResampler {
	data: TrackT,
	ind: IndexT,
	inc: SampleT,
	speed: MathT,
	loop_start: u64,
	loop_end: u64,
}

impl MonoResampler {
	/// Creates a new MonoResampler object.
	/// 
	/// # Parameters
	/// 
	/// * `data` - The track containing the original audio data to resample.
	/// * `source_sample_rate` - The sample rate the original data was recorded at.
	/// * `loop_start` - The start point of looping.
	/// * `loop_end` - The end point of looping. If this value is 0, no looping is assumed.
	/// 
	/// If `loop_end` is less than `loop_start`, they are swapped.
	pub fn new(data:TrackT, source_sample_rate: u64, mut loop_start: u64, mut loop_end: u64) -> Self {
		if loop_end < loop_start {
			std::mem::swap(&mut loop_start, &mut loop_end);
		}

		MonoResampler {
			data,
			ind: 0.0,
			inc: (source_sample_rate as MathT * INV_SAMPLE_RATE) as SampleT,
			speed: 1.0,
			loop_start,
			loop_end,
		}
	}

	/// Sets the playback speed.
	pub fn set_playback_speed(&mut self, speed: MathT) {
		self.speed = speed;
	}

	/// Returns the playback speed.
	pub fn get_playback_speed(&self) -> MathT {
		self.speed
	}

	/// Calculates and returns the next sample.
	pub fn process(&mut self) -> SampleT {
		if self.ind as usize >= self.data.len() && self.loop_end == 0 {
			return SampleT::default();
		}

		let frac: SampleT = self.ind.fract();

		let p1: SampleT = if self.ind.trunc() as usize + 1 >= self.data.len() && self.loop_end != 0 {
			self.data[(self.ind - (self.loop_end - self.loop_start) as IndexT) as usize]
		} else if self.ind.trunc() as usize + 1 >= self.data.len() {
			self.data[self.ind.trunc() as usize]
		} else {
			self.data[(self.ind + 1.0).trunc() as usize]
		};

		let x1: SampleT = self.data[self.ind.trunc() as usize];
		let x2: SampleT = p1;

		let y = x1 + frac as SampleT * (x2 - x1);

		self.ind += self.inc * self.speed as SampleT;

		if self.ind >= self.loop_end as IndexT && self.loop_end != 0 {
			self.ind -= (self.loop_end - self.loop_start) as IndexT;
		}

		y
	}
}

impl Clone for MonoResampler {
	fn clone(&self) -> Self {
		MonoResampler {
			data: self.data.clone(),
			ind: 0.0,
			inc: self.inc,
			speed: self.speed,
			loop_start: self.loop_start,
			loop_end: self.loop_end,
		}
	}
}
