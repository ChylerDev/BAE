//! # Resampler
//! 
//! Trans codes the given audio signal from it's source sampling rate to the
//! sampling rate OCAE runs at.

use super::*;

/// Type used for fractional indexing.
type IndexT = SampleT;

/// Struct tracking all of the data required for resampling, with some extra
/// features like playback speed and looping.
pub struct Resampler {
	data: TrackT,
	ind: IndexT,
	inc: SampleT,
	speed: MathT,
	loop_start: u64,
	loop_end: u64,
}

impl Resampler {
	/// Creates a new Resampler object.
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

		Resampler {
			data,
			ind: 0.0,
			inc: source_sample_rate as SampleT * INV_SAMPLE_RATE as SampleT,
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
	pub fn process(&mut self) -> StereoData {
		if self.ind as usize >= self.data.len() && self.loop_end == 0 {
			return StereoData::default();
		}

		let frac: SampleT = self.ind.fract();

		let p1: StereoData = if self.ind.trunc() as usize + 1 >= self.data.len() && self.loop_end != 0 {
			self.data[(self.ind - (self.loop_end - self.loop_start) as IndexT) as usize]
		} else if self.ind.trunc() as usize + 1 >= self.data.len() {
			self.data[self.ind.trunc() as usize]
		} else {
			self.data[(self.ind + 1.0).trunc() as usize]
		};

		let l_x1: SampleT = self.data[self.ind.trunc() as usize].left;
		let l_x2: SampleT = p1.left;
		let r_x1: SampleT = self.data[self.ind.trunc() as usize].right;
		let r_x2: SampleT = p1.right;

		let l:SampleT = l_x1 + frac as SampleT * (l_x2 - l_x1);
		let r:SampleT = r_x1 + frac as SampleT * (r_x2 - r_x1);

		let y = StereoData::from_stereo(l, r);

		self.ind += self.inc * self.speed as SampleT;

		if self.ind >= self.loop_end as IndexT && self.loop_end != 0 {
			self.ind -= (self.loop_end - self.loop_start) as IndexT;
		}

		y
	}
}
