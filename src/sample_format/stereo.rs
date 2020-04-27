//! # Stereo Data
//! 
//! Module containing type for handling stereo audio data.

use super::*;
use crate::utils::*;

/// Struct representing a stereophonic audio sample.
#[derive(Copy,Clone,Default)]
pub struct Stereo{
    /// Left sample value.
    pub left:  SampleT,
    /// Right sample value.
    pub right: SampleT,
}

impl Stereo {
    /// Returns a new Stereo object with default <0,0> values.
    pub fn new() -> Stereo {
        Stereo {
            left:  0.0,
            right: 0.0,
        }
    }

    /// Returns a new Stereo object created from individual left and right
    /// audio samples.
    /// 
    /// # Parameters
    /// 
    /// * `l` - the left audio sample.
    /// * `r` - the right audio sample.
    pub fn from(l:SampleT, r:SampleT) -> Stereo {
        Stereo {
            left:  l,
            right: r,
        }
    }
}

impl SampleFormat for Stereo
{
    fn from_mono(x:SampleT) -> Self {
        Stereo{
            left:  x * SampleT::sqrt(0.5),
            right: x * SampleT::sqrt(0.5)
        }
    }

    fn into_mono(self) -> SampleT {
        (self.left + self.right)/SampleT::sqrt(0.5)
    }
}

impl Panner<f32> for Stereo {
    fn to_sample_format(s: SampleT, g: f32) -> Self {
        let l_lerp = if g <= 0.0 {
            utils::clerp(g as MathT, -1.0, 0.0, -3.0, -120.0)
        } else {
            utils::clerp(g as MathT, 0.0, 1.0, 0.0, -3.0)
        };
        let r_lerp = if g >= 0.0 {
            utils::clerp(g as MathT, 0.0, 1.0, -3.0, -120.0)
        } else {
            utils::clerp(g as MathT, -1.0, 0.0, 0.0, -3.0)
        };

        Stereo {
            left: (db_to_linear(l_lerp) * s as MathT) as SampleT,
            right: (db_to_linear(r_lerp) * s as MathT) as SampleT
        }
    }
}
impl Panner<f64> for Stereo {
    fn to_sample_format(s: SampleT, g: f64) -> Self {
        let l_lerp = if g <= 0.0 {
            utils::clerp(g as MathT, -1.0, 0.0, -3.0, -120.0)
        } else {
            utils::clerp(g as MathT, 0.0, 1.0, 0.0, -3.0)
        };
        let r_lerp = if g >= 0.0 {
            utils::clerp(g as MathT, 0.0, 1.0, -3.0, -120.0)
        } else {
            utils::clerp(g as MathT, -1.0, 0.0, 0.0, -3.0)
        };

        Stereo {
            left: (db_to_linear(l_lerp) * s as MathT) as SampleT,
            right: (db_to_linear(r_lerp) * s as MathT) as SampleT
        }
    }
}

impl std::ops::Neg for Stereo {
    type Output = Self;

    fn neg(self) -> Self::Output {
        Stereo {
            left: -self.left,
            right: -self.right,
        }
    }
}

impl std::ops::Add<Stereo> for Stereo {
    type Output = Self;

    fn add(self, rhs: Stereo) -> Self::Output {
        Stereo {
            left: self.left + rhs.left,
            right: self.right + rhs.right,
        }
    }
}
impl std::ops::AddAssign<Stereo> for Stereo {
    fn add_assign(&mut self, rhs: Stereo) {
        self.left += rhs.left;
        self.right += rhs.right;
    }
}

impl std::ops::Sub<Stereo> for Stereo {
    type Output = Self;

    fn sub(self, rhs: Stereo) -> Self {
        Stereo {
            left: self.left - rhs.left,
            right: self.right - rhs.right,
        }
    }
}
impl std::ops::SubAssign<Stereo> for Stereo {
    fn sub_assign(&mut self, rhs: Stereo) {
        self.left -= rhs.left;
        self.right -= rhs.right;
    }
}

impl std::ops::Mul<Stereo> for Stereo {
    type Output = Stereo;

