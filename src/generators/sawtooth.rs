/// # Sawtooth
/// 
/// A sawtooth sample generator.

use super::*;
use super::FreqMod;
use super::Generator;

/// Struct for generating sawtooth samples.
pub struct Sawtooth {
    irate: MathT,
    inc: SampleT,
}

impl Sawtooth {
    /// Creates a new sawtooth object for the given frequency.
    /// 
    /// # Parameters
    /// 
    /// * `f` - The frequency for the new object
    fn new(f: MathT) -> Sawtooth {
        Sawtooth {
            irate: 2*f*INV_SAMPLE_RATE,
            inc: 0.0,
        }
    }
}

impl FreqMod for Sawtooth {
    fn set_frequency(&mut self, f: MathT) {
        self.irate = 2.0*f*INV_SAMPLE_RATE;
    }

    fn get_frequency(&self) -> MathT {
        self.irate / (2.0 * INV_SAMPLE_RATE)
    }
}

impl Generator for Sawtooth {
    fn process(&mut self) -> StereoData {
        let y = self.inc;

        self.inc += self.irate as SampleT;

        if self.inc >= 1.0 {
            self.inc -= 2;
        }

        StereoData::from_mono(y)
    }
}
