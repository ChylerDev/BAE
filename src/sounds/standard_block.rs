//! # StandardBlock
//! 
//! A structure implementing the ability to create a graph of different
//! [`Generator`]s, [`Modifier`]s, and the form of interaction between them to
//! create large-scale effects such as those found in synthesizers.
//! 
//! [`Generator`]: ../../generators/trait.Generator.html
//! [`Modifier`]: ../../modifiers/trait.Modifier.html

use super::*;
use std::rc::Rc;

/// Type defining the closure that combines inputted SampleT samples from the
/// outputs of the [`Generator`]s and [`Modifier`]s of the cointaining [`StandardBlock`]
/// 
/// [`Generator`]: ../../generators/trait.Generator.html
/// [`Modifier`]: ../../modifiers/trait.Modifier.html
/// [`StandardBlock`]: struct.StandardBlock.html
pub type InterBase = dyn FnMut(SampleT, SampleT) -> SampleT;

/// Reference-counted wrapper for the closure [`InterBase`]
/// 
/// [`InterBase`]: type.InterBase.html
pub type Inter = Rc<InterBase>;

/// Struct used for generalizing the structure of and abstracting the [`Sound`]
/// struct. This allows us to create complex sounds as a graph of [`StandardBlock`]s,
/// where each block can be a [`Modifier`], [`Generator`], or both, and there output
/// of the [`StandardBlock`] is defined as some user-definable combination of the
/// [`Generator`] and [`Modifier`] output. See [`Sound`] documentation for more info.
/// 
/// Internally, the [`Generator`], [`Modifier`], and [`Inter`] are stored wrapped
/// within an [`Rc`] (note: may become [`Arc`] depending on future needs of the
/// library). This means that when you clone a [`StandardBlock`], the internal objects
/// are *not* cloned. Rather, their reference count is incremented, and the
/// wrapped objects stay where they are.
/// 
/// [`Generator`]: ../../generators/trait.Generator.html
/// [`Modifier`]: ../../modifiers/trait.Modifier.html
/// [`StandardBlock`]: struct.StandardBlock.html
/// [`Sound`]: struct.Sound.html
/// [`Inter`]: type.Inter.html
/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
/// [`Arc`]: https://doc.rust-lang.org/std/sync/struct.Arc.html
pub struct StandardBlock {
	g: generators::GeneratorRc,
	m: modifiers::ModifierRc,
	i: Inter,
	input: SampleT,
}

impl StandardBlock {
	/// Creates a new StandardBlock from the given [`Generator`], [`Modifier`], and
	/// [`Inter`].
	/// 
	/// # Parameters
	/// 
	/// * `g` - The [`Generator`] for the [`StandardBlock`].
	/// * `m` - The [`Modifier`] for the [`StandardBlock`].
	/// * `i` - The interactor (typically a closure) that defines the combination
	/// of `g`s and `m`s samples when `StandardBlock::process()` is called.
	/// 
	/// [`Generator`]: ../../generators/trait.Generator.html
	/// [`Modifier`]: ../../modifiers/trait.Modifier.html
	/// [`StandardBlock`]: struct.StandardBlock.html
	/// [`Inter`]: type.Inter.html
	pub fn new<T, U>(g: T, m: U, i: Inter) -> Self
		where T: 'static + generators::Generator,
		      U: 'static + modifiers::Modifier
	{
		StandardBlock {
			g: Rc::new(g),
			m: Rc::new(m),
			i,
			input: SampleT::default(),
		}
	}

	/// Creates a new block from the given [`Generator`]. For the [`StandardBlock`],
	/// [`Empty`] is used for the `m`, and the return value of
	/// [`StandardBlock::generator_passthrough`] is used for `i`.
	/// 
	/// # Parameters
	/// * `g` - The [`Generator`] for the [`StandardBlock`].
	/// 
	/// [`Generator`]: ../../generators/trait.Generator.html
	/// [`StandardBlock`]: struct.StandardBlock.html
	/// [`StandardBlock::generator_passthrough`]: struct.StandardBlock.html#method.generator_passthrough
	/// [`Inter`]: type.Inter.html
	/// [`Empty`]: ../../generators/empty/struct.Empty.html
	pub fn from_generator<T>(g: T) -> Self
		where T: 'static + generators::Generator
	{
		StandardBlock {
			g: Rc::new(g),
			m: Rc::new(
				modifiers::Passthrough::new()
			),
			i: Self::generator_passthrough(),
			input: SampleT::default()
		}
	}

