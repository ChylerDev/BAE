//! # Sample Format
//! 
//! Module containing different output formats like stereo, 2.1, 5.1, 7.1, etc.

use super::*;

pub mod stereodata;
pub use stereodata::*;

/// Trait implementing the ability to pan a monophonic sample into a ployphonic
/// sample. This is generic for the polyphonic type and the type that defines
/// how it is panned. To see an implementation, see
/// [`StereoData::to_sample_format`].
/// 
/// [`StereoData::to_sample_format`]: stereodata/struct.StereoData.html#method.to_sample_format
pub trait Panner<SF,G>
	where SF: Panner<SF,G>,
	      G: Sized
{
	/// Converts the monophonic sample into a polyphonic sample.
	fn to_sample_format(s: SampleT, g: G) -> SF;
}

/// Converts a u8 8-bit sample to a `SampleT`.
pub fn sample_from_u8(v: u8) -> SampleT {
	(v as SampleT - 128.0) / 128.0
}

/// Converts a raw byte to a `SampleT`.
pub fn sample_from_u8_bytes(v:[u8;1]) -> SampleT {
	(v[0] as SampleT - 128.0) / 128.0
}

/// Converts an i16 16-bit sample to a `SampleT`.
pub fn sample_from_i16(v: i16) -> SampleT {
	v as SampleT / ((1 << 15) as SampleT - 1.0)
}

/// Converts raw bytes to a `SampleT`.
pub fn sample_from_i16_bytes(v:[u8;2]) -> SampleT {
	(i16::from_le_bytes(v) as SampleT) / ((1 << 15) as SampleT - 1.0)
}

/// Converts an i32 24-bit sample to a `SampleT`.
pub fn sample_from_i24(v: i32) -> SampleT {
	v as SampleT / ((1 << 23) as SampleT - 1.0)
}

/// Converts raw bytes to a `SampleT`.
pub fn sample_from_i24_bytes(v:[u8;3]) -> SampleT {
	(i32::from_le_bytes([v[0],v[1],v[2],0]) as SampleT) / ((1 << 23) as SampleT - 1.0)
}
