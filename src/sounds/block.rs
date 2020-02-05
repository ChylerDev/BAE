//! # Block
//! 
//! A structure implementing the ability to create a graph of different
//! [`Generator`]s, [`Modifier`]s, and the form of interaction between them to
//! create large-scale effects such as those found in synthesizers.
//! 
//! [`Generator`]: ../../generators/trait.Generator.html
//! [`Modifier`]: ../../modifiers/trait.Modifier.html

use super::*;
use std::rc::Rc;

/// Reference-counted wrapper for a [`Generator`]
/// 
/// [`Generator`]: ../../generators/trait.Generator.html
pub type Gen = Rc<dyn generators::Generator>;

/// Reference-counted wrapper for a [`Modifier`]
/// 
/// [`Modifier`]: ../../modifiers/trait.Modifier.html
pub type Mod = Rc<dyn modifiers::Modifier>;

/// Type defining the closure that combines inputted StereoData samples from the
/// outputs of the [`Generator`]s and [`Modifier`]s of the cointaining [`Block`]
/// 
/// [`Generator`]: ../../generators/trait.Generator.html
/// [`Modifier`]: ../../modifiers/trait.Modifier.html
/// [`Block`]: struct.Block.html
pub type InterBase = dyn FnMut(StereoData, StereoData) -> StereoData;

/// Reference-counted wrapper for the closure [`InterBase`]
/// 
/// [`InterBase`]: type.InterBase.html
pub type Inter = Rc<InterBase>;

/// Struct used for generalizing the structure of and abstracting the [`Sound`]
/// struct. This allows us to create complex sounds as a graph of [`Block`]s,
/// where each block can be a [`Modifier`], [`Generator`], or both, and there output
/// of the [`Block`] is defined as some user-definable combination of the
/// [`Generator`] and [`Modifier`] output. See [`Sound`] documentation for more info.
/// 
/// Internally, the [`Generator`], [`Modifier`], and [`Inter`] are stored wrapped
/// within an [`Rc`] (note: may become [`Arc`] depending on future needs of the
/// library). This means that when you clone a [`Block`], the internal objects
/// are *not* cloned. Rather, their reference count is incremented, and the
/// wrapped objects stay where they are.
/// 
/// [`Generator`]: ../../generators/trait.Generator.html
/// [`Modifier`]: ../../modifiers/trait.Modifier.html
/// [`Block`]: struct.Block.html
/// [`Sound`]: struct.Sound.html
/// [`Inter`]: type.Inter.html
/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
/// [`Arc`]: https://doc.rust-lang.org/std/sync/struct.Arc.html
pub struct Block {
	g: Gen,
	m: Mod,
	i: Inter,
	input: StereoData,
}

impl Block {
	/// Creates a new Block from the given [`Generator`], [`Modifier`], and
	/// [`Inter`].
	/// 
	/// # Parameters
	/// 
	/// * `g` - The [`Generator`] for the [`Block`].
	/// * `m` - The [`Modifier`] for the [`Block`].
	/// * `i` - The interactor (typically a closure) that defines the combination
	/// of `g`s and `m`s samples when `Block::process()` is called.
	/// 
	/// [`Generator`]: ../../generators/trait.Generator.html
	/// [`Modifier`]: ../../modifiers/trait.Modifier.html
	/// [`Block`]: struct.Block.html
	/// [`Inter`]: type.Inter.html
	pub fn new<T, U>(g: T, m: U, i: Inter) -> Block
		where T: 'static + generators::Generator + Clone,
		      U: 'static + modifiers::Modifier + Clone
	{
		Block {
			g: Rc::new(g),
			m: Rc::new(m),
			i,
			input: StereoData::default(),
		}
	}

	/// Creates a new block from the given [`Generator`]. For the [`Block`],
	/// [`Empty`] is used for the `m`, and the return value of
	/// [`Block::generator_passthrough`] is used for `i`.
	/// 
	/// # Parameters
	/// * `g` - The [`Generator`] for the [`Block`].
	/// 
	/// [`Generator`]: ../../generators/trait.Generator.html
	/// [`Block`]: struct.Block.html
	/// [`Block::generator_passthrough`]: struct.Block.html#method.generator_passthrough
	/// [`Inter`]: type.Inter.html
	/// [`Empty`]: ../../generators/empty/struct.Empty.html
	pub fn from_generator<T>(g: T) -> Block
		where T: 'static + generators::Generator + Clone
	{
		Block {
			g: Rc::new(g),
			m: Rc::new(
				modifiers::Empty::new()
			),
			i: Block::generator_passthrough(),
			input: StereoData::default()
		}
	}

	/// Creates a new block from the given [`Modifier`]. For the [`Block`],
	/// [`Empty`] is used for `g`, and the return value of
	/// [`Block::modifier_passthrough`] is used for `i`.
	/// 
	/// # Parametrs
	/// * `m` - The [`Modifier`] for the [`Block`].
	/// 
	/// [`Modifier`]: ../../modifiers/trait.Modifier.html
	/// [`Block`]: struct.Block.html
	/// [`Block::modifier_passthrough`]: struct.Block.html#method.modifier_passthrough
	/// [`Inter`]: type.Inter.html
	/// [`Empty`]: ../../modifiers/empty/struct.Empty.html
	pub fn from_modifier<U>(m: U) -> Block
		where U: 'static + modifiers::Modifier + Clone
	{
		Block {
			g: Rc::new(
				generators::Empty::new()
			),
			m: Rc::new(m),
			i: Block::modifier_passthrough(),
			input: StereoData::default()
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
	pub fn get_g(&self) -> &Gen {
		&self.g
	}

	/// Returns a reference to the [`Rc`]-wrapped [`Modifier`].
	/// 
	/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
	/// [`Modifier`]: ../../modifiers/trait.Modifier.html
	pub fn get_m(&self) -> &Mod {
		&self.m
	}

	/// Returns a mutable reference to the [`Rc`]-wrapped [`Generator`].
	/// 
	/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
	/// [`Generator`]: ../../generators/trait.Generator.html
	pub fn get_g_mut(&mut self) -> &mut Gen {
		&mut self.g
	}

	/// Returns a mutable reference to the [`Rc`]-wrapped [`Modifier`].
	/// 
	/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
	/// [`Modifier`]: ../../modifiers/trait.Modifier.html
	pub fn get_m_mut(&mut self) -> &mut Mod {
		&mut self.m
	}

	/// Increments the internal input sample by the give sample.
	pub fn prime_input(&mut self, x: StereoData) {
		self.input += x;
	}

	/// Process the [`Block`]. Individually processes the stored [`Generator`]
	/// and [`Modifier`] which are both combined using the [`Inter`] and
	/// returned.
	/// 
	/// [`Block`]: struct.Block.html
	/// [`Generator`]: ../../generators/trait.Generator.html
	/// [`Modifier`]: ../../modifiers/trait.Modifier.html
	/// [`Inter`]: type.Inter.html
	pub fn process(&mut self) -> StereoData {
		let y = Rc::get_mut(&mut self.i).unwrap()(
			Rc::get_mut(&mut self.g).unwrap().process(),
			Rc::get_mut(&mut self.m).unwrap().process(self.input)
		);

		self.input = StereoData::default();

		y
	}
}

/// Type alias for an [`Rc`]-wrapped [`Block`] object.
/// 
/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
/// [`Block`]: struct.Block.html
pub type BlockRc = Box<Rc<Block>>;
