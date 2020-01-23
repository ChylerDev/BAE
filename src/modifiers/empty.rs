use super::*;

#[derive(Copy,Clone,Default,Debug)]
pub struct Empty{}

impl Empty {
	pub fn new() -> Empty {
		Empty {
		}
	}
}

impl Modifier for Empty {
	fn process(&mut self, x: StereoData) -> StereoData {
		x
	}
}
