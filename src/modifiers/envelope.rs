//! # Envelope

use super::*;

pub struct Envelope {
	au: SampleT,
	bu: SampleT,
	ad: SampleT,
	bd: SampleT,

	x1: StereoData,
	y1: StereoData,
}

impl Envelope {
	pub fn new(lower: SampleT, upper: SampleT) -> Envelope{
		let theta_u = (std::f32::consts::PI * upper * INV_SAMPLE_RATE as SampleT).tan();
		let theta_d = (std::f32::consts::PI * lower * INV_SAMPLE_RATE as SampleT).tan();

		Envelope {
			au: theta_u / (1.0 + theta_u),
			bu: (1.0 - theta_u) / (1.0 + theta_u),
			ad: theta_d / (1.0 + theta_d),
			bd: (1.0 - theta_d) / (1.0 + theta_d),

			x1: StereoData::default(),
			y1: StereoData::default(),
		}
	}
}

impl Modifier for Envelope {
	fn process(&mut self, x: StereoData) -> StereoData {
		let y = StereoData::from_stereo(
			if x.left.abs() > self.y1.left {
				self.au * (x.left.abs() + self.x1.left.abs()) + self.bu * self.y1.left
			} else {
				self.ad * (x.left.abs() + self.x1.left.abs()) + self.bd * self.y1.left
			},
			if x.right.abs() > self.y1.right {
				self.au * (x.right.abs() + self.x1.right.abs()) + self.bu * self.y1.right
			} else {
				self.ad * (x.right.abs() + self.x1.right.abs()) + self.bd * self.y1.right
			}
		);

		self.y1 = y;
		self.x1 = x;

		y
	}
}

impl Name for Envelope {
	fn get_name(&self) -> &str {
		"Modifiers.Envelope"
	}
}

impl Clone for Envelope {
	fn clone(&self) -> Self {
		Envelope {
			au: self.au,
			bu: self.bu,
			ad: self.ad,
			bd: self.bd,
			
			x1: Default::default(),
			y1: Default::default(),
		}
	}
}
