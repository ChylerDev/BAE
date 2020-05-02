//! # Sample Format
//!
//! Module containing different output formats like stereo, 2.1, 5.1, 7.1, etc.
//!
//! All functions that deal with converting raw bytes to numeric types assume
//! the bytes are in little-endian format.
//! 
//! As there is no i24 built-in type, i32 is used in it's place where
//! applicable. In most cases where a 24-bit sample is stored in a 32-bit data
//! type, the upper byte is ignored or explicitly set to 0.

use super::*;

pub mod mono;
pub mod stereo;
pub use mono::*;
pub use stereo::*;

use std::convert::TryFrom;
use std::ops::*;

/// Trait implementing the ability to perform math operations with a polyphonic
/// sample format and a monophonic sample.
/// 
/// # Dependencies:
/// 
/// * Most mathematical operators are required to be implemented to be able to
/// perform common operations on sample values.
/// * [`Mul`]/[`MulAssign`] is defined for both [`MathT`] as well as [`SampleT`]
/// for the convenience of common audio operations.
/// * [`From`]/[`Into`] implemented for [`SampleT`] - These functions should be
/// simple calls to [`from_mono`] and [`into_mono`] respectively.
/// * [`TryFrom`]/[`Into`] implemented for [`Vec<_>`] - These functions should
/// convert the sample values to the given standard integer types. As [`Vec`]s
/// are generic types, it cannot be assumed that any attempted conversions of
/// [`Vec`]s to a given sample format will succeed. Therefore those conversions
/// use [`TryFrom`] to indicate when there is an issue, which can be
/// communicated with the given [`String`] used for the error type. An example
/// of such an error could be (for the [`Stereo`] type):
/// 
/// ```
/// format!("ERROR: Given vector was length {}. This function requires length 2.", v.len())
/// ```
/// 
/// [`Mul`]: https://doc.rust-lang.org/std/ops/trait.Mul.html
/// [`MulAssign`]: https://doc.rust-lang.org/std/ops/trait.MulAssign.html
/// [`MathT`]: ../type.MathT.html
/// [`SampleT`]: ../type.SampleT.html
/// [`From`]: https://doc.rust-lang.org/std/convert/trait.From.html
/// [`Into`]: https://doc.rust-lang.org/std/convert/trait.Into.html
/// [`from_mono`]: #tymethod.from_mono
/// [`into_mono`]: #tymethod.into_mono
/// [`TryFrom`]: https://doc.rust-lang.org/std/convert/trait.TryFrom.html
/// [`Vec<_>`]: https://doc.rust-lang.org/std/vec/struct.Vec.html
/// [`Vec`]: https://doc.rust-lang.org/std/vec/struct.Vec.html
/// [`String`]: https://doc.rust-lang.org/std/convert/trait.From.html
/// [`Stereo`]: stereo/struct.Stereo.html
pub trait SampleFormat:
    Neg<Output=Self> +
    Add<        Self,  Output=Self> + AddAssign<   Self> +
    Sub<        Self,  Output=Self> + SubAssign<   Self> +
    Mul<        Self,  Output=Self> + MulAssign<   Self> +
    Mul<     SampleT,  Output=Self> + MulAssign<SampleT> +
    Mul<       MathT,  Output=Self> + MulAssign<  MathT> +
    From<                  SampleT> + Into<     SampleT> +
    TryFrom<Vec< u8>, Error=String> + Into<    Vec< u8>> +
    TryFrom<Vec<i16>, Error=String> + Into<    Vec<i16>> +
    TryFrom<Vec<i32>, Error=String> + Into<    Vec<i32>>
{
    /// Creates an object from a single monophonic sample.
    fn from_mono(x:SampleT) -> Self;

    /// Converts the given polyphonic sample to a monophonic sample.
    fn into_mono(self) -> SampleT;

    /// Returns the number of [`SampleT`] values held within a given
    /// [`SampleFormat`]. A common use for this would be for ensuring [`Vec`]s
    /// given to [`try_from`] have the correct size.
    /// 
    /// [`SampleT`]: ../type.SampleT.html
    /// [`SampleFormat`]: trait.SampleFormat.html
    /// [`Vec`]: https://doc.rust-lang.org/std/vec/struct.Vec.html
    /// [`try_from`]: https://doc.rust-lang.org/std/convert/trait.TryFrom.html#tymethod.try_from
    fn num_samples() -> usize;
}

/// Trait implementing the ability to pan a monophonic sample into a polyphonic
/// sample. This is generic for the polyphonic type and the type that defines
/// how it is panned. To see an implementation, see
/// [`Stereo::to_sample_format`].
///
/// [`Stereo::to_sample_format`]: stereo/struct.Stereo.html#method.to_sample_format
pub trait Panner<G>: SampleFormat {
    /// Converts the monophonic sample into a polyphonic sample.
    fn to_sample_format(s: SampleT, g: G) -> Self;
}

/// Converts a u8 8-bit sample to a `SampleT`.
pub fn sample_from_u8(v: u8) -> SampleT {
    (v as SampleT - 128.0) / 128.0
}
/// Converts a raw byte to a `SampleT`.
pub fn sample_from_u8_bytes(v:[u8;1]) -> SampleT {
    (v[0] as SampleT - 128.0) / 128.0
}

/// Converts a `SampleT` to an `u8`.
pub fn sample_to_u8(s: SampleT) -> u8 {
    (s * 128.0 + 128.0).round() as u8
}
/// Converts a `SampleT` to a raw little-endian byte.
pub fn sample_to_u8_bytes(s: SampleT) -> [u8;1] {
    [sample_to_u8(s)]
}

/// Converts an i16 16-bit sample to a `SampleT`.
pub fn sample_from_i16(v: i16) -> SampleT {
    v as SampleT / ((1 << 15) as SampleT - 1.0)
}
/// Converts raw bytes to a `SampleT`.
pub fn sample_from_i16_bytes(v:[u8;2]) -> SampleT {
    (i16::from_le_bytes(v) as SampleT) / ((1 << 15) as SampleT - 1.0)
}

/// Converts a `SampleT` to an `i16`.
pub fn sample_to_i16(s: SampleT) -> i16 {
    (s * ((1<<15) as SampleT - 1.0)).round() as i16
}
/// Converts a `SampleT` to raw little-endian bytes.
pub fn sample_to_i16_bytes(s: SampleT) -> [u8; 2] {
    sample_to_i16(s).to_le_bytes()
}

/// Converts an i32 24-bit sample to a `SampleT`.
pub fn sample_from_i24(v: i32) -> SampleT {
    v as SampleT / ((1 << 23) as SampleT - 1.0)
}
/// Converts raw bytes to a `SampleT`.
pub fn sample_from_i24_bytes(v:[u8;3]) -> SampleT {
    (i32::from_le_bytes([v[0],v[1],v[2],0]) as SampleT) / ((1 << 23) as SampleT - 1.0)
}

/// Converts a `SampleT` to an `i24`.
pub fn sample_to_i24(s: SampleT) -> i32 {
    (s * ((1<<23) as SampleT - 1.0)).round() as i32
}
/// Converts a `SampleT` to raw little-endian bytes.
pub fn sample_to_i24_bytes(s: SampleT) -> [u8; 3] {
    let i = sample_to_i24(s).to_le_bytes();

    [i[0],i[1],i[2]]
}
