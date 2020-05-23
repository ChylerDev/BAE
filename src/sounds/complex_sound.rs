//! # Complex Sound
//!
//! Module containing types implementing the ability to run multiple
//! [`Generator`]s and [`Modifier`]s within a single object, granting the
//! ability to create complex systems like those found in digital synthesizers.
//!
//! [`Generator`]: ../../generators/trait.Generator.html
//! [`Modifier`]: ../../modifiers/trait.Modifier.html

use super::*;
use petgraph::graph;
use std::collections::VecDeque;
use std::sync::Arc;

/// Alias for the graph type used by [`ComplexSound`].
///
/// [`ComplexSound`]: struct.ComplexSound.html
pub type Graph = graph::DiGraph<BlockSP, ()>;

/// Alias for the nodes of the graph used by [`ComplexSound`].
///
/// [`ComplexSound`]: struct.ComplexSound.html
pub type GraphNode = graph::NodeIndex;

/// Alias for the container storing the order of processing [`GraphNode`]s.
///
/// [`GraphNode`]: type.GraphNode.html
pub type ProcessOrder = VecDeque<GraphNode>;

/// Type implementing the ability to run multiple [`Generator`]s and
/// [`Modifier`]s within a single object, granting the ability to create complex
/// systems like those found in digital synthesizers.
///
/// [`Generator`]: ../../generators/trait.Generator.html
/// [`Modifier`]: ../../modifiers/trait.Modifier.html
#[derive(Clone)]
pub struct ComplexSound {
    graph: Graph,
    process_order: ProcessOrder,
    input_gain: GraphNode,
    output_gain: GraphNode,
    id: Option<usize>,
    is_muted: bool,
    is_paused: bool,
}

impl ComplexSound {
    /// Creates a new [`ComplexSound`] object with the given input and output gain
    /// values.
    ///
    /// [`ComplexSound`]: struct.ComplexSound.html
    pub fn new(input_gain: MathT, output_gain: MathT) -> Self {
        let mut graph = Graph::new();
        let input_gain = graph.add_node(Arc::new(StandardBlock::from_modifier(
            modifiers::Gain::new(input_gain as SampleT),
        )));
        let output_gain = graph.add_node(Arc::new(StandardBlock::from_modifier(
            modifiers::Gain::new(output_gain as SampleT),
        )));

        ComplexSound {
            graph,
            process_order: ProcessOrder::new(),
            input_gain,
            output_gain,
            id: None,
            is_muted: false,
            is_paused: false,
        }
    }

    /// Returns the ['GraphNode'] containing the input gain [`Modifier`].
    ///
    /// [`Graph`]: type.Graph.html
    /// [`Modifier`]: ../../modifiers/trait.Modifier.html
    pub fn get_input_gain(&self) -> GraphNode {
        self.input_gain
    }

    /// Returns the ['GraphNode'] containing the output gain [`Modifier`].
    ///
    /// [`GraphNode`]: type.GraphNode.html
    /// [`Modifier`]: ../../modifiers/trait.Modifier.html
    pub fn get_output_gain(&self) -> GraphNode {
        self.output_gain
    }

    /// Adds a new node to the [`Graph`], using the given [`Block`] for the node
    /// value. Returns the [`GraphNode`] containing the [`Block`] for later use
    /// with [`add_connection`] and [`remove_connection`].
    ///
    /// [`Graph`]: type.Graph.html
    /// [`Block`]: ../trait.Block.html
    /// [`add_connection]: struct.ComplexSound.html#method.add_connection
    /// [`remove_connection]: struct.ComplexSound.html#method.remove_connection
    pub fn add_block(&mut self, block: BlockSP) -> GraphNode {
        self.graph.add_node(block)
    }

    /// Adds a new connection (edge) between the two given [`GraphNode`]s.
    ///
    /// [`GraphNode`]: type.GraphNode.html
    pub fn add_connection(&mut self, from: GraphNode, to: GraphNode) {
        self.graph.update_edge(from, to, ());

        self.process_order();
    }

    /// Removes a connection between the two given [`GraphNode`]s.
    ///
    /// [`GraphNode`]: type.GraphNode.html
    pub fn remove_connection(&mut self, from: GraphNode, to: GraphNode) {
        if let Some(e) = self.graph.find_edge(from, to) {
            self.graph.remove_edge(e);
        }

        self.process_order();
    }

    /// Returns a copy of the list of all nodes of the graph in the order in
    /// which they will be processed.
    pub fn get_nodes(&self) -> ProcessOrder {
        self.process_order.clone()
    }

    /// Processes the graph and constructs the order to process the
    /// [`GraphNode`]s.
    ///
    /// [`GraphNode`]: type.GraphNode.html
    fn process_order(&mut self) {
        self.process_order.clear();

        self.process_order
            .extend(self.graph.externals(petgraph::Direction::Incoming));
        let mut i = 0;
        while i < self.process_order.len() {
            let neighbors = self.graph.neighbors(self.process_order[i]);
            self.process_order.extend(neighbors);

            let mut j = 0;
            while j < i {
                Self::remove_dups(&mut self.process_order, j);
                j += 1;
            }

            i += 1;
        }

        let ig = self.input_gain;
        let og = self.output_gain;

        self.process_order.retain(|e| *e != ig && *e != og);

        self.process_order.push_front(self.input_gain);
        self.process_order.push_back(self.output_gain);
    }

    /// Removes duplicate [`GraphNode`]s from the process order to prevent parsing cycles
    ///
    /// [`GraphNode`]: type.GraphNode.html
    fn remove_dups(v: &mut ProcessOrder, whitelist: usize) {
        let mut i = whitelist + 1;

        while i < v.len() {
            if v[i] == v[whitelist] {
                v.remove(i);
            } else {
                i += 1;
            }
        }
    }
}

impl Sound for ComplexSound {
    fn toggle_pause(&mut self) {
        self.is_paused = !self.is_paused;
    }

    fn is_paused(&self) -> bool {
        self.is_paused
    }

    fn toggle_mute(&mut self) {
        self.is_muted = !self.is_muted;
    }

    fn is_muted(&self) -> bool {
        self.is_muted
    }

    fn register(&mut self, id: usize) {
        self.id = Some(id);
    }

    fn unregister(&mut self) {
        self.id = None;
    }

    fn process(&mut self, input: SampleT) -> SampleT {
        if self.is_paused {
            return Default::default();
        }

        let mut out = Default::default();

        BlockSP::get_mut(self.graph.node_weight_mut(self.input_gain).unwrap())
            .unwrap()
            .prime_input(input);

        for b in &self.process_order {
            let block = BlockSP::get_mut(self.graph.node_weight_mut(*b).unwrap()).unwrap();
            out = block.process();

            let mut neighbors = self.graph.neighbors(*b).detach();

            while let Some(t) = neighbors.next(&self.graph) {
                BlockSP::get_mut(self.graph.node_weight_mut(t.1).unwrap())
                    .unwrap()
                    .prime_input(out);
            }
        }

        if self.is_muted {
            Default::default()
        } else {
            out
        }
    }

    fn get_id(&self) -> Option<usize> {
        self.id
    }
}
