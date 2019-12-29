//! # Generators
//! 
//! Module including many of the common types of basic sound types, including
//! sine, sawtooth, square, white noise, and more.

use super::*;

pub trait FreqMod {
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

pub mod sine;
pub mod noise;
pub mod sawtooth;
