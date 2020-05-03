//! # Mono
//! 
//! Module containing type for handling monophonic audio data.

use super::*;

pub type MonoTrackT = Vec<Mono>;

/// Struct representing a monophonic audio sample.
#[derive(Copy,Clone,Default)]
pub struct Mono {
    /// The single, monophonic sample.
    pub mono: SampleT,
}

impl Mono {
    /// Creates a new Mono object with a default value of 0.
    pub fn new() -> Self {
        Mono::default()
    }

    /// Creates a new Mono object from the given sample value.
    pub fn from(s: SampleT) -> Self {
        Mono {
            mono: s
        }
    }
}

impl SampleFormat for Mono {
    fn from_sample(x: SampleT) -> Self {
        Mono {
            mono: x
        }
    }

    fn into_sample(self) -> SampleT {
        self.mono
    }

    fn num_samples() -> usize {
        1
    }
}

impl<T> Panner<T> for Mono {
    fn to_sample_format(s: SampleT, _: T) -> Self {
        Mono {
            mono: s
        }
    }
}

use std::ops::*;

impl Neg for Mono {
    type Output = Self;

    fn neg(self) -> Self::Output {
        Mono {
            mono: -self.mono
        }
    }
}

impl Add<Mono> for Mono {
    type Output = Self;

    fn add(self, rhs: Mono) -> Self::Output {
        Mono {
            mono: self.mono + rhs.mono
        }
    }
}
impl AddAssign<Mono> for Mono {
    fn add_assign(&mut self, rhs: Mono) {
        self.mono += rhs.mono;
    }
}

impl Sub<Mono> for Mono {
    type Output = Self;

    fn sub(self, rhs: Mono) -> Self {
        Mono {
            mono: self.mono + rhs.mono
        }
    }
}
impl SubAssign<Mono> for Mono {
    fn sub_assign(&mut self, rhs: Mono) {
        self.mono -= rhs.mono;
    }
}

impl Mul<Mono> for Mono {
    type Output = Mono;

    fn mul(self, rhs: Mono) -> Self::Output {
        Mono {
            mono: self.mono * rhs.mono
        }
    }
}
impl MulAssign<Mono> for Mono {
    fn mul_assign(&mut self, rhs: Mono) {
        self.mono *= rhs.mono;
    }
}

impl Mul<SampleT> for Mono {
    type Output = Mono;

    fn mul(self, rhs: SampleT) -> Self::Output {
        Mono {
            mono: self.mono * rhs
        }
    }
}
impl MulAssign<SampleT> for Mono {
    fn mul_assign(&mut self, rhs: SampleT) {
        self.mono *= rhs;
    }
}

impl Mul<MathT> for Mono {
    type Output = Mono;

    fn mul(self, rhs: MathT) -> Self::Output {
        Mono {
            mono: (self.mono as MathT * rhs) as SampleT
        }
    }
}
impl MulAssign<MathT> for Mono {
    fn mul_assign(&mut self, rhs: MathT) {
        self.mono = (self.mono as MathT * rhs) as SampleT;
    }
}

impl From<SampleT> for Mono {
    fn from(s: SampleT) -> Self {
        Mono::from_sample(s)
    }
}
impl Into<SampleT> for Mono {
    fn into(self) -> SampleT {
        Mono::into_sample(self)
    }
}

impl TryFrom<Vec<u8>> for Mono {
    type Error = String;

    fn try_from(v: Vec<u8>) -> Result<Self, Self::Error> {
        if v.len() < 1 {
            Err(format!("ERROR: Given vector was length {}. This function requires length 1.", v.len()))
        } else {
            Ok(
                Mono {
                    mono: sample_from_u8(v[0])
                }
            )
        }
    }
}
impl Into<Vec<u8>> for Mono {
    fn into(self) -> Vec<u8> {
        vec![sample_to_u8(self.mono)]
    }
}

impl TryFrom<Vec<i16>> for Mono {
    type Error = String;

    fn try_from(v: Vec<i16>) -> Result<Self, Self::Error> {
        if v.len() < 1 {
            Err(format!("ERROR: Given vector was length {}. This function requires length 1.", v.len()))
        } else {
            Ok(
                Mono {
                    mono: sample_from_i16(v[0])
                }
            )
        }
    }
}
impl Into<Vec<i16>> for Mono {
    fn into(self) -> Vec<i16> {
        vec![sample_to_i16(self.mono)]
    }
}

impl TryFrom<Vec<i32>> for Mono {
    type Error = String;

    fn try_from(v: Vec<i32>) -> Result<Self, Self::Error> {
        if v.len() < 1 {
            Err(format!("ERROR: Given vector was length {}. This function requires length 1.", v.len()))
        } else {
            Ok(
                Mono {
                    mono: sample_from_i24(v[0])
                }
            )
        }
    }
}
impl Into<Vec<i32>> for Mono {
    fn into(self) -> Vec<i32> {
        vec![sample_to_i24(self.mono)]
    }
}
