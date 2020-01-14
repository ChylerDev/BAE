use super::*;

pub struct HighPass {
	a0: SampleT,
	a1: SampleT,
	a2: SampleT,
	a3: SampleT,
	b1: SampleT,
	b2: SampleT,
	b3: SampleT,

	x1: StereoData,
	x2: StereoData,
	x3: StereoData,
	y1: StereoData,
	y2: StereoData,
	y3: StereoData,

	fc: MathT,
	r: MathT
}

impl HighPass {
	pub fn new(fc: MathT, r: MathT) -> HighPass {
		let r = r.min(1.0).max(0.0);
		let mut hp = HighPass {
			a0: SampleT::default(),
			a1: SampleT::default(),
			a2: SampleT::default(),
			a3: SampleT::default(),
			b1: SampleT::default(),
			b2: SampleT::default(),
			b3: SampleT::default(),
			x1: StereoData::default(),
			x2: StereoData::default(),
			x3: StereoData::default(),
			y1: StereoData::default(),
			y2: StereoData::default(),
			y3: StereoData::default(),
			fc,
			r,
		};

		hp.reset();

		hp
	}

	fn reset(&mut self) {
		let theta = std::f64::consts::PI * (4.0 - self.r) / 6.0;
		let k = 1.0 - 2.0 * theta.cos();
		let w = 2.0 * std::f64::consts::PI * self.fc;
		let t = w * INV_SAMPLE_RATE;
		let g = t.powf(3.0) + k*t.powf(2.0) + k*t + 1.0;

		self.a0 = ( 1.0/g) as SampleT;
		self.a1 = (-3.0/g) as SampleT;
		self.a2 = ( 3.0/g) as SampleT;
		self.a3 = (-1.0/g) as SampleT;

		self.b1 = ((k*t.powf(2.0) + 2.0*k*t + 3.0)/g) as SampleT;
		self.b2 = ((-k*t - 3.0)/g) as SampleT;
		self.b3 = (1.0/g) as SampleT;
	}
}

impl Modifier for HighPass {
	fn process(&mut self, x: StereoData) -> StereoData {
		let y = StereoData::from_stereo(
			self.a0*x.left + self.a1*self.x1.left + self.a2*self.x2.left + self.a3*self.x3.left +
			self.b1*self.y1.left + self.b2*self.y2.left + self.b3*self.y3.left,
			self.a0*x.right + self.a1*self.x1.right + self.a2*self.x2.right + self.a3*self.x3.right +
			self.b1*self.y1.right + self.b2*self.y2.right + self.b3*self.y3.right
		);

		self.x3 = self.x2;
		self.x2 = self.x1;
		self.x1 = x;
		self.y3 = self.y2;
		self.y2 = self.y1;
		self.y1 = y;

		y
	}
}
