//! # Driver

use super::*;

use std::rc::Rc;
use std::collections::HashMap;
use crate::sounds::sound::*;

#[derive(Clone)]
pub struct Driver {
	output: TrackT,
	sounds: HashMap<usize, Rc<sounds::Sound>>,
	gain: SampleT,
	id_counter: usize
}

impl Driver {
	pub fn new(track_size: usize, gain: MathT) -> Self {
		let mut d = Driver {
			output: TrackT::new(),
			sounds: HashMap::new(),
			gain: gain as SampleT,
			id_counter: 0
		};

		d.output.reserve(track_size);

		d
	}

	pub fn add_sound(&mut self, sound: SoundRc) -> usize {
		let id = self.get_id();
		self.sounds.insert(id, sound);

		id
	}

	pub fn remove_sound(&mut self, id: usize) -> Option<SoundRc> {
		self.sounds.remove(&id)
	}

	pub fn set_gain(&mut self, gain: MathT) {
		self.gain = gain as SampleT;
	}

	pub fn get_output(&self) -> &TrackT {
		&self.output
	}

	fn get_id(&mut self) -> usize {
		let old = self.id_counter;

		self.id_counter += 1;

		old
	}
}

pub type DriverRc = Rc<Driver>;
