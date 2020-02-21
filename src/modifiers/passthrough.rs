//! # Passthrough

use super::*;

/// Passthrough filter, does nothing to the input.
#[derive(Copy,Clone,Default,Debug)]
pub struct Passthrough{}

impl Passthrough {
	/// Creates new Passthrough object.
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
