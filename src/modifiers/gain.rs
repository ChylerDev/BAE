//! # Gain

use super::*;

#[derive(Default,Copy,Clone)]
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

impl Modifier<Gain> for Gain {
	fn process(&mut self, x: StereoData) -> StereoData {
		x * self.a
	}
}

impl Name for Gain {
	fn get_name(&self) -> &str {
		"Modifiers.Gain"
	}
}
