//! # BAE_Audio
//! 
//! The Overly Complicated Audio Engine.
//! A crate for processing audio samples for various uses.

#![warn(missing_docs)]

use std::vec::Vec;

/// Type to perform infrequenty mathematic operations with when accuracy is needed.
pub type MathT = f64;
/// Type to calculate samples with.
pub type SampleT = f32;
/// Shorthand for a vector containing sample data.
pub type TrackT = Vec<SampleT>;

/// The sampling rate the engine is set to run at.
pub const SAMPLE_RATE:u64 = 48_000;
/// The inverse of the sampling rate for easy referencing.
pub const INV_SAMPLE_RATE:MathT = 1.0/(SAMPLE_RATE as MathT);

/// Linear interpolation of a given value.
pub fn lerp<T>(x:T, x1:T, x2:T, y1:T, y2:T) -> T
	where T: Copy + Sized + std::ops::Add<Output=T> + std::ops::Sub<Output=T> + std::ops::Mul<Output=T> + std::ops::Div<Output=T>
{
	((y2 - y1) / (x2 - x1)) * (x - x1) + y1
}

/// Trait that implements the ability to get the name of a type simply.
#[cfg(test)]
pub trait Name {
	/// Returns a string slice for the name of the type.
	fn get_name(&self) -> &str;
}

pub mod core;
pub mod generators;
pub mod modifiers;
pub mod sounds;
pub mod sample_format;
pub mod tools;

pub use sample_format::*;
