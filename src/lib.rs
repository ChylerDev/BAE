//! # OCAE
//! 
//! The Overly Complicated Audio Engine.
//! A crate for processing audio samples for various uses.

use std::vec::Vec;

/// Type to perform infrequenty mathematic operations with when accuracy is needed.
pub type MathT = f64;
/// Type to calculate samples with.
pub type SampleT = f32;
/// Shorthand for a vector containing sample data.
pub type TrackT = std::vec::Vec<StereoData>;

/// The sampling rate the engine is set to run at.
pub const SAMPLE_RATE:u64 = 48_000;
/// The inverse of the sampling rate for easy referencing.
pub const INV_SAMPLE_RATE:MathT = 1.0/(SAMPLE_RATE as MathT);

pub mod stereodata;
pub use stereodata::*;

/// Converts a raw bytes to a Sample
/// It is assumed the bytes are 8-bit unsigned audio samples.
/// 
/// # Parameters
/// 
/// * `v` - The raw bytes to convert from.
pub fn sample_from_u8(v:[u8;1]) -> SampleT {
	(v[0] as SampleT - 128.0) / 128.0
}

/// Converts raw bytes to a Sample
/// It is assumed that the bytes are 16-bit signed audio samples.
/// 
/// # Parameters
/// 
/// * `v` - The raw bytes to convert from.
pub fn sample_from_i16(v:[u8;2]) -> SampleT {
	(i16::from_le_bytes(v) as SampleT) / (32768_f32)
}

/// Converts raw bytes to a Sample
/// It is assumed that the bytes are 24-bit signed audio samples.
/// 
/// # Parameters
/// 
/// * `v` - The raw bytes to convert from.
pub fn sample_from_i24(v:[u8;3]) -> SampleT {
	(i32::from_le_bytes([v[0],v[1],v[2],0]) as SampleT) / (32768_f32)
}

/// Converts a decibel value to a linear gain value.
/// This assumes that 0dB is unity gain, and ~-6bB is 0.5 gain
pub fn db_linear(db:MathT) -> MathT {
	10.0_f64.powf(db/20.0)
}

/// Converts a linear gain value to a decibel value.
/// This assumes that 0dB is unity gain, and ~-6bB is 0.5 gain
pub fn linear_db(g:MathT) -> MathT {
	20.0 * g.log10()
}

pub trait Name {
	fn get_name(&self) -> &str;
}

pub mod generators;
pub mod modifiers;
pub mod sounds;
pub mod tools;
