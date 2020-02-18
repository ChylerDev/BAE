use super::*;
use crate::core::*;
use std::rc::Rc;

pub trait Sound<S>
	where S: Clone
{
	fn toggle_pause(&mut self);
	fn is_paused(&self) -> bool;
	fn toggle_mute(&mut self);
	fn is_muted(&self) -> bool;
	fn process(&mut self, input: StereoData) -> StereoData;
	fn register(this: Rc<S>, driver: DriverRc<S>);
	fn unregister(this: Rc<S>);
}

pub type SoundRc<S> = Rc<dyn Sound<S>>;
