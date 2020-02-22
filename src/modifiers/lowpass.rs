//! # HighPass
//! 
//! 18dB/octave
//! Derived from 3rd Order Butterworth Low Pass Filter.

use super::*;

/// 3rd Order Butterworth Low Pass Filter with resonance.
pub struct LowPass {
	fc: MathT,
	r: MathT,
	coeff: [SampleT; 4],
	outs: [SampleT; 3]
}

impl LowPass {
	/// Creates a new low pass from the given cutoff frequency and resonance
	/// values.
	/// 
	/// # Parameters
	/// 
	/// * `fc` - The cutoff frequency.
	/// * `r` - The resonance of the filter. Value should be in the range [0,1].
	/// If the value falls out of that range it is clamped to the closer value.
	pub fn new(fc: MathT, r: MathT) -> LowPass {
		let fc = fc.min(SAMPLE_RATE as MathT / 2.0);
		let r = r.min(1.0).max(0.0);

		let mut lp = LowPass {
			fc,
			r,
			coeff: [0.0; 4],
			outs: [SampleT::default(); 3]
		};

		lp.reset();

		lp
	}

	/// Returns the central frequency of the filter.
	pub fn get_central_frequency(&self) -> MathT {
		self.fc
	}

	/// Sets the central frequency of the filter.
	pub fn set_central_frequency(&mut self, fc: MathT) {
		let fc = fc.min(SAMPLE_RATE as MathT / 2.0);

		self.fc = fc;
		self.reset();
	}

	/// Returns the resonance of the filter.
	pub fn get_resonance(&self) -> MathT {
		self.r
	}

	/// Sets the resonance of the filter.
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
	fn process(&mut self, x: SampleT) -> SampleT {
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
			outs: [SampleT::default(); 3]
		}
	}
}
