//! # Echo

use super::*;
use std::collections::VecDeque;
use std::time::Duration;

/// Simple Echo filter: H(z) = 1/(1-az^-d)
pub struct Echo {
    delay: VecDeque<SampleT>,
    gain: SampleT,
}

impl Echo {
    /// Creates a new ['Echo'] object with the given delay duration and feedback
    /// amount.
    /// 
    /// [`Echo`]: struct.Echo.html
    pub fn new(d: Duration, g: MathT) -> Self{
        let mut v = VecDeque::new();
        for _ in 0..((d.as_secs_f64()*SAMPLE_RATE as MathT) as usize) {
            v.push_back(SampleT::default());
        }

        Echo {
            delay: v,
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
            delay: {
                let mut v = VecDeque::new();

                for _ in 0..(self.delay.len() * SAMPLE_RATE as usize) {
                    v.push_back(SampleT::default());
                }

                v
            },
            gain: self.gain,
        }
    }
}
