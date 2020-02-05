//! # Generators
//! 
//! Module including many of the common/basic sound types, including sine,
//! sawtooth, square, white noise, and more.

use super::*;

pub trait FreqMod {
	/// Creates a new object for the given frequency.
	/// 
	/// # Parameters
	/// 
	/// * `f` - The frequency for the new object
	fn new(f: MathT) -> Self;

	/// Sets the frequency of the given object.
	/// 
	/// # Parameters
	/// 
	/// * `f` - The new frequency.
	fn set_frequency(&mut self, f: MathT);

	/// Gets the current frequency of the given object.
	fn get_frequency(&self) -> MathT;
}

pub trait Generator {
	/// Generates a rendered audio sample
	fn process(&mut self) -> StereoData;
}

pub mod empty;
pub mod noise;
pub mod sawtooth;
pub mod sine;
pub mod square;
pub mod triangle;
pub mod wavplayer;

pub use empty::*;
pub use noise::*;
pub use sawtooth::*;
pub use sine::*;
pub use square::*;
pub use triangle::*;
pub use wavplayer::*;
