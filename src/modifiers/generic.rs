//! # Generic

use super::*;
use std::collections::VecDeque;

pub type Zeros = VecDeque<(usize,MathT)>;
pub type Poles = VecDeque<(usize,MathT)>;
pub type Samples = VecDeque<StereoData>;

pub struct Generic {
	zeros:Zeros,
	poles:Poles,
	inputs:Samples,
	outputs:Samples,
}

impl Generic {
	pub fn new(zeros: Zeros, poles: Poles) -> Generic {
		let z_back = if let Some(z) = zeros.back() {
			z.0 + 1
		} else {
			0
		};
		let p_back = if let Some(p) = poles.back() {
			p.0 + 1
		} else {
			0
		};
		Generic {
			zeros,
			poles,
			inputs: {
				let mut v = Samples::new();
				for _ in 0..z_back {
					v.push_back(StereoData::default());
				}
				v
			},
			outputs: {
				let mut v = Samples::new();
				for _ in 0..p_back {
					v.push_back(StereoData::default());
				}
				v
			}
		}
	}
}

impl Modifier<Generic> for Generic {
	fn process(&mut self, x: StereoData) -> StereoData {
		let mut y = StereoData::default();

		self.inputs.push_front(x);
		self.inputs.pop_back();

		for z in &self.zeros {
			y += self.inputs[z.0] * z.1;
		}
		for p in &self.poles {
			y += self.outputs[p.0] * p.1;
		}

		self.outputs.push_front(y);
		self.outputs.pop_back();

		y
	}
}

impl Name for Generic {
	fn get_name(&self) -> &str {
		"Modifiers.Generic"
	}
}

impl Clone for Generic {
	fn clone(&self) -> Self {
		Generic {
			zeros: self.zeros,
			poles: self.poles,
			inputs: {
				let mut v = Samples::new();
				for _ in 0..self.inputs.len() {
					v.push_back(StereoData::default());
				}
				v
			},
			outputs: {
				let mut v = Samples::new();
				for _ in 0..self.outputs.len() {
					v.push_back(StereoData::default());
				}
				v
			}
		}
	}
}
