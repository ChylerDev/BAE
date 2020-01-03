use super::*;

pub enum ADSRState {
	attack,
	decay,
	sustain,
	release,
	stopped,
}

pub struct ADSR {
	a:SampleT,
	d:SampleT,
	s:SampleT,
	r:SampleT,
	state:ADSRState,
	g:SampleT,
}

impl ADSR {
	pub fn new(a:MathT,d:MathT,s:MathT,r:MathT) -> Self {
		ADSR {
			a: 1.0/(a * SAMPLE_RATE as MathT),
			d: (db_linear(s)-1.0)/(d * SAMPLE_RATE as MathT),
			s: db_linear(s),
			r: (-db_linear(s))/(r * SAMPLE_RATE as MathT),
			state: ADSRState::attack,
			gain: 0.0
		}
	}

	pub fn release(&mut self) {
		self.state = ADSRState::release;
	}
}

impl Modifier for ADSR {
	fn process(&mut self, x: StereoData) -> StereoData {
		match self.state {
			attack => {
				self.g += self.a;
				if self.g >= 1.0 {
					self.state = decay;
					self.g = 1.0;
				}
			},
			decay => {
				self.g += self.d;
				if self.g <= self.s {
					self.state = sustain;
					self.g = self.s;
				}
			},
			sustain => {
			},
			release => {
				self.g += self.r;
				if self.g <= 0.0 {
					self.state = stopped;
					self.g = 0.0;
				}
			},
			stopped => return StereoData::default(),
		}

		x * self.g
	}
}
