//! # Band Pass

use super::*;

/// -12dB per octave BandPass filter.
pub struct BandPass {
	central_f: MathT,
	quality: MathT,
	a0: MathT,
	b1: MathT,
	b2: MathT,
	x1: SampleT,
	x2: SampleT,
	y1: SampleT,
	y2: SampleT,
}

impl BandPass {
	/// Creates a new BandPass object from the given central frequency and q
	/// value.
	/// 
	/// The filter's quality is set to the central frequency divided by the
	/// difference between the corner frequencies.
	pub fn new(f: MathT, q: MathT) -> Self {
		let mut bp = BandPass {
			central_f: f,
			quality: q,
			a0: 0.0,
			b1: 0.0,
			b2: 0.0,
			x1: SampleT::default(),
			x2: SampleT::default(),
			y1: SampleT::default(),
			y2: SampleT::default(),
		};

		bp.reset();

		bp
	}

	/// Creates a new BandPass object from given corner frequencies.
	pub fn from_corners(f: (MathT,MathT)) -> Self {
		let mut bp = BandPass {
			central_f: (f.0*f.1).abs().sqrt(),
			quality: (f.0*f.1).abs().sqrt()/(f.1-f.0).abs(),
			a0: 0.0,
			b1: 0.0,
			b2: 0.0,
			x1: SampleT::default(),
			x2: SampleT::default(),
			y1: SampleT::default(),
			y2: SampleT::default(),
		};

		bp.reset();

		bp
	}

	/// Returns the central frequency of the filter.
	pub fn get_central_frequency(&self) -> MathT {
		self.central_f
	}

	/// Sets a new central frequency.
	pub fn set_central_frequency(&mut self, f: MathT) {
		self.central_f = f;

		self.reset();
	}

	/// Returns the quality of the filter.
	pub fn get_quality(&self) -> MathT {
		self.quality
	}

	/// Sets the quality of the filter.
	/// 
	/// The filter's quality is set to the central frequency divided by the
	/// difference between the corner frequencies.
	pub fn set_quality(&mut self, q: MathT) {
		self.quality = q;

		self.reset();
	}

	/// Returns the corner frequencies of the filter.
	pub fn get_corner_frequencies(&self) -> (MathT,MathT) {
		let b = -self.central_f/self.quality;

		let (p,n) = quadratic(1.0, b, -self.central_f*self.central_f);
		let fl = if p > 0.0 {
			p
		} else {
			n
		};
		let fh = fl + b;

		if fl < fh {
			(fl, fh)
		} else {
			(fh, fl)
		}
	}

	/// Sets the corner frequencies of the filter.
	pub fn set_corner_frequencies(&mut self, f: (MathT,MathT)) {
		self.central_f = (f.0 * f.1).sqrt();
		self.quality = self.central_f/(f.0-f.1).abs();

		self.reset();
	}

	fn reset(&mut self) {
		let (fh, fl) = self.get_corner_frequencies();

		let theta_l = (std::f64::consts::PI * fl * INV_SAMPLE_RATE).tan();
		let theta_h = (std::f64::consts::PI * fh * INV_SAMPLE_RATE).tan();

		let al = 1.0 / (1.0+theta_l);
		let ah = 1.0 / (1.0+theta_h);

		let bl = (1.0-theta_l) / (1.0+theta_l);
		let bh = (1.0-theta_h) / (1.0+theta_h);

		self.a0 = (1.0-al) * ah;
		self.b1 = bl + bh;
		self.b2 = bl * bh;
	}
}

impl Modifier<BandPass> for BandPass {
	fn process(&mut self, x: SampleT) -> SampleT {
		let y = (self.a0 * (x - self.x2) as MathT +
			self.b1 * self.y1 as MathT - 
			self.b2 * self.y2 as MathT) as SampleT;

		self.y2 = self.y1;
		self.y1 = y;
		self.x2 = self.x1;
		self.x1 = x;

		y
	}
}

fn quadratic(a: MathT, b: MathT, c: MathT) -> (MathT,MathT) {
	(
		(-b + (b*b - 4.0*a*c).sqrt())/(2.0*a),
		(-b - (b*b - 4.0*a*c).sqrt())/(2.0*a)
	)
}

#[cfg(test)]
impl Name for BandPass {
	fn get_name(&self) -> &str {
		"Modifier.BandPass"
	}
}

impl Clone for BandPass {
	fn clone(&self) -> Self {
		BandPass {
			central_f: self.central_f,
			quality: self.quality,
			a0: self.a0,
			b1: self.b1,
			b2: self.b2,
			x1: SampleT::default(),
			x2: SampleT::default(),
			y1: SampleT::default(),
			y2: SampleT::default(),
		}
	}
}