	/// Creates a new block from the given [`Modifier`]. For the [`StandardBlock`],
	/// [`Empty`] is used for `g`, and the return value of
	/// [`StandardBlock::modifier_passthrough`] is used for `i`.
	/// 
	/// # Parametrs
	/// * `m` - The [`Modifier`] for the [`StandardBlock`].
	/// 
	/// [`Modifier`]: ../../modifiers/trait.Modifier.html
	/// [`StandardBlock`]: struct.StandardBlock.html
	/// [`StandardBlock::modifier_passthrough`]: struct.StandardBlock.html#method.modifier_passthrough
	/// [`Inter`]: type.Inter.html
	/// [`Empty`]: ../../modifiers/empty/struct.Empty.html
	pub fn from_modifier<U>(m: U) -> Self
		where U: 'static + modifiers::Modifier
	{
		StandardBlock {
			g: Rc::new(
				generators::Zero::new()
			),
			m: Rc::new(m),
			i: Self::modifier_passthrough(),
			input: SampleT::default()
		}
	}

	/// Creates the default interactor which simply multiplies the two passed
	/// samples together.
	pub fn default_interactor() -> Inter {
		Rc::new(|ge, mo| ge * mo)
	}

	/// Creates a passthrough interactor which passes the [`Generator`] sample
	/// through.
	/// 
	/// [`Generator`]: ../../generators/trait.Generator.html
	pub fn generator_passthrough() -> Inter {
		Rc::new(|ge, _| ge)
	}

	/// Creates a passthrough interactor which passes the [`Modifier`] sample
	/// through.
	/// 
	/// [`Modifier`]: ../../modifiers/trait.Modifier.html
	pub fn modifier_passthrough() -> Inter {
		Rc::new(|_, mo| mo)
	}

	/// Returns a reference to the [`Rc`]-wrapped [`Generator`].
	/// 
	/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
	/// [`Generator`]: ../../generators/trait.Generator.html
	pub fn get_g(&self) -> &generators::GeneratorRc {
		&self.g
	}

	/// Returns a reference to the [`Rc`]-wrapped [`Modifier`].
	/// 
	/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
	/// [`Modifier`]: ../../modifiers/trait.Modifier.html
	pub fn get_m(&self) -> &modifiers::ModifierRc {
		&self.m
	}

	/// Returns a mutable reference to the [`Rc`]-wrapped [`Generator`].
	/// 
	/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
	/// [`Generator`]: ../../generators/trait.Generator.html
	pub fn get_g_mut(&mut self) -> &mut generators::GeneratorRc {
		&mut self.g
	}

	/// Returns a mutable reference to the [`Rc`]-wrapped [`Modifier`].
	/// 
	/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
	/// [`Modifier`]: ../../modifiers/trait.Modifier.html
	pub fn get_m_mut(&mut self) -> &mut modifiers::ModifierRc {
		&mut self.m
	}
}

impl Block for StandardBlock {
	fn prime_input(&mut self, x: SampleT) {
		self.input += x;
	}

	fn process(&mut self) -> SampleT {
		let y = Rc::get_mut(&mut self.i).unwrap()(
			Rc::get_mut(&mut self.g).unwrap().process(),
			Rc::get_mut(&mut self.m).unwrap().process(self.input)
		);

		self.input = SampleT::default();

		y
	}
}

/// Alias for an [`Rc`]-wrapped [`StandardBlock`] object.
/// 
/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
/// [`StandardBlock`]: struct.StandardBlock.html
pub type BlockRc = Rc<StandardBlock>;
