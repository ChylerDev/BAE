//! # HighPass
//! 
//! 18dB/octave
//! Derived from 3rd Order Butterworth Low Pass Filter.

use super::*;

pub struct LowPass {
	fc: MathT,
	r: MathT,
	coeff: [SampleT; 4],
	outs: [StereoData; 3]
}

impl LowPass {
	pub fn new(fc: MathT, r: MathT) -> LowPass {
		let fc = fc.min(SAMPLE_RATE as MathT / 2.0);
		let r = r.min(1.0).max(0.0);

		let mut lp = LowPass {
			fc,
			r,
			coeff: [0.0; 4],
			outs: [StereoData::default(); 3]
		};

		lp.reset();

		lp
	}

	pub fn get_central_frequency(&self) -> MathT {
		self.fc
	}
	pub fn set_central_frequency(&mut self, fc: MathT) {
		let fc = fc.min(SAMPLE_RATE as MathT / 2.0);

		self.fc = fc;
		self.reset();
	}

	pub fn get_resonance(&self) -> MathT {
		self.r
	}
	pub fn set_resonance(&mut self, r: MathT) {
		let r = r.min(1.0).max(0.0);

		self.r = r;
		self.reset();
	}

	fn reset(&mut self) {
		let theta = (std::f64::consts::PI / 6.0) * (4.0 - self.r);
		let k = 1.0 - 2.0 * theta.cos();
		let w = 2.0 * std::f64::consts::PI * self.fc;
		let t = w * INV_SAMPLE_RATE;
		let g = t.powf(3.0) + k*t.powf(2.0) + k*t + 1.0;

		self.coeff[0] = (t.powf(3.0) / g) as SampleT;
		self.coeff[1] = ((k*t.powf(2.0) + 2.0*k*t + 3.0) / g) as SampleT;
		self.coeff[2] = ((-k*t - 3.0) / g) as SampleT;
		self.coeff[3] = (1.0 / g) as SampleT;
	}
}

impl Modifier<LowPass> for LowPass {
	fn process(&mut self, x: StereoData) -> StereoData {
		let y = self.coeff[0]*x +
				self.coeff[1]*self.outs[0] +
				self.coeff[2]*self.outs[1] +
				self.coeff[3]*self.outs[2];

		self.outs[2] = self.outs[1];
		self.outs[1] = self.outs[0];
		self.outs[0] = y;

		y
	}
}

#[cfg(test)]
impl Name for LowPass {
	fn get_name(&self) -> &str {
		"Modifiers.LowPass"
	}
}

impl Clone for LowPass {
	fn clone(&self) -> Self {
		LowPass {
			fc: self.fc,
			r: self.r,
			coeff: self.coeff,
			outs: [StereoData::default(); 3]
		}
	}
}
