//! # Echo

use super::*;
use std::collections::VecDeque;
use std::time::Duration;

pub struct Echo {
	delay: VecDeque<StereoData>,
	gain: SampleT,
}

impl Echo {
	pub fn new(d: Duration, g: MathT) -> Self{
		let mut v = VecDeque::new();
		for _ in 0..((d.as_secs_f64()*SAMPLE_RATE as MathT) as usize) {
			v.push_back(StereoData::default());
		}

		Echo {
			delay: v,
			gain: g as SampleT,
		}
	}
}

impl Modifier for Echo {
	fn process(&mut self, x: StereoData) -> StereoData {
		let wet = self.delay.pop_front().unwrap() * self.gain + x;
		self.delay.push_back(wet);

		wet
	}
}

impl Name for Echo {
	fn get_name(&self) -> &str {
		"Modifiers.Echo"
	}
}

impl Clone for Echo {
	fn clone(&self) -> Self {
		Echo {
			delay: {
				let mut v = VecDeque::new();

				for _ in 0..(self.delay.len() * SAMPLE_RATE as usize) {
					v.push_back(StereoData::default());
				}

				v
			},
			gain: self.gain,
		}
	}
}
