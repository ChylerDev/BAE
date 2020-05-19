//! # HighPass
//! 
//! 18dB/octave
//! Derived from 3rd Order Butterworth Low Pass Filter.

use super::*;

/// High pass filter adapted from the 3rd Order Butterworth Low Pass Filter with
/// resonance.
pub struct HighPass {
    an: [SampleT ; 4],
    xn: [SampleT ; 3],

    bn: [SampleT ; 3],
    yn: [SampleT ; 3],

    sample_rate: MathT,

    fc: MathT,
    r: MathT
}

impl HighPass {
    /// Creates a new high pass from the given cutoff frequency and resonance
    /// values.
    /// 
    /// # Parameters
    /// 
    /// * `fc` - The cutoff frequency.
    /// * `r` - The resonance of the filter. Value should be in the range [0,1].
    /// If the value falls out of that range it is clamped to the closer value.
    pub fn new(fc: MathT, r: MathT, sample_rate: MathT) -> HighPass {
        let fc = fc.min(sample_rate / 2.0);
        let r = r.min(1.0).max(0.0);
        let mut hp = HighPass {
            an: [Default::default() ; 4],
            bn: [Default::default() ; 3],

            xn: [Default::default() ; 3],
            yn: [Default::default() ; 3],

            sample_rate,

            fc,
            r,
        };

        hp.reset();

        hp
    }

    /// Returns the central frequency of the filter.
    pub fn get_central_frequency(&self) -> MathT {
        self.fc
    }

    /// Sets the central frequency of the filter.
    pub fn set_central_frequency(&mut self, fc: MathT) {
        let fc = fc.min(self.sample_rate / 2.0);

        self.fc = fc;
        self.reset();
    }

    /// Returns the resonance of the filter.
    pub fn get_resonance(&self) -> MathT {
        self.r
    }

    /// Sets the resonance of the filter. Value should be in the range [0,1].
    /// If the value falls out of that range it is clamped to the closer value.
    pub fn set_resonance(&mut self, r: MathT) {
        let r = r.min(1.0).max(0.0);

        self.r = r;
        self.reset();
    }

    fn reset(&mut self) {
        let theta = std::f64::consts::PI * (4.0 - self.r) / 6.0;
        let k = 1.0 - 2.0 * theta.cos();
        let w = 2.0 * std::f64::consts::PI * self.fc;
        let t = w / self.sample_rate;
        let g = t.powf(3.0) + k*t.powf(2.0) + k*t + 1.0;

        self.an[0] = ( 1.0/g) as SampleT;
        self.an[1] = (-3.0/g) as SampleT;
        self.an[2] = ( 3.0/g) as SampleT;
        self.an[3] = (-1.0/g) as SampleT;

        self.bn[0] = ((k*t.powf(2.0) + 2.0*k*t + 3.0)/g) as SampleT;
        self.bn[1] = ((-k*t - 3.0)/g) as SampleT;
        self.bn[2] = (1.0/g) as SampleT;
    }
}

impl Modifier for HighPass {
    fn process(&mut self, x: SampleT) -> SampleT {
        let y =
            self.an[0] * x +
            self.an[1] * self.xn[0] +
            self.an[2] * self.xn[1] +
            self.an[3] * self.xn[2] +
            self.bn[0] * self.yn[0] +
            self.bn[1] * self.yn[1] +
            self.bn[2] * self.yn[2];

        self.xn.rotate_right(1);
        self.xn[0] = x;
        self.yn.rotate_right(1);
        self.yn[0] = y;

        y
    }
}

impl Clone for HighPass {
    fn clone(&self) -> Self {
        HighPass {
            an: self.an,
            bn: self.bn,

            xn: [SampleT::default() ; 3],
            yn: [SampleT::default() ; 3],

            sample_rate: self.sample_rate,

            fc: self.fc,
            r: self.r,
        }
    }
}
