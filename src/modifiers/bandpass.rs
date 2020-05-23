//! # Band Pass

use super::*;

/// -12dB per octave BandPass filter.
pub struct BandPass {
    coeffs: [MathT; 3],

    xn: [SampleT; 2],
    yn: [SampleT; 2],

    sample_rate: MathT,

    central_f: MathT,
    quality: MathT,
}

impl BandPass {
    /// Creates a new BandPass object from the given central frequency and q
    /// value.
    ///
    /// The filter's quality is set to the central frequency divided by the
    /// difference between the corner frequencies.
    pub fn new(f: MathT, q: MathT, sample_rate: MathT) -> Self {
        let mut bp = BandPass {
            coeffs: [Default::default(); 3],

            xn: [Default::default(); 2],
            yn: [Default::default(); 2],

            sample_rate,

            central_f: f,
            quality: q,
        };

        bp.reset();

        bp
    }

    /// Creates a new BandPass object from given corner frequencies.
    pub fn from_corners(f: (MathT, MathT), sample_rate: MathT) -> Self {
        let mut bp = BandPass {
            coeffs: [Default::default(); 3],

            xn: [Default::default(); 2],
            yn: [Default::default(); 2],

            sample_rate,

            central_f: (f.0 * f.1).abs().sqrt(),
            quality: (f.0 * f.1).abs().sqrt() / (f.1 - f.0).abs(),
        };

        bp.reset();

        bp
    }

    /// Returns the central frequency of the filter.
    pub fn get_central_frequency(&self) -> MathT {
        self.central_f
    }

    /// Sets a new central frequency.
    pub fn set_central_frequency(&mut self, f: MathT) {
        self.central_f = f;

        self.reset();
    }

    /// Returns the quality of the filter.
    pub fn get_quality(&self) -> MathT {
        self.quality
    }

    /// Sets the quality of the filter.
    ///
    /// The filter's quality is set to the central frequency divided by the
    /// difference between the corner frequencies.
    pub fn set_quality(&mut self, q: MathT) {
        self.quality = q;

        self.reset();
    }

    /// Returns the corner frequencies of the filter.
    pub fn get_corner_frequencies(&self) -> (MathT, MathT) {
        let b = -self.central_f / self.quality;

        let (p, n) = quadratic(1.0, b, -self.central_f * self.central_f);
        let fl = if p > 0.0 { p } else { n };
        let fh = fl + b;

        if fl < fh {
            (fl, fh)
        } else {
            (fh, fl)
        }
    }

    /// Sets the corner frequencies of the filter.
    pub fn set_corner_frequencies(&mut self, f: (MathT, MathT)) {
        self.central_f = (f.0 * f.1).sqrt();
        self.quality = self.central_f / (f.0 - f.1).abs();

        self.reset();
    }

    fn reset(&mut self) {
        let (fh, fl) = self.get_corner_frequencies();

        let theta_l = (std::f64::consts::PI * fl / self.sample_rate).tan();
        let theta_h = (std::f64::consts::PI * fh / self.sample_rate).tan();

        let al = 1.0 / (1.0 + theta_l);
        let ah = 1.0 / (1.0 + theta_h);

        let bl = (1.0 - theta_l) / (1.0 + theta_l);
        let bh = (1.0 - theta_h) / (1.0 + theta_h);

        self.coeffs[0] = (1.0 - al) * ah;
        self.coeffs[1] = bl + bh;
        self.coeffs[2] = bl * bh;
    }
}

impl Modifier for BandPass {
    fn process(&mut self, x: SampleT) -> SampleT {
        let y = (self.coeffs[0] * (x - self.xn[1]) as MathT + self.coeffs[1] * self.yn[0] as MathT
            - self.coeffs[2] * self.yn[1] as MathT) as SampleT;

        self.xn.rotate_right(1);
        self.xn[0] = x;
        self.yn.rotate_right(1);
        self.yn[0] = y;

        y
    }
}

fn quadratic(a: MathT, b: MathT, c: MathT) -> (MathT, MathT) {
    (
        (-b + (b * b - 4.0 * a * c).sqrt()) / (2.0 * a),
        (-b - (b * b - 4.0 * a * c).sqrt()) / (2.0 * a),
    )
}

impl Clone for BandPass {
    fn clone(&self) -> Self {
        BandPass {
            coeffs: self.coeffs,

            xn: [Default::default(); 2],
            yn: [Default::default(); 2],

            sample_rate: self.sample_rate,

            central_f: self.central_f,
            quality: self.quality,
        }
    }
}
