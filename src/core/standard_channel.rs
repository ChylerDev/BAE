//! # Standard Channel

use super::*;

use std::rc::Rc;
use std::collections::HashMap;
use crate::sounds::*;

#[derive(Clone)]
pub struct StandardChannel {
	output: TrackT,
	sounds: HashMap<usize, ComplexSoundRc>,
	gain: SampleT,
	id_counter: usize
}

impl StandardChannel {
	pub fn new(track_size: usize, gain: MathT) -> Self {
		StandardChannel {
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

impl Channel<ComplexSound> for StandardChannel {
	fn add_sound(&mut self, sound: ComplexSoundRc) -> usize {
		let id = self.get_id();
		self.sounds.insert(id, sound);

		id
	}

	fn remove_sound(&mut self, id: usize) -> Option<ComplexSoundRc> {
		self.sounds.remove(&id)
	}

	fn set_gain(&mut self, gain: MathT) {
		self.gain = gain as SampleT;
	}

	fn get_output(&self) -> &TrackT {
		&self.output
	}
}

impl BaseChannel for StandardChannel {
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

pub type ComplexChannelRc = ChannelRc<ComplexSound>;
