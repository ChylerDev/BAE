use super::*;

type IndexT = SampleT;

pub struct Resampler {
	data: TrackT,
	ind: IndexT,
	inc: SampleT,
	speed: MathT,
	loop_start: u64,
	loop_end: u64,
}

impl Resampler {
	pub fn new(data:TrackT, source_sample_rate: u64, loop_start: u64, loop_end: u64) -> Self {
		Resampler {
			data,
			ind: 0.0,
			inc: source_sample_rate as SampleT * INV_SAMPLE_RATE as SampleT,
			speed: 1.0,
			loop_start,
			loop_end,
		}
	}

	pub fn set_playback_speed(&mut self, speed: MathT) {
		self.speed = speed;
	}

	pub fn get_playback_speed(&self) -> MathT {
		self.speed
	}

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

		let y = StereoData::from(l, r);

		self.ind += self.inc * self.speed as SampleT;

		if self.ind >= self.loop_end as IndexT && self.loop_end != 0 {
			self.ind -= (self.loop_end - self.loop_start) as IndexT;
		}

		y
	}
}
