use super::*;

pub struct Gain {
	a: SampleT,
}

impl Gain {
	pub fn new(a: SampleT) -> Gain {
		Gain{
			a
		}
	}
}

impl Modifier for Gain {
	fn process(&mut self, x: StereoData) -> StereoData {
		x * self.a
	}
}
