//! # Sound Channels
//! 
//! This module includes the types and traits needed to process a group of
//! [`Sound`]s to generate a single output sample.
//! 
//! [`Sound`]: ../sounds/sound/trait.Sound.html

use super::*;
use std::rc::Rc;

pub trait Channel<S>
	where S: Clone + sounds::Sound<S>
{
	fn add_sound(&mut self, sound: Rc<S>) -> usize;
	fn remove_sound(&mut self, id: usize) -> Option<Rc<S>>;
	fn set_gain(&mut self, gain: MathT);
	fn get_output(&self) -> &TrackT;
}

pub trait BaseChannel {
	fn process(&mut self);
}

pub type ChannelRc<S> = Rc<dyn Channel<S>>;
pub type SimpleChannelRc = ChannelRc<crate::sounds::SimpleSound>;
pub type ComplexChannelRc = ChannelRc<crate::sounds::ComplexSound>;

pub mod standard_channel;
pub use standard_channel::*;
