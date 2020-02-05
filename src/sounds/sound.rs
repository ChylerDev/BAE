//! # Sound

use super::*;
use std::rc::Rc;
use std::collections::{HashMap, VecDeque};
use crate::driver::*;

pub type BlockList = VecDeque<BlockRc>;
pub type Graph = HashMap<BlockRc, BlockList>;

pub struct Sound {
	graph: Graph,
	process_order: BlockList,
	input_gain: BlockRc,
	output_gain: BlockRc,
	driver: Option<DriverRc>,
	id: usize,
	is_paused: bool
}

impl Sound {
	pub fn new(input_gain: MathT, output_gain: MathT) -> Self {
		Sound {
			graph: Graph::new(),
			process_order: BlockList::new(),
			input_gain: BlockRc::new(
				Rc::new(
					Block::from_modifier(
						modifiers::Gain::new(input_gain as SampleT)
					)
				)
			),
			output_gain: BlockRc::new(
				Rc::new(
					Block::from_modifier(
						modifiers::Gain::new(output_gain as SampleT)
					)
				)
			),
			driver: None,
			id: usize::max_value(),
			is_paused: false
		}
	}
}

pub type SoundRc = Rc<Sound>;
