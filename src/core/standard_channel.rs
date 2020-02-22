//! # Standard Channel

use super::*;

use std::rc::Rc;
use std::collections::HashMap;
use crate::sounds::*;

/// Standard implementation of the [`Channel`] trait.
/// 
/// [`Channel`]: ../trait.Channel.html
#[derive(Clone)]
pub struct StandardChannel<S>
	where S: Clone + Sound<S,StandardChannel<S>>
{
	output: TrackT,
	sounds: HashMap<usize, Rc<S>>,
	gain: SampleT,
	id_counter: usize
}

impl<S> StandardChannel<S>
	where S: Clone + Sound<S,StandardChannel<S>>
{
	/// Creates a new channel with the given gain.
	/// 
	/// The internal track is initialized for 10ms' worth of samples. Call
	/// [`set_process_time`] to change this.
	/// 
	/// [`set_process_time`]: ../trait.Channel.html#tymethod.set_process_time
	pub fn new(gain: MathT) -> Self {
		StandardChannel {
			output: TrackT::with_capacity((0.01 * SAMPLE_RATE as MathT) as usize),
			sounds: HashMap::new(),
			gain: gain as SampleT,
			id_counter: 0
		}
	}

	fn get_id(&mut self) -> usize {
		let old = self.id_counter;

		self.id_counter += 1;

		old
	}
}

impl<S> SoundChannel<S,StandardChannel<S>> for StandardChannel<S>
	where S: Clone + Sound<S,StandardChannel<S>>
{
	fn add_sound(&mut self, sound: Rc<S>) -> usize {
		let id = self.get_id();
		self.sounds.insert(id, sound);

		id
	}

	fn remove_sound(&mut self, id: usize) -> Option<Rc<S>> {
		self.sounds.remove(&id)
	}
}

impl<S> Channel for StandardChannel<S>
	where S: Clone + Sound<S,StandardChannel<S>>
{
	fn set_process_time(&mut self, d: Duration) {
		self.output = TrackT::with_capacity((d.as_secs_f64() * SAMPLE_RATE as MathT) as usize);
	}

	fn get_output(&self) -> &TrackT {
		&self.output
	}

	fn set_gain(&mut self, gain: MathT) {
		self.gain = gain as SampleT;
	}

	fn process(&mut self) {
		self.output.resize_with(self.output.len(), Default::default);

		for sample in &mut self.output {
			for mut sound in &mut self.sounds {
				*sample += Rc::get_mut(&mut sound.1).unwrap().process(Default::default());
			}

			*sample *= self.gain;
		}
	}
}
