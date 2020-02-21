//! # Zero
//! 
//! A 0 generator.

use super::*;

/// Zero struct to allow for a Generator object that doesn't do anything.
#[derive(Default,Copy,Clone)]
pub struct Zero {
}

impl Zero {
	/// Returns new Zero object.
	pub fn new() -> Self {
		Zero{}
	}
}

impl Generator<Zero> for Zero {
	fn process(&mut self) -> StereoData {
		StereoData::default()
	}
}

#[cfg(test)]
impl Name for Zero {
	fn get_name(&self) -> &str {
		"Generator.Zero"
	}
}
