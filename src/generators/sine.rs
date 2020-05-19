//! # Sine
//! 
//! A sinusoidal sample generator.

use lazy_static::lazy_static;
use super::*;

/// The number of elements in the wavetable.
const WAVETABLE_SIZE: usize = 1024*1024;

lazy_static! {
    /// Lazy static initialization of the static WAVETABLE object.
    static ref WAVETABLE: Vec<MathT> = {
        let mut wt = Vec::new();
        for i in 0..WAVETABLE_SIZE {
            wt.push((2.0*std::f64::consts::PI*(i as MathT)/(WAVETABLE_SIZE as MathT)).sin());
        }
        wt
    };
}

/// Struct for generating sinusoidal samples.
pub struct Sine {
    ind: MathT,
    inc: MathT,
    sample_rate: MathT,
    table: &'static [MathT],
}

impl FreqMod for Sine {
    fn new(f: MathT, sample_rate: MathT) -> Self {
        Sine{
            ind: 0.0,
            inc: (f * WAVETABLE_SIZE as MathT) / sample_rate,
            sample_rate,
            table: WAVETABLE.as_slice()
        }
    }

    fn set_frequency(&mut self, f: MathT) {
        self.inc = (f * WAVETABLE_SIZE as MathT) / self.sample_rate;
    }

    fn get_frequency(&self) -> MathT {
        self.inc * self.sample_rate as MathT / WAVETABLE_SIZE as MathT
    }
}

impl Generator for Sine {
    fn process(&mut self) -> SampleT {
        let k = self.ind.floor();
        let g = self.ind - k;
        let k1 = if k + 1.0 >= WAVETABLE_SIZE as MathT {
            0.0
        } else {
            k + 1.0
        } as usize;
        let k = k as usize;

        let y = ((1.0-g)*self.table[k] + g*self.table[k1]) as SampleT;

        self.ind += self.inc;

        if self.ind >= (WAVETABLE_SIZE as MathT) - 1.0 {
            self.ind -= WAVETABLE_SIZE as MathT - 1.0;
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
            table: self.table,
        }
    }
}
