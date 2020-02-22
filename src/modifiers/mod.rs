//! # Modifiers
//! 
//! Module including many of the common/basic filters including lowpass,
//! bandpass, echo, delay, etc.

use super::*;
use std::rc::Rc;

/// The `Modifier` trait defines types that filter audio samples.
/// 
/// There is a template parameter to allow for [CRTP], which is used to
/// guarantee that any type that implements [`Modifier`] also implements
/// [`Clone`].
/// 
/// [`Modifier`]: trait.Modifier.html
/// [`Clone`]: https://doc.rust-lang.org/std/clone/trait.Clone.html
/// [CRTP]: https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
pub trait Modifier<T>
	where T: Clone
{
	/// Filters the given audio sample.
	/// 
	/// # Parameters
	/// 
	/// * `x` - The "dry" audio sample before filtering.
	fn process(&mut self, x: SampleT) -> SampleT;
}

/// Type alias for a [`Modifier`] wrapped in an [`Rc`].
/// 
/// [`Modifier`]: trait.Modifier.html
/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
pub type ModifierRc<T> = Rc<dyn Modifier<T>>;

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
