//! # Triangle
//!
//! A triangle wave generator.

use super::*;

/// Struct for generating triangle waves at a given frequency.
pub struct Triangle {
    sample_rate: MathT,
    irate: MathT,
    inc: MathT,
}

impl FreqMod for Triangle {
    fn new(f: MathT, sample_rate: MathT) -> Self {
        Triangle {
            sample_rate,
            irate: 4.0 * f / sample_rate,
            inc: 0.0,
        }
    }

    fn set_frequency(&mut self, f: MathT) {
        self.irate = 4.0 * f / self.sample_rate;
    }

    fn get_frequency(&self) -> MathT {
        self.irate / (4.0 / self.sample_rate)
    }
}

impl Generator for Triangle {
    fn process(&mut self) -> SampleT {
        let y = self.inc;

        self.inc += self.irate;

        if self.inc >= 1.0 || self.inc <= -1.0 {
            self.irate = -self.irate;

            self.inc = if self.inc >= 1.0 {
                2.0 - self.inc
            } else {
                -2.0 - self.inc
            };
        }

        y as SampleT
    }
}

impl Clone for Triangle {
    fn clone(&self) -> Self {
        Triangle {
            sample_rate: self.sample_rate,
            irate: self.irate,
            inc: 0.0,
        }
    }
}
