//! # Sound Channels
//! 
//! This module includes the types and traits needed to process a group of
//! [`Sound`]s to generate a single output sample.
//! 
//! [`Sound`]: ../sounds/sound/trait.Sound.html

use super::*;
use std::rc::Rc;

pub trait Channel<S>
	where S: sounds::Sound<S> + Clone
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

pub mod complex_channel;
pub mod simple_channel;

pub use complex_channel::*;
pub use simple_channel::*;
