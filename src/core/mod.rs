//! # Sound Channels
//! 
//! This module includes the types and traits needed to process a group of
//! [`Sound`]s to generate a single output sample.
//! 
//! [`Sound`]: ../sounds/sound/trait.Sound.html

use super::*;
use std::rc::Rc;

pub trait Channel {
	fn process(&mut self);
}

/// Trait defining the interface for a channel which operates on a specific
/// sound type.
pub trait SoundChannel<S,C>
	where S: Clone + sounds::Sound<S,C>,
	      C: Clone + Channel
{
	fn add_sound(&mut self, sound: Rc<S>) -> usize;
	fn remove_sound(&mut self, id: usize) -> Option<Rc<S>>;
	fn set_gain(&mut self, gain: MathT);
	fn get_output(&self) -> &TrackT;
}

pub type SoundChannelRc<S,C> = Rc<dyn SoundChannel<S,C>>;
pub type SimpleSoundChannelRc<C> = SoundChannelRc<crate::sounds::SimpleSound<C>,C>;
pub type ComplexSoundChannelRc<C> = SoundChannelRc<crate::sounds::ComplexSound<C>,C>;

pub type ChannelRc = Rc<dyn Channel>;

pub mod standard_channel;
pub use standard_channel::*;
