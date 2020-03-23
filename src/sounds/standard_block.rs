//! # StandardBlock
//! 
//! A structure implementing the ability to create a graph of different
//! [`Generator`]s, [`Modifier`]s, and the form of interaction between them to
//! create large-scale effects such as those found in synthesizers.
//! 
//! [`Generator`]: ../../generators/trait.Generator.html
//! [`Modifier`]: ../../modifiers/trait.Modifier.html

use super::*;

use std::sync::Arc;
use generators::GeneratorSP;
use modifiers::ModifierSP;

/// Type defining the closure that combines inputted SampleT samples from the
/// outputs of the [`Generator`]s and [`Modifier`]s of the containing
/// [`StandardBlock`].
/// 
/// [`Generator`]: ../../generators/trait.Generator.html
/// [`Modifier`]: ../../modifiers/trait.Modifier.html
/// [`StandardBlock`]: struct.StandardBlock.html
pub type InterBase = dyn FnMut(SampleT, SampleT) -> SampleT;

/// Reference-counted wrapper for the closure [`InterBase`]
/// 
/// [`InterBase`]: type.InterBase.html
pub type Inter = Arc<InterBase>;

/// Struct used for generalizing the structure of and abstracting the [`Sound`]
/// struct. This allows us to create complex sounds as a graph of [`StandardBlock`]s,
/// where each block can be a [`Modifier`], [`Generator`], or both, and there output
/// of the [`StandardBlock`] is defined as some user-definable combination of the
/// [`Generator`] and [`Modifier`] output. See [`Sound`] documentation for more info.
/// 
/// Internally, the [`Generator`], [`Modifier`], and [`Inter`] are stored wrapped
/// within an [`Arc`]. This means that when you clone a [`StandardBlock`], the
/// internal objects are *not* cloned. Rather, their reference count is incremented,
/// and the wrapped objects stay where they are.
/// 
/// [`Generator`]: ../../generators/trait.Generator.html
/// [`Modifier`]: ../../modifiers/trait.Modifier.html
/// [`StandardBlock`]: struct.StandardBlock.html
/// [`Sound`]: struct.Sound.html
/// [`Inter`]: type.Inter.html
/// [`Arc`]: https://doc.rust-lang.org/std/sync/struct.Arc.html
pub struct StandardBlock {
    g: GeneratorSP,
    m: ModifierSP,
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
            g: Arc::new(g),
            m: Arc::new(m),
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
            g: Arc::new(g),
            m: Arc::new(
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
            g: Arc::new(
                generators::Zero::new()
            ),
            m: Arc::new(m),
            i: Self::modifier_passthrough(),
            input: SampleT::default()
        }
    }

    /// Creates the default interactor which simply multiplies the two passed
    /// samples together.
    pub fn default_interactor() -> Inter {
        Arc::new(|ge, mo| ge * mo)
    }

    /// Creates a passthrough interactor which passes the [`Generator`] sample
    /// through.
    /// 
    /// [`Generator`]: ../../generators/trait.Generator.html
    pub fn generator_passthrough() -> Inter {
        Arc::new(|ge, _| ge)
    }

    /// Creates a passthrough interactor which passes the [`Modifier`] sample
    /// through.
    /// 
    /// [`Modifier`]: ../../modifiers/trait.Modifier.html
    pub fn modifier_passthrough() -> Inter {
        Arc::new(|_, mo| mo)
    }

    /// Returns a reference to the [`Generator`] wrapped in a smart pointer.
    /// 
    /// [`Generator`]: ../../generators/trait.Generator.html
    pub fn get_g(&self) -> &generators::GeneratorSP {
        &self.g
    }

    /// Returns a reference to the [`Modifier`] wrapped in a smart pointer.
    /// 
    /// [`Modifier`]: ../../modifiers/trait.Modifier.html
    pub fn get_m(&self) -> &modifiers::ModifierSP {
        &self.m
    }

    /// Returns a mutable reference to the [`Generator`] wrapped in a smart pointer.
    /// 
    /// [`Generator`]: ../../generators/trait.Generator.html
    pub fn get_g_mut(&mut self) -> &mut generators::GeneratorSP {
        &mut self.g
    }

    /// Returns a mutable reference to the [`Modifier`] wrapped in a smart pointer.
    /// 
    /// [`Modifier`]: ../../modifiers/trait.Modifier.html
    pub fn get_m_mut(&mut self) -> &mut modifiers::ModifierSP {
        &mut self.m
    }
}

impl Block for StandardBlock {
    fn prime_input(&mut self, x: SampleT) {
        self.input += x;
    }

    fn process(&mut self) -> SampleT {
        let y = Inter::get_mut(&mut self.i).unwrap()(
            GeneratorSP::get_mut(&mut self.g).unwrap().process(),
            ModifierSP::get_mut(&mut self.m).unwrap().process(self.input)
        );

        self.input = SampleT::default();

        y
    }
}

/// Alias for a [`StandardBlock`] object wrapped in a smart pointer.
/// 
/// [`StandardBlock`]: struct.StandardBlock.html
pub type StandardBlockSP = Arc<StandardBlock>;