    fn mul(self, rhs: Stereo) -> Self::Output {
        Stereo {
            left: self.left * rhs.left,
            right: self.right * rhs.right
        }
    }
}
impl std::ops::MulAssign<Stereo> for Stereo {
    fn mul_assign(&mut self, rhs: Stereo) {
        self.left *= rhs.left;
        self.right *= rhs.right;
    }
}

impl std::ops::Mul<SampleT> for Stereo {
    type Output = Stereo;

    fn mul(self, rhs: SampleT) -> Self::Output {
        Stereo {
            left: self.left * rhs,
            right: self.right * rhs,
        }
    }
}
impl std::ops::MulAssign<SampleT> for Stereo {
    fn mul_assign(&mut self, rhs: SampleT) {
        self.left *= rhs;
        self.right *= rhs;
    }
}

impl std::ops::Mul<MathT> for Stereo {
    /// Output type of the multiplication
    type Output = Stereo;

    /// Multiplies a sample by a value. E.g. scaling the sample by a gain amount.
    fn mul(self, rhs: MathT) -> Self::Output {
        Stereo {
            left:(self.left as MathT * rhs) as SampleT,
            right:(self.right as MathT * rhs) as SampleT,
        }
    }
}
impl std::ops::MulAssign<MathT> for Stereo {
    fn mul_assign(&mut self, rhs: MathT) {
        self.left *= rhs as SampleT;
        self.right *= rhs as SampleT;
    }
}

impl From<SampleT> for Stereo {
    /// Copies the given sample to the left and right channels. If you want to
    /// use the half-power conversion, use [`Stereo::from_mono`].
    /// 
    /// [`Stereo::from_mono`]: struct.Stereo.html#method.from_mono
    fn from(s: SampleT) -> Self {
        Stereo::from_mono(s)
    }
}
impl Into<SampleT> for Stereo {
    fn into(self) -> SampleT {
        self.into_mono()
    }
}

impl From<[u8;2]> for Stereo {
    /// Converts the array of 2 bytes into a Stereo object.
    /// It is assumed that the bytes are 8-bit unsigned audio samples.
    /// 
    /// # Parameters
    /// 
    /// * `v` - The raw bytes to convert from.
    fn from(v:[u8;2]) -> Self {
        Stereo {
            left:  sample_from_u8_bytes([v[0]]),
            right: sample_from_u8_bytes([v[1]])
        }
    }
}
impl Into<[u8;2]> for Stereo {
    fn into(self) -> [u8;2] {
        [sample_to_u8(self.left), sample_to_u8(self.right)]
    }
}

impl From<[u8;4]> for Stereo {
    /// Converts the array of 4 bytes into a Stereo object.
    /// It is assumed that the bytes are 16-bit signed audio samples.
    /// 
    /// # Parameters
    /// 
    /// * `v` - The raw bytes to convert from.
    fn from(v:[u8;4]) -> Self {
        Stereo {
            left:  sample_from_i16_bytes([v[0],v[1]]),
            right: sample_from_i16_bytes([v[2],v[3]])
        }
    }
}
impl Into<[u8;4]> for Stereo {
    fn into(self) -> [u8;4] {
        let l = sample_to_i16_bytes(self.left);
        let r = sample_to_i16_bytes(self.right);

        [l[0],l[1],r[0],r[1]]
    }
}

impl From<[u8;6]> for Stereo {
    /// Converts the array of 6 bytes into a Stereo object.
    /// It is assumed that the bytes are 24-bit signed audio samples.
    /// 
    /// # Parameters
    /// 
    /// * `v` - The raw bytes to convert from.
    fn from(v:[u8;6]) -> Self {
        Stereo {
            left:  sample_from_i24_bytes([v[0],v[1],v[2]]),
            right: sample_from_i24_bytes([v[3],v[4],v[5]])
        }
    }
}
impl Into<[u8;6]> for Stereo {
    fn into(self) -> [u8;6] {
        let l = sample_to_i24_bytes(self.left);
        let r = sample_to_i24_bytes(self.right);

        [l[0],l[1],l[2],r[0],r[1],r[2]]
    }
}
