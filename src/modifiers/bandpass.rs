//! # Band Pass
//! 
//! Derived from the 18dB/octave High- and Low-Pass filters with resonance.

use super::*;

pub struct BandPass {
	fc: MathT,
	q: MathT,
	r: MathT,
	zeros: [SampleT; 4],
	poles: [SampleT; 6],
	inputs: [StereoData; 3],
	outputs: [StereoData; 6],
}

impl BandPass {
	pub fn new(fc: MathT, q: MathT, r: MathT) -> Self {
		let fc = fc.abs().min(SAMPLE_RATE as MathT / 2.0);
		let r = r.min(1.0).max(0.0);

		let mut bp = BandPass {
			fc,
			q,
			r,
			zeros: Default::default(),
			poles: Default::default(),
			inputs: Default::default(),
			outputs: Default::default(),
		};

		bp.reset();

		bp
	}

	pub fn from_corners(f: (MathT,MathT), r: MathT) -> Self {
		let f = (
			f.0.abs().min(SAMPLE_RATE as MathT / 2.0),
			f.1.abs().min(SAMPLE_RATE as MathT / 2.0)
		);
		let r = r.min(1.0).max(0.0);

		let mut bp = BandPass {
			fc: (f.0*f.1).abs().sqrt(),
			q: (f.0*f.1).abs().sqrt()/(f.1-f.0).abs(),
			r,
			zeros: Default::default(),
			poles: Default::default(),
			inputs: Default::default(),
			outputs: Default::default(),
		};

		bp.reset();

		bp
	}

	pub fn get_central_frequency(&self) -> MathT {
		self.fc
	}

	pub fn set_central_frequency(&mut self, f: MathT) {
		self.fc = f;

		self.reset();
	}

	pub fn get_quality(&self) -> MathT {
		self.q
	}

	pub fn set_quality(&mut self, q: MathT) {
		self.q = q;

		self.reset();
	}

	pub fn get_resonance(&self) -> MathT {
		self.r
	}

	pub fn set_resonance(&mut self, r: MathT) {
		self.r = r;

		self.reset();
	}

	pub fn get_corner_frequencies(&self) -> (MathT,MathT) {
		let b = -self.fc/self.q;
		let (p,n) = quadratic(1.0, b, -self.fc*self.fc);
		let fl = if p > 0.0 {
			p
		} else {
			n
		};
		let fh = fl + b;

		(fh,fl)
	}

	pub fn set_corner_frequencies(&mut self, f: (MathT,MathT)) {
		self.fc = (f.0 * f.1).sqrt();
		self.q = self.fc/(f.0-f.1).abs();

		self.reset();
	}

	fn reset(&mut self) {
		let theta = (std::f64::consts::PI / 6.0) * (4.0 - self.r);
		let k = 1.0 - 2.0 * theta.cos();
		let k2 = k.powf(2.0);
		let t = self.fc * INV_SAMPLE_RATE;
		let t2 = t.powf(2.0);
		let t3 = t.powf(3.0);
		let t4 = t.powf(4.0);
		let t5 = t.powf(5.0);
		let t6 = t.powf(6.0);

		let r1 = 2.0*k;
		let r2 = 2.0*k + k2;
		let r3 = 2.0 + 2.0*k2;

		let g = 1.0 + r1*t + r2*t2 + r3*t3 + r2*t4 + r1*t5 + t6;

		self.poles[0] = ((6.0 + 5.0*r1*t + 4.0*r2*t2 + 3.0*r3*t3 + 2.0*r2*t4 + r1*t5) / g) as SampleT;
		self.poles[1] = -((15.0 + 10.0*r1*t + 6.0*r2*t2 + 3.0*r3*t3 + r2*t4) / g) as SampleT;
		self.poles[2] = ((20.0 + 10.0*r1*t + 4.0*r2*t2 + r3*t3) / g) as SampleT;
		self.poles[3] = -((15.0 + 5.0*r1*t + r2*t2) / g) as SampleT;
		self.poles[4] = ((6.0 + r1*t) / g) as SampleT;
		self.poles[5] = -((1.0) / g) as SampleT;

		self.zeros[0] = ((t3) / g) as SampleT;
		self.zeros[1] = ((-3.0*t3) / g) as SampleT;
		self.zeros[2] = ((3.0*t3) / g) as SampleT;
		self.zeros[3] = ((-t3) / g) as SampleT;
	}
}

impl Modifier for BandPass {
	fn process(&mut self, x: StereoData) -> StereoData {
		let y = self.zeros[0]*x + self.zeros[1]*self.inputs[0] + self.zeros[2]*self.inputs[1] + self.zeros[3]*self.inputs[2]
		      + self.poles[0]*self.outputs[0] + self.poles[1]*self.outputs[1] + self.poles[2]*self.outputs[2]
		      + self.poles[3]*self.outputs[3] + self.poles[4]*self.outputs[4] + self.poles[5]*self.outputs[5];

		self.inputs[2] = self.inputs[1];
		self.inputs[1] = self.inputs[0];
		self.inputs[0] = x;

		self.outputs[5] = self.outputs[4];
		self.outputs[4] = self.outputs[3];
		self.outputs[3] = self.outputs[2];
		self.outputs[2] = self.outputs[1];
		self.outputs[1] = self.outputs[0];
		self.outputs[0] = y;

		y
	}
}

fn quadratic(a: MathT, b: MathT, c: MathT) -> (MathT,MathT) {
	(
		(-b + (b*b - 4.0*a*c).sqrt())/(2.0*a),
		(-b - (b*b - 4.0*a*c).sqrt())/(2.0*a)
	)
}


impl Name for BandPass {
	fn get_name(&self) -> &str {
		"Modifiers.BandPass"
	}
}
