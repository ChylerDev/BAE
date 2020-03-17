//! # Sounds
//! 
//! This module includes the types needed to represent a collection of
//! [`Generator`]s and [`Modifier`]s in an abstract, simple way.
//! 
//! [`Generator`]: ../generators/trait.Generator.html
//! [`Modifier`]: ../modifiers/trait.Modifier.html

use super::*;
use std::rc::Rc;

pub mod standard_block;
pub mod complex_sound;
pub mod simple_sound;

pub use standard_block::*;
pub use complex_sound::*;
pub use simple_sound::*;

/// Trait used for generalizing the interface that allows for the processing of
/// multiple audio objects (be they [`Generator`]s or [`Modifier`]s or both).
/// 
/// [`Generator`]: ../../generators/trait.Generator.html
/// [`Modifier`]: ../../modifiers/trait.Modifier.html
pub trait Block {
    /// Increments the internal input sample by the given sample.
    fn prime_input(&mut self, x: SampleT);

    /// Process the [`Block`]. Individually processes the stored [`Generator`]
    /// and [`Modifier`] objects which are both combined using the [`Inter`] and
    /// returned.
    /// 
    /// [`Block`]: trait.Block.html
    /// [`Generator`]: ../../generators/trait.Generator.html
    /// [`Modifier`]: ../../modifiers/trait.Modifier.html
    /// [`Inter`]: type.Inter.html
    fn process(&mut self) -> SampleT;
}

/// This trait defines the interface that anything producing sound that will be
/// output to a [`Channel`] must define.
/// 
/// [`Channel`]: ../../core/trait.Channel.html
pub trait Sound {
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

    /// Sets itself as registered with the given ID.
    /// 
    /// Caution should be taken when registering and unregistering sounds to or
    /// from a [`Channel`], as [`Sound`]s don't control their own registration.
    /// As such you should be registering through [`Channel::add_sound`].
    /// 
    /// [`Channel`]: ../../core/trait.Channel.html
    /// [`Channel::add_sound`]: ../../core/trait.Channel.html#tymethod.add_sound
    /// [`Sound`]: trait.Sound.html
    /// [`Sound::unregister`]: trait.Sound.html#tymethod.unregister
    fn register(&mut self, id: usize);

    /// Sets itself as unregistered and clears the saved ID.
    /// 
    /// Caution should be taken when registering and unregistering sounds to or
    /// from a [`Channel`] as [`Sound`]s don't control their own registration.
    /// As such you should be registering through [`Channel::remove_sound`].
    /// 
    /// [`Channel`]: ../../core/trait.Channel.html
    /// [`Channel::remove_sound`]: ../../core/trait.Channel.html#tymethod.remove_sound
    /// [`Sound`]: trait.Sound.html
    fn unregister(&mut self);

    /// Returns the ID given to the [`Sound`] during registration with a
    /// [`Channel`]. If the [`Sound`] is unregistered, it will return `None`.
    /// 
    /// [`Channel`]: ../../core/trait.Channel.html
    /// [`Sound`]: trait.Sound.html
    fn get_id(&self) -> Option<usize>;
}

/// Alias for an [`Rc`]-wrapped [`Block`] object.
/// 
/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
/// [`Block`]: trait.Block.html
pub type BasicBlockRc = Rc<dyn Block>;

/// Type alias for a [`Sound`] wrapped in an [`Rc`]. Types implementing
/// [`Sound`] can define their own custom name for this type that is
/// interchangable, (e.g. [`ComplexSoundRc`] for [`ComplexSound`]).
/// 
/// [`Sound`]: trait.Sound.html
/// [`Rc`]:https://doc.rust-lang.org/std/rc/struct.Rc.html
/// [`ComplexSoundRc`]: ../complex_sound/type.ComplexSoundRc.html
/// [`ComplexSound`]: ../complex_sound/struct.ComplexSound.html
pub type SoundRc = Rc<dyn Sound>;
