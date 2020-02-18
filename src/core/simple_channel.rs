use super::*;

use std::rc::Rc;
use std::collections::HashMap;
use crate::sounds::*;

#[derive(Clone)]
pub struct SimpleChannel {
	output: TrackT,
	sounds: HashMap<usize, SimpleSoundRc>,
	gain: SampleT,
	id_counter: usize
}

impl SimpleChannel {
	pub fn new(track_size: usize, gain: MathT) -> Self {
		SimpleChannel {
			output: TrackT::with_capacity(track_size),
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

impl Channel<SimpleSound> for SimpleChannel {
	fn add_sound(&mut self, sound: SimpleSoundRc) -> usize {
		let id = self.get_id();
		self.sounds.insert(id, sound);

		id
	}

	fn remove_sound(&mut self, id: usize) -> Option<SimpleSoundRc> {
		self.sounds.remove(&id)
	}

	fn set_gain(&mut self, gain: MathT) {
		self.gain = gain as SampleT;
	}

	fn get_output(&self) -> &TrackT {
		&self.output
	}
}

impl BaseChannel for SimpleChannel {
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

pub type SimpleChannelRc = ChannelRc<SimpleSound>;
