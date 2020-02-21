//! # Passthrough

use super::*;

#[derive(Copy,Clone,Default,Debug)]
pub struct Passthrough{}

impl Passthrough {
	pub fn new() -> Passthrough {
		Passthrough {
		}
	}
}

impl Modifier<Passthrough> for Passthrough {
	fn process(&mut self, x: StereoData) -> StereoData {
		x
	}
}
