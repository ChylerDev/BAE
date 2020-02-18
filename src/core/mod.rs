use super::*;
use std::rc::Rc;

pub trait Driver<S>
	where S: sounds::Sound<S> + Clone
{
	fn add_sound(&mut self, sound: Rc<S>) -> usize;
	fn remove_sound(&mut self, id: usize) -> Option<Rc<S>>;
	fn set_gain(&mut self, gain: MathT);
	fn get_output(&self) -> &TrackT;
	fn process(&mut self);
}

pub type DriverRc<S> = Rc<dyn Driver<S>>;

pub mod complex_driver;
pub use complex_driver::*;
