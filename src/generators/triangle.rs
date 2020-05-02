//! # Triangle
//! 
//! A triangle wave generator.

use super::*;

/// Struct for generating triangle waves at a given frequency.
pub struct Triangle {
    irate: MathT,
    inc: MathT,
}

impl FreqMod for Triangle {
    fn new(f: MathT) -> Self {
        Triangle {
            irate: 4.0 * f * INV_SAMPLE_RATE,
            inc: 0.0,
        }
    }

    fn set_frequency(&mut self, f: MathT) {
        self.irate = 4.0 * f * INV_SAMPLE_RATE;
    }

    fn get_frequency(&self) -> MathT {
        self.irate / (4.0 * INV_SAMPLE_RATE)
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
            irate: self.irate,
            inc: 0.0
        }
    }
}
