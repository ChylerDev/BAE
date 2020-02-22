//! # Basic Block Interface

use super::*;
use std::rc::Rc;

/// Trait used for generalizing the interface that allows for the processing of
/// multiple audio objects (be they [`Generator`]s or [`Modifier`]s or both).
/// 
/// [`Generator`]: ../../generators/trait.Generator.html
/// [`Modifier`]: ../../modifiers/trait.Modifier.html
pub trait BasicBlock {
	/// Increments the internal input sample by the given sample.
	fn prime_input(&mut self, x: SampleT);

	/// Process the [`BasicBlock`]. Individually processes the stored [`Generator`]
	/// and [`Modifier`] objects which are both combined using the [`Inter`] and
	/// returned.
	/// 
	/// [`BasicBlock`]: trait.BasicBlock.html
	/// [`Generator`]: ../../generators/trait.Generator.html
	/// [`Modifier`]: ../../modifiers/trait.Modifier.html
	/// [`Inter`]: type.Inter.html
	fn process(&mut self) -> SampleT;
}

/// Alias for an [`Rc`]-wrapped [`BasicBlock`] object.
/// 
/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
/// [`BasicBlock`]: trait.BasicBlock.html
pub type BlockRc = Rc<dyn BasicBlock>;
