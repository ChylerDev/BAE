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
pub const SAMPLE_RATE:u64 = 48000;
/// The inverse of the sampling rate for easy referencing.
pub const INV_SAMPLE_RATE:MathT = 1.0/(SAMPLE_RATE as MathT);

#[derive(Debug,Default,Copy,Clone)]
/// Struct representing a stereophonic audio sample
pub struct StereoData{
	left:SampleT,
	right:SampleT,
}

impl StereoData {
	/// Returns a new StereoData object created from individual left and right
	/// audio samples.
	/// 
	/// # Parameters
	/// 
	/// * `l` - the left audio sample.
	/// * `r` - the right audio sapmle.
	pub fn from(l:SampleT, r:SampleT) -> StereoData {
		StereoData{
			left:l,
			right:r
		}
	}

	/// Creates a new StereoData object from a single monophonic sample. This
	/// function reduces the power of the given sample by half to reflect human
	/// hearing.
	/// 
	/// # Parameters
	/// 
	/// * `x` - the input sample.
	pub fn from_mono(x:SampleT) -> StereoData {
		StereoData{
			left: SampleT::sqrt(0.5)*x,
			right: SampleT::sqrt(0.5)*x
		}
	}

	/// Converts the given stereophonic sample to a monophonic sample by summing
	/// the left and right samples and dividing by half power to get the full
	/// power monophonic sample.
	pub fn as_mono(&self) -> SampleT {
		(self.left + self.right)/SampleT::sqrt(0.5)
	}

	pub fn left(&self) -> SampleT {
		self.left
	}

	pub fn right(&self) -> SampleT {
		self.right
	}
}

impl Into<Vec<u8>> for StereoData {
	/// Converts the StereoData into a vector of bytes.
	fn into(self) -> Vec<u8> {
		let mut v = Vec::new();

			// Converts the left sample from SampleT (f32) to i16, then to bytes
		let n = ((self.left * 0x8000 as SampleT) as i16).to_le_bytes();
		v.push(n[0]);
		v.push(n[1]);

			// Converts the right sample from SampleT (f32) to i16, then to bytes
		let n = ((self.right * 0x8000 as SampleT) as i16).to_le_bytes();
		v.push(n[0]);
		v.push(n[1]);

		v
	}
}

pub mod generators;
pub mod tools;
