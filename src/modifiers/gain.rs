//! # Gain

use super::*;

/// Simple gain filter, the simplest filter of all.
#[derive(Default,Copy,Clone)]
pub struct Gain {
	/// The gain of the gain filter.
	pub a: SampleT,
}

impl Gain {
	/// Constructs a new gain filter from the given...... gain....
	pub fn new(a: SampleT) -> Gain {
		Gain{
			a
		}
	}
}

impl Modifier for Gain {
	fn process(&mut self, x: SampleT) -> SampleT {
		x * self.a
	}
}
