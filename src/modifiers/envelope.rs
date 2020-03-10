//! # Envelope

use super::*;

/// Envelope Follower filter. I don't remember my lectures well enough to write
/// a detailed description.
pub struct Envelope {
	au: SampleT,
	bu: SampleT,
	ad: SampleT,
	bd: SampleT,

	x1: SampleT,
	y1: SampleT,
}

impl Envelope {
	/// Creates a new [`Envelope`] object from the given max and min frequencies
	/// to follow.
	/// 
	/// [`Envelope`]: struct.Envelope.html
	pub fn new(lower: SampleT, upper: SampleT) -> Envelope{
		let theta_u = (std::f32::consts::PI * upper * INV_SAMPLE_RATE as SampleT).tan();
		let theta_d = (std::f32::consts::PI * lower * INV_SAMPLE_RATE as SampleT).tan();

		Envelope {
			au: theta_u / (1.0 + theta_u),
			bu: (1.0 - theta_u) / (1.0 + theta_u),
			ad: theta_d / (1.0 + theta_d),
			bd: (1.0 - theta_d) / (1.0 + theta_d),

			x1: SampleT::default(),
			y1: SampleT::default(),
		}
	}
}

impl Modifier for Envelope {
	fn process(&mut self, x: SampleT) -> SampleT {
		let y = if x.abs() > self.y1 {
			self.au * (x.abs() + self.x1.abs()) + self.bu * self.y1
		} else {
			self.ad * (x.abs() + self.x1.abs()) + self.bd * self.y1
		};

		self.y1 = y;
		self.x1 = x;

		y
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
