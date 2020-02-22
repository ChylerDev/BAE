//! # Sound
//! 
//! Module containing the trait [`Sound`] defining the interface for anything
//! that will produce sound to be processed by a [`Channel`]. Two implementations
//! of this trait are provided in the form of [`ComplexSound`] and
//! [`SimpleSound`].
//! 
//! [`Sound`]: trait.Sound.html
//! [`Channel`]: ../../core/trait.Channel.html
//! [`ComplexSound`]: ../complex_sound/struct.ComplexSound.html
//! [`SimpleSound`]: ../simple_sound/struct.SimpleSound.html

use super::*;
use crate::core::*;
use std::rc::Rc;

/// This trait defines the interface that anything producing sound that will be
/// output to a [`Channel`] must define.
/// 
/// # Generic
/// 
/// To ensure a sound object can be cloned, this trait implements CRTP for the
/// implementing type. This allows constraints to be placed on the types that
/// can can implement [`Sound`], namely that they also implement [`Clone`].
/// 
/// [`Channel`]: ../../core/trait.Channel.html
/// [`Sound`]: trait.Sound.html
/// [`Clone`]: https://doc.rust-lang.org/std/clone/trait.Clone.html
pub trait Sound<S,C>
	where S: Clone + Sound<S,C>,
	      C: Clone + crate::core::Channel
{
	/// Toggles the pause state of the sound. If the sound is paused, the
	/// internal structures aren't process during a call to [`process`], instead
	/// only [`Default::default()`] is returned.
	/// 
	/// [`process`]: trait.Sound.html#tymethod.process
	/// [`Default::default()`]: https://doc.rust-lang.org/std/default/trait.Default.html#tymethod.default
	fn toggle_pause(&mut self);

	/// Returns the pause state of the sound.
	fn is_paused(&self) -> bool;

	/// Toggels the mute state of the sound. If the sound is muted, the internal
	/// structures are still processed during a call to [`process`], but 
	/// [`Default::default()`] is returned.
	/// 
	/// [`process`]: trait.Sound.html#tymethod.process
	/// [`Default::default()`]: https://doc.rust-lang.org/std/default/trait.Default.html#tymethod.default
	fn toggle_mute(&mut self);

	/// Returns the mute state of the sound.
	fn is_muted(&self) -> bool;

	/// Processes the sound and its internal structures, returning the resulting
	/// audio sample.
	/// 
	/// If the sound is paused, no processing of the internal structures is
	/// performed, instead only [`Default::default()`] is returned.
	/// 
	/// If the sound is muted, the internal structures are still processed, but
	/// [`Default::default()`] is still returned.
	/// 
	/// [`Default::default()`]: https://doc.rust-lang.org/std/default/trait.Default.html#tymethod.default
	fn process(&mut self, input: SampleT) -> SampleT;

	/// Registers a sound wrapped in an [`Rc`] with the provided [`Channel`],
	/// giving the [`Channel`] the ability to process the sound and retrieve
	/// samples for output.
	/// 
	/// In this function the [`Sound`] registers itself with the given
	/// [`Channel`]. Calling [`Channel::add_sound()`] yourself is not necessary.
	/// 
	/// If a [`Channel`] is already registered with this [`Sound`] then it will
	/// first unregister itself with [`unregister`].
	/// 
	/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
	/// [`Channel`]: ../../core/trait.Channel.html
	/// [`Channel::add_sound`]: ../../core/trait.Channel.html#tymethod.add_sound
	/// [`Sound`]: trait.Sound.html
	/// [`unregister`]: trait.Sound.html#tymethod.unregister
	fn register(this: Rc<S>, channel: SoundChannelRc<S,C>);

	/// Unregisters a sound wrapped in an [`Rc`] from its [`Channel`], freeing
	/// the [`Sound`] to be processed individually or allow registration with a
	/// different [`Channel`].
	/// 
	/// In this function the [`Sound`] unregisters itself from its [`Channel`].
	/// Calling [`Channel::remove_sound()`] yourself is not necessary.
	/// 
	/// If the [`Sound`] is not registered to any [`Channel`], then nothing
	/// happens.
	/// 
	/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
	/// [`Channel`]: ../../core/trait.Channel.html
	/// [`Channel::add_sound`]: ../../core/trait.Channel.html#tymethod.remove_sound
	/// [`Sound`]: trait.Sound.html
	/// [`unregister`]: trait.Sound.html#tymethod.unregister
	fn unregister(this: Rc<S>);
}

/// Type alias for a [`Sound`] wrapped in an [`Rc`]. Types implementing
/// [`Sound`] can define their own custom name for this type that is
/// interchangable, (e.g. [`ComplexSoundRc`] for [`ComplexSound`]).
/// 
/// [`Sound`]: trait.Sound.html
/// [`Rc`]:https://doc.rust-lang.org/std/rc/struct.Rc.html
/// [`ComplexSoundRc`]: ../complex_sound/type.ComplexSoundRc.html
/// [`ComplexSound`]: ../complex_sound/struct.ComplexSound.html
pub type SoundRc<S,C> = Rc<dyn Sound<S,C>>;
