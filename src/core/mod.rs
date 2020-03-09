//! # Sound Channels
//! 
//! This module includes the types and traits needed to process a group of
//! [`Sound`]s to generate a single output sample.
//! 
//! [`Sound`]: ../sounds/sound/trait.Sound.html

use super::*;
use std::rc::Rc;
use std::time::Duration;

/// Trait defining the simplest possible interface for a channel, with the
/// ability to process a batch of samples at a time.
pub trait Channel {
	/// Sets the amount of time [`process`] should calculate samples for. The
	/// given duration is truncated to a integer sample value.
	/// 
	/// [`process`]: trait.Channel.html#tymethod.process
	fn set_process_time(&mut self, d: Duration);

	/// Returns a reference to the internal track of samples.
	fn get_output(&self) -> &TrackT;

	/// Sets the gain of the output of the channel.
	fn set_gain(&mut self, gain: MathT);

	/// Processes the given number of samples, storing the results in the
	/// internal track of samples.
	fn process(&mut self);
}

/// Trait defining the interface for a channel which operates on a specific
/// sound type. The given channel must also implement the [`Channel`] trait.
/// 
/// [`Channel`]: trait.Channel.html
pub trait SoundChannel<S,C>
	where S: Clone + sounds::Sound<S>,
	      C: Clone + Channel
{
	/// Adds a sound to the driver for processing.
	/// 
	/// This function should generally only be called by the sound itself. This
	/// is to ensure that the sound and driver are in sync with their respective
	/// registrations.
	fn add_sound(&mut self, sound: Rc<S>) -> usize;

	/// Removes a sound from the driver.
	/// 
	/// This function should generally only be called by the sound itself. This
	/// is to ensure that the sound and driver are in sync with their respective
	/// registrations.
	fn remove_sound(&mut self, id: usize) -> Option<Rc<S>>;
}

/// Alias for a [`Channel`] wrapped in an [`Rc`].
/// 
/// [`Channel`]: trait.Channel.html
/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
pub type ChannelRc = Rc<dyn Channel>;

/// Alias for a [`SoundChannel`] wrapped in an [`Rc`].
/// 
/// [`SoundChannel`]: trait.SoundChannel.html
/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
pub type SoundChannelRc<S,C> = Rc<dyn SoundChannel<S,C>>;

/// Alias for a [`SoundChannelRc`] templated for [`SimpleSound`].
/// 
/// [`SoundChannelRc`]: type.SoundChannelRc.html
/// [`SimpleSound`]: ../sounds/simple_sound/struct.SimpleSound.html
pub type SimpleSoundChannelRc<C> = SoundChannelRc<crate::sounds::SimpleSound,C>;

/// Alias for a [`SoundChannelRc`] templated for [`ComplexSound`].
/// 
/// [`SoundChannelRc`]: type.SoundChannelRc.html
/// [`SimpleSound`]: ../sounds/complex_sound/struct.ComplexSound.html
pub type ComplexSoundChannelRc<C> = SoundChannelRc<crate::sounds::ComplexSound,C>;

pub mod standard_channel;
pub use standard_channel::*;
