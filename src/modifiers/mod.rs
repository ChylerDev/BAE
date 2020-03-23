//! # Modifiers
//! 
//! Module including many of the common/basic filters including lowpass,
//! bandpass, echo, delay, etc.

use super::*;
use std::sync::Arc;

pub mod adsr;
pub mod bandpass;
pub mod delay;
pub mod echo;
pub mod passthrough;
pub mod envelope;
pub mod gain;
pub mod generic;
pub mod highpass;
pub mod lowpass;

pub use adsr::*;
pub use bandpass::*;
pub use delay::*;
pub use echo::*;
pub use passthrough::*;
pub use envelope::*;
pub use gain::*;
pub use generic::*;
pub use highpass::*;
pub use lowpass::*;

/// The `Modifier` trait defines types that filter audio samples.
/// 
/// [`Modifier`]: trait.Modifier.html
/// [`Clone`]: https://doc.rust-lang.org/std/clone/trait.Clone.html
pub trait Modifier
{
    /// Filters the given audio sample.
    /// 
    /// # Parameters
    /// 
    /// * `x` - The "dry" audio sample before filtering.
    fn process(&mut self, x: SampleT) -> SampleT;
}

/// Type alias for a [`Modifier`] wrapped in a smart pointer.
/// 
/// [`Modifier`]: trait.Modifier.html
pub type ModifierSP = Arc<dyn Modifier>;
