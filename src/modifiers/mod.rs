//! # Modifiers
//! 
//! Module including many of the common/basic filters including lowpass,
//! bandpass, echo, delay, etc.

use super::*;

pub trait Modifier {
	/// Filters the given audio sample.
	/// 
	/// # Parameters
	/// 
	/// * `x` - The "dry" audio sample before filtering.
	fn process(&mut self, x: StereoData) -> StereoData;
}

pub mod adsr;
