//! # Sine
//! 
//! A sinusoidal sample generator.

use lazy_static::lazy_static;
use super::*;

/// The frequency used by the wavetable.
const WAVETABLE_FREQ:u64 = 10;
/// The number of elements in the wavetable.
const WAVETABLE_SIZE:u64 = SAMPLE_RATE/WAVETABLE_FREQ;

lazy_static! {
	/// Lazy static initialization of the static WAVETABLE object.
	static ref WAVETABLE: [MathT; WAVETABLE_SIZE as usize] = {
		let mut wt = [0.0 ; WAVETABLE_SIZE as usize];
		for i in 0..WAVETABLE_SIZE {
			wt[i as usize] = MathT::sin(2.0*std::f64::consts::PI*INV_SAMPLE_RATE*(WAVETABLE_FREQ as MathT)*(i as MathT));
		}
		wt
	};
}

/// Struct for generating sinusoidal samples.
pub struct Sine {
	ind:MathT,
	inc:MathT,
}

impl FreqMod for Sine {
	fn new(f: MathT) -> Self {
		Sine{
			ind: 0.0,
			inc: f/(WAVETABLE_FREQ as MathT),
		}
	}

	fn set_frequency(&mut self, f: MathT) {
		self.inc = f / (WAVETABLE_FREQ as MathT);
	}

	fn get_frequency(&self) -> MathT {
		self.inc * (WAVETABLE_FREQ as MathT)
	}
}

impl Generator<Sine> for Sine {
	fn process(&mut self) -> StereoData {
		let k = MathT::floor(self.ind);
		let g:MathT = self.ind - k;
		let k1 = if k+1.0 >= WAVETABLE_SIZE as MathT {
			0.0
		} else {
			k+1.0
		} as usize;
		let k = k as usize;

		let y = ((1.0-g)*WAVETABLE[k] + g*WAVETABLE[k1]) as SampleT;

		self.ind += self.inc;

		if self.ind >= (WAVETABLE_SIZE as MathT)-1.0 {
			self.ind -= WAVETABLE_SIZE as MathT;
		}

		StereoData::from_mono(y)
	}
}

impl Clone for Sine {
	fn clone(&self) -> Self {
		Sine {
			ind: 0.0,
			inc: self.inc
		}
	}
}

impl Name for Sine {
	fn get_name(&self) -> &str {
		"Generator.Sine"
	}
}
