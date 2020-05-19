//! # Stereo
//! 
//! Module containing type for handling stereophonic audio data.

use super::*;
use crate::utils::*;

/// Type for a track of [`Stereo`] samples
/// 
/// [`Stereo`]: struct.Stereo.html
pub type StereoTrackT = Vec<Stereo>;

/// Struct representing a stereophonic audio sample.
#[derive(Debug,Copy,Clone,Default,PartialEq)]
#[repr(C)]
pub struct Stereo{
    /// Left sample value.
    pub left:  SampleT,
    /// Right sample value.
    pub right: SampleT,
}

impl Stereo {
    /// Returns a new Stereo object with default <0,0> values.
    pub fn new() -> Self {
        Stereo::default()
    }

    /// Returns a new Stereo object created from individual left and right
    /// audio samples.
    /// 
    /// # Parameters
    /// 
    /// * `l` - the left audio sample.
    /// * `r` - the right audio sample.
    pub fn from(l:SampleT, r:SampleT) -> Self {
        Stereo {
            left:  l,
            right: r,
        }
    }
}

impl SampleFormat for Stereo
{
    fn from_sample(x:SampleT) -> Self {
        Stereo {
            left:  x * SampleT::sqrt(0.5),
            right: x * SampleT::sqrt(0.5)
        }
    }

    fn into_sample(self) -> SampleT {
        (self.left + self.right)/SampleT::sqrt(0.5)
    }

    fn num_samples() -> usize {
        2
    }
}

/// Pans a given sample between the left and right channels. The panning
/// parameter `g` is a floating point value of the rang \[-1,1\], where -1 is 
/// panned full left and 1 is panned full right. If the given value is not
/// within this range, it is clamped to it.
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
            left:  (db_to_linear(l_lerp) * s as MathT) as SampleT,
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
            left:  (db_to_linear(l_lerp) * s as MathT) as SampleT,
            right: (db_to_linear(r_lerp) * s as MathT) as SampleT
        }
    }
}

impl std::ops::Neg for Stereo {
    type Output = Self;

    fn neg(self) -> Self::Output {
        Stereo {
            left:  -self.left,
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
    type Output = Stereo;

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
    fn from(s: SampleT) -> Self {
        Stereo::from_sample(s)
    }
}
impl Into<SampleT> for Stereo {
    fn into(self) -> SampleT {
        self.into_sample()
    }
}

impl TryFrom<Vec<u8>> for Stereo {
    type Error = String;

    fn try_from(v: Vec<u8>) -> Result<Self, Self::Error> {
        if v.len() < 2 {
            Err(format!("ERROR: Given vector was length {}. This function requires length 2.", v.len()))
        } else {
            Ok(
                Stereo {
                    left:  sample_from_u8(v[0]),
                    right: sample_from_u8(v[1]),
                }
            )
        }
    }
}
impl Into<Vec<u8>> for Stereo {
    fn into(self) -> Vec<u8> {
        vec![sample_to_u8(self.left), sample_to_u8(self.right)]
    }
}

impl TryFrom<Vec<i16>> for Stereo {
    type Error = String;

    fn try_from(v: Vec<i16>) -> Result<Self, Self::Error> {
        if v.len() < 2 {
            Err(format!("ERROR: Given vector was length {}. This function requires length 2.", v.len()))
        } else {
            Ok(
                Stereo {
                    left:  sample_from_i16(v[0]),
                    right: sample_from_i16(v[1]),
                }
            )
        }
    }
}
impl Into<Vec<i16>> for Stereo {
    fn into(self) -> Vec<i16> {
        vec![sample_to_i16(self.left), sample_to_i16(self.right)]
    }
}

impl TryFrom<Vec<i32>> for Stereo {
    type Error = String;

    fn try_from(v: Vec<i32>) -> Result<Self, Self::Error> {
        if v.len() < 2 {
            Err(format!("ERROR: Given vector was length {}. This function requires length 2.", v.len()))
        } else {
            Ok(
                Stereo {
                    left:  sample_from_i24(v[0]),
                    right: sample_from_i24(v[1]),
                }
            )
        }
    }
}
impl Into<Vec<i32>> for Stereo {
    fn into(self) -> Vec<i32> {
        vec![sample_to_i24(self.left), sample_to_i24(self.right)]
    }
}
