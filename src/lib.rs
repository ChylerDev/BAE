//! # OCAE
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
pub type TrackT = std::vec::Vec<StereoData>;

/// The sampling rate the engine is set to run at.
pub const SAMPLE_RATE:u64 = 48_000;
/// The inverse of the sampling rate for easy referencing.
pub const INV_SAMPLE_RATE:MathT = 1.0/(SAMPLE_RATE as MathT);

/// Trait that implements the ability to get the name of a type simply.
pub trait Name {
	/// Returns a string slice for the name of the type.
	fn get_name(&self) -> &str;
}

pub mod driver;
pub mod generators;
pub mod modifiers;
pub mod sounds;
pub mod stereodata;
pub mod tools;

pub use stereodata::*;
