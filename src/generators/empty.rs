//! # Empty
//! 
//! A 0 generator.

use super::*;

#[derive(Default,Copy,Clone)]
pub struct Empty {
}

impl Empty {
	pub fn new() -> Self {
		Empty{}
	}
}

impl Generator for Empty {
	fn process(&mut self) -> StereoData {
		StereoData::default()
	}
}


impl Name for Empty {
	fn get_name(&self) -> &str {
		"Generator.Empty"
	}
}
