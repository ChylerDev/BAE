//! # Empty
//! 
//! A 0 generator.

use super::*;

/// Empty struct to allow for a Generator object that doesn't do anything.
#[derive(Default,Copy,Clone)]
pub struct Empty {
}

impl Empty {
	/// Returns new Empty object.
	pub fn new() -> Self {
		Empty{}
	}
}

impl Generator<Empty> for Empty {
	fn process(&mut self) -> StereoData {
		StereoData::default()
	}
}


impl Name for Empty {
	fn get_name(&self) -> &str {
		"Generator.Empty"
	}
}
