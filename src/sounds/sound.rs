//! # Sound

use super::*;
use std::rc::Rc;
use std::collections::VecDeque;
use crate::driver::*;
use super::basic_block::*;
use petgraph::graph;

pub type BlockList = VecDeque<BlockRc>;
pub type Graph = graph::DiGraph<BlockRc, ()>;
pub type GraphNode = graph::NodeIndex;

pub struct Sound {
	graph: Graph,
	input_gain: GraphNode,
	output_gain: GraphNode,
	driver: Option<DriverRc>,
	id: Option<usize>,
	is_muted: bool,
	is_paused: bool
}

impl Sound {
	pub fn new(input_gain: MathT, output_gain: MathT) -> Self {
		let mut graph = Graph::new();
		let input_gain = graph.add_node(
			Rc::new(
				Block::from_modifier(
					modifiers::Gain::new(input_gain as SampleT)
				)
			)
		);
		let output_gain = graph.add_node(
			Rc::new(
				Block::from_modifier(
					modifiers::Gain::new(output_gain as SampleT)
				)
			)
		);

		Sound {
			graph,
			input_gain,
			output_gain,
			driver: None,
			id: None,
			is_muted: false,
			is_paused: false
		}
	}

	pub fn get_input_gain(&self) -> GraphNode {
		self.input_gain.clone()
	}

	pub fn get_output_gain(&self) -> GraphNode {
		self.output_gain.clone()
	}

	pub fn toggle_pause(&mut self) {
		self.is_paused = !self.is_paused;
	}

	pub fn is_paused(&self) -> bool {
		self.is_paused
	}

	pub fn toggle_mute(&mut self) {
		self.is_muted = !self.is_muted;
	}

	pub fn is_muted(&self) -> bool {
		self.is_muted
	}

	pub fn add_block(&mut self, block: BlockRc) -> GraphNode {
		self.graph.add_node(block)
	}

	pub fn add_connection(&mut self, from: GraphNode, to: GraphNode) {
		self.graph.update_edge(from, to, ());
	}

	pub fn remove_connection(&mut self, from: GraphNode, to: GraphNode) {
		if let Some(e) = self.graph.find_edge(from, to) {
			self.graph.remove_edge(e);
		}
	}

	pub fn register(this: SoundRc, mut driver: DriverRc) {
		Sound::unregister(this.clone());

		if let Some(sound) = Rc::get_mut(&mut this.clone()) {

			sound.driver = Some(driver.clone());
			if let Some(driver) = Rc::get_mut(&mut driver) {
				sound.id = Some(driver.register(this));
			}
		}
	}

	pub fn unregister(mut this: SoundRc) {
		if this.id != None {
			if let Some(sound) = Rc::get_mut(&mut this) {
				if let Some(mut driver) = sound.driver.clone() {
					if let Some(driver) = Rc::get_mut(&mut driver) {
						if let Some(id) = sound.id {
							driver.unregister(id);
						}
					}
				}

				sound.driver = None;
				sound.id = None;
			}
		}
	}
}

pub type SoundRc = Rc<Sound>;
