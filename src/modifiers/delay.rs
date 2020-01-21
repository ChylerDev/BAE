use super::*;
use std::time::Duration;
use std::collections::VecDeque;

pub struct Delay {
	delay: VecDeque<StereoData>,
}

impl Delay {
	pub fn new(d: Duration) -> Self {
		let mut v = VecDeque::new();

		for _ in 0..((d.as_secs_f64()*SAMPLE_RATE as MathT) as usize) {
			v.push_back(StereoData::default());
		}

		Delay {
			delay:v
		}
	}

	pub fn get_delay(&self) -> Duration {
		Duration::from_secs_f64(self.delay.len() as MathT / SAMPLE_RATE as MathT)
	}
}

impl Modifier for Delay {
	fn process(&mut self, x: StereoData) -> StereoData {
		self.delay.push_back(x);

		self.delay.pop_front().unwrap()
	}
}


impl Name for Delay {
	fn get_name(&self) -> &str {
		"Modifiers.Delay"
	}
}
