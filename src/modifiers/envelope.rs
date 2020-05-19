//! # Envelope

use super::*;

/// Envelope Follower filter. I don't remember my lectures well enough to write
/// a detailed description.
pub struct Envelope {
    sample_rate: MathT,

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
    pub fn new(lower: MathT, upper: MathT, sample_rate: MathT) -> Envelope{
        let theta_u = (std::f64::consts::PI * upper / sample_rate).tan();
        let theta_d = (std::f64::consts::PI * lower / sample_rate).tan();

        Envelope {
            sample_rate,

            au: (theta_u / (1.0 + theta_u)) as SampleT,
            bu: ((1.0 - theta_u) / (1.0 + theta_u)) as SampleT,
            ad: (theta_d / (1.0 + theta_d)) as SampleT,
            bd: ((1.0 - theta_d) / (1.0 + theta_d)) as SampleT,

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
            sample_rate: self.sample_rate,

            au: self.au,
            bu: self.bu,
            ad: self.ad,
            bd: self.bd,
            
            x1: Default::default(),
            y1: Default::default(),
        }
    }
}
