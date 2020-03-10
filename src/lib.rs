//! # bae_rs
//! 
//! The Broad Audio Engine presents the ability to process sounds and wav files
//! for applications like video games and DAWs.

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
/// The inverse of the sampling rate for easy referencing and calculating.
pub const INV_SAMPLE_RATE:MathT = 1.0/(SAMPLE_RATE as MathT);

pub mod channels;
pub mod generators;
pub mod modifiers;
pub mod sounds;
pub mod sample_format;
pub mod tools;

pub use sample_format::*;
