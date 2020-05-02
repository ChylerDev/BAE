//! # Square
//! 
//! A square wave generator.

use super::*;

/// Struct for generating square wave samples at a specified frequency
pub struct Square {
    ind:MathT,
    inv:MathT,
}

impl FreqMod for Square {
    fn new(f:MathT) -> Self {
        Square {
            ind: 0.0,
            inv: SAMPLE_RATE as MathT/(2.0 * f)
        }
    }

    fn set_frequency(&mut self, f: MathT) {
        self.inv = SAMPLE_RATE as MathT/(2.0 * f);
    }

    fn get_frequency(&self) -> MathT {
        SAMPLE_RATE as MathT/(2.0 * self.inv)
    }
}

impl Generator for Square {
    fn process(&mut self) -> SampleT {
        let y = if self.ind >= self.inv && self.ind < 2.0 * self.inv {
            -1.0
        } else {
            1.0
        };

        if self.ind >= 2.0 * self.inv {
            self.ind -= 2.0 * self.inv;
        }

        self.ind += 1.0;

        y
    }
}

impl Clone for Square {
    fn clone(&self) -> Self {
        Square {
            ind: 0.0,
            inv: self.inv
        }
    }
}
