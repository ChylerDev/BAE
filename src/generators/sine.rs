//! # Sine
//! 
//! A sinusoidal sample generator.

use lazy_static::lazy_static;
use super::*;

/// The number of elements in the wavetable.
const WAVETABLE_SIZE:u64 = 1024;

lazy_static! {
    /// Lazy static initialization of the static WAVETABLE object.
    static ref WAVETABLE: [MathT; WAVETABLE_SIZE as usize] = {
        let mut wt = [0.0 ; WAVETABLE_SIZE as usize];
        for i in 0..WAVETABLE_SIZE {
            wt[i as usize] = MathT::sin(2.0*std::f64::consts::PI*(i as MathT)/(WAVETABLE_SIZE as MathT));
        }
        wt
    };
}

/// Struct for generating sinusoidal samples.
pub struct Sine {
    ind: MathT,
    inc: MathT,
    sample_rate: MathT,
}

impl FreqMod for Sine {
    fn new(f: MathT, sample_rate: MathT) -> Self {
        Sine{
            ind: 0.0,
            inc: f / (sample_rate * WAVETABLE_SIZE as MathT),
            sample_rate,
        }
    }

    fn set_frequency(&mut self, f: MathT) {
        self.inc = f / (self.sample_rate * WAVETABLE_SIZE as MathT);
    }

    fn get_frequency(&self) -> MathT {
        self.inc * self.sample_rate as MathT * WAVETABLE_SIZE as MathT
    }
}

impl Generator for Sine {
    fn process(&mut self) -> SampleT {
        let k = MathT::floor(self.ind);
        let g = self.ind - k;
        let k1 = if k + 1.0 >= WAVETABLE_SIZE as MathT {
            0.0
        } else {
            k + 1.0
        } as usize;
        let k = k as usize;

        let y = ((1.0-g)*WAVETABLE[k] + g*WAVETABLE[k1]) as SampleT;

        self.ind += self.inc;

        if self.ind >= (WAVETABLE_SIZE as MathT) - 1.0 {
            self.ind -= WAVETABLE_SIZE as MathT;
        }

        y
    }
}

impl Clone for Sine {
    fn clone(&self) -> Self {
        Sine {
            ind: 0.0,
            inc: self.inc,
            sample_rate: self.sample_rate,
        }
    }
}
