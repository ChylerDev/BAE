//! # Stereo Data
//! 
//! Module containing type for handling stereo audio data.

use super::*;
use crate::utils::*;

/// Struct representing a stereophonic audio sample.
#[derive(Copy,Clone,Default)]
pub struct Stereo{
    /// Left sample value.
    pub left:SampleT,
    /// Right sample value.
    pub right:SampleT,
}

impl Stereo {
    /// Returns a new Stereo object created from individual left and right
    /// audio samples.
    /// 
    /// # Parameters
    /// 
    /// * `l` - the left audio sample.
    /// * `r` - the right audio sapmle.
    pub fn from_stereo(l:SampleT, r:SampleT) -> Stereo {
        Stereo{
            left:l,
            right:r
        }
    }

    /// Returns a new Stereo object where both left and right channels are
    /// copied from the passed sample value.
    pub fn single_stereo(x:SampleT) -> Stereo {
        Stereo{
            left:x,
            right:x
        }
    }

    /// Creates a new Stereo object from a single monophonic sample. This
    /// function reduces the power of the given sample by half to reflect human
    /// hearing.
    /// 
    /// # Parameters
    /// 
    /// * `x` - the input sample.
    pub fn from_mono(x:SampleT) -> Stereo {
        Stereo{
            left: SampleT::sqrt(0.5)*x,
            right: SampleT::sqrt(0.5)*x
        }
    }

    /// Converts the given stereophonic sample to a monophonic sample by summing
    /// the left and right samples and dividing by half power to get the full
    /// power monophonic sample.
    pub fn as_mono(self) -> SampleT {
        (self.left + self.right)/SampleT::sqrt(0.5)
    }
}

impl Panner<Stereo, f32> for Stereo {
    fn to_sample_format(s: SampleT, g: f32) -> Stereo {
        let l_lerp = utils::lerp(g, -1.0, 1.0, 0.0, -120.0);
        let r_lerp = utils::lerp(g, -1.0, 1.0, -120.0, 0.0);

        Stereo {
            left: db_to_linear(l_lerp as MathT) as SampleT * s,
            right: db_to_linear(r_lerp as MathT) as SampleT * s
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
impl std::ops::Mul<Stereo> for SampleT {
    type Output = Stereo;

    fn mul(self, rhs: Stereo) -> Self::Output {
        Stereo {
            left: self * rhs.left,
            right: self * rhs.right,
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
impl std::ops::Mul<Stereo> for MathT {
    type Output = Stereo;

    fn mul(self, rhs: Stereo) -> Self::Output {
        Stereo {
            left: self as SampleT * rhs.left,
            right: self as SampleT * rhs.right
        }
    }
}

impl std::ops::MulAssign<MathT> for Stereo {
    fn mul_assign(&mut self, rhs: MathT) {
        self.left *= rhs as SampleT;
        self.right *= rhs as SampleT;
    }
}

impl Into<Vec<u8>> for Stereo {
    /// Converts the Stereo into a vector of bytes.
    fn into(self) -> Vec<u8> {
        let mut v = Vec::new();

            // Converts the left sample from SampleT (f32) to i16, then to bytes
        let n = ((self.left * 32768_f32) as i16).to_le_bytes();
        v.push(n[0]);
        v.push(n[1]);

            // Converts the right sample from SampleT (f32) to i16, then to bytes
        let n = ((self.right * 32768_f32) as i16).to_le_bytes();
        v.push(n[0]);
        v.push(n[1]);

        v
    }
}

impl From<SampleT> for Stereo {
    /// Copies the given sample to the left and right channels. If you want to
    /// use the half-power conversion, use [`Stereo::from_mono`].
    /// 
    /// [`Stereo::from_mono`]: struct.Stereo.html#method.from_mono
    fn from(s: SampleT) -> Self {
        Stereo{
            left: s,
            right: s,
        }
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
