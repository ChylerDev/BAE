use super::*;

use std::rc::Rc;
use crate::core::*;
use super::basic_block::*;

#[derive(Clone)]
pub struct SimpleSound {
	block: BlockRc,
	input_gain: SampleT,
	output_gain: SampleT,
	channel: Option<SimpleChannelRc>,
	id: Option<usize>,
	is_muted: bool,
	is_paused: bool,
}

impl SimpleSound {
	pub fn new(input_gain: MathT, output_gain: MathT, block: BlockRc) -> Self {
		SimpleSound {
			block,
			input_gain: input_gain as SampleT,
			output_gain: output_gain as SampleT,
			channel: None,
			id: None,
			is_muted: false,
			is_paused: false,
		}
	}

	pub fn get_input_gain(&self) -> MathT {
		self.input_gain as MathT
	}

	pub fn get_output_gain(&self) -> MathT {
		self.output_gain as MathT
	}

	pub fn set_input_gain(&mut self, g: MathT) {
		self.input_gain = g as SampleT;
	}

	pub fn set_output_gain(&mut self, g: MathT) {
		self.output_gain = g as SampleT;
	}
}

impl Sound<SimpleSound> for SimpleSound {
	fn toggle_pause(&mut self) {
		self.is_paused = !self.is_paused;
	}

	fn is_paused(&self) -> bool {
		self.is_paused
	}

	fn toggle_mute(&mut self) {
		self.is_muted = !self.is_muted
	}

	fn is_muted(&self) -> bool {
		self.is_muted
	}

	fn register(this: SimpleSoundRc, mut channel: SimpleChannelRc) {		
		Self::unregister(this.clone());

		if let Some(sound) = Rc::get_mut(&mut this.clone()) {
			sound.channel = Some(channel.clone());
			if let Some(channel) = Rc::get_mut(&mut channel) {
				sound.id = Some(channel.add_sound(this));
			}
		}
	}

	fn unregister(mut this: SimpleSoundRc) {
		if this.id != None {
			if let Some(sound) = Rc::get_mut(&mut this) {
				if let Some(mut channel) = sound.channel.clone() {
					if let Some(channel) = Rc::get_mut(&mut channel) {
						if let Some(id) = sound.id {
							let _ = channel.remove_sound(id);
						}
					}
				}

				sound.channel = None;
				sound.id = None;
			}
		}
	}

	fn process(&mut self, mut input: StereoData) -> StereoData {
		if self.is_paused {
			return Default::default();
		}

		let out = if let Some(b) = Rc::get_mut(&mut self.block) {
			b.prime_input(input * self.input_gain);
			b.process() * self.output_gain
		} else {
			Default::default()
		};

		if self.is_muted {
			Default::default()
		} else {
			out
		}
	}
}

pub type SimpleSoundRc = Rc<SimpleSound>;
