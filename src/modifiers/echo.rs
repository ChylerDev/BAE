//! # Echo

use super::*;
use std::collections::VecDeque;
use std::time::Duration;

/// Simple Echo filter: H(z) = 1/(1-az^-d)
pub struct Echo {
    sample_rate: MathT,
    delay: VecDeque<SampleT>,
    gain: SampleT,
}

impl Echo {
    /// Creates a new ['Echo'] object with the given delay duration and feedback
    /// amount.
    /// 
    /// [`Echo`]: struct.Echo.html
    pub fn new(d: Duration, g: MathT, sample_rate: MathT) -> Self{
        let mut v = VecDeque::new();
        for _ in 0..((d.as_secs_f64() * sample_rate as MathT) as usize) {
            v.push_back(SampleT::default());
        }

        Echo {
            sample_rate,
            delay: {
                let mut v = VecDeque::new();

                for _ in 0..((d.as_secs_f64() * sample_rate as MathT) as usize) {
                    v.push_back(SampleT::default());
                }

                v
            },
            gain: g as SampleT,
        }
    }
}

impl Modifier for Echo {
    fn process(&mut self, x: SampleT) -> SampleT {
        let wet = self.delay.pop_front().unwrap() * self.gain + x;
        self.delay.push_back(wet);

        wet
    }
}

impl Clone for Echo {
    fn clone(&self) -> Self {
        Echo {
            sample_rate: self.sample_rate,
            delay: {
                let mut v = VecDeque::new();

                for _ in 0..(self.delay.len() * self.sample_rate as usize) {
                    v.push_back(SampleT::default());
                }

                v
            },
            gain: self.gain,
        }
    }
}
