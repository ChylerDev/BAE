//! # Simple Sound
//! 
//! Module containing types implementing the ability to run a single
//! [`Generator`] and [`Modifier`] within a single object, granting the ability
//! for fast processing of simple [`Generator`]s and [`Modifier`]s into a single
//! output.
//! 
//! [`Generator`]: ../../generators/trait.Generator.html
//! [`Modifier`]: ../../generators/trait.Modifier.html

use super::*;

use std::rc::Rc;

/// Type implementing the ablitiy to run multiple a single [`Generator`] by a
/// given list of [`Modifier`]s operated in series. This allows for simple and
/// fast processing of the structure's elements while still allowing for a wide
/// range of complex sounds.
/// 
/// [`Generator`]: ../../generators/trait.Generator.html
/// [`Modifier`]: ../../modifiers/trait.Modifier.html
#[derive(Clone)]
pub struct SimpleSound {
    generator: BasicBlockRc,
    modifier_list: Vec<BasicBlockRc>,
    input_gain: SampleT,
    output_gain: SampleT,
    id: Option<usize>,
    is_muted: bool,
    is_paused: bool,
}

impl SimpleSound {
    /// Constructs a new [`SimpleSound`] object. The new object is initialized
    /// with an empty [`Vec`] of [`Modifier`]s. Add [`Modifier`]s with
    /// [`add_modifier`] or [`extend_modifiers`].
    /// 
    /// [`SimpleSound`]: struct.SimpleSound.html
    /// [`Vec`]: https://doc.rust-lang.org/std/vec/struct.Vec.html
    /// [`Modifier`]: ../../modifiers/trait.Modifier.html
    /// [`add_modifier`]: struct.SimpleSound.html#method.add_modifier
    /// [`extend_modifiers`]: struct.SimpleSound.html#method.extend_modifiers
    pub fn new(input_gain: MathT, output_gain: MathT, generator: BasicBlockRc) -> Self {
        SimpleSound {
            generator,
            modifier_list: Vec::new(),
            input_gain: input_gain as SampleT,
            output_gain: output_gain as SampleT,
            id: None,
            is_muted: false,
            is_paused: false,
        }
    }

    /// Adds a single modifier to the internal [`Vec`] of [`Modifier`]s.
    /// 
    /// [`Vec`]: https://doc.rust-lang.org/std/vec/struct.Vec.html
    /// [`Modifier`]: ../../modifiers/trait.Modifier.html
    pub fn add_modifier<M>(&mut self, m: BasicBlockRc)
        where M: 'static + Clone
    {
        self.modifier_list.push(m);
    }

    /// Extends the internal [`Vec`] of [`Modifier`]s with the given [`Vec`].
    /// 
    /// [`Vec`]: https://doc.rust-lang.org/std/vec/struct.Vec.html
    /// [`Modifier`]: ../../modifiers/trait.Modifier.html
    pub fn extend_modifiers(&mut self, m_list: Vec<BasicBlockRc>) {
        self.modifier_list.extend(m_list);
    }

    /// Returns the linear gain applied to the input during processing.
    pub fn get_input_gain(&self) -> MathT {
        self.input_gain as MathT
    }

    /// Returns the linear gain applied to the output during processing.
    pub fn get_output_gain(&self) -> MathT {
        self.output_gain as MathT
    }

    /// Sets the input linear gain that is applied during processing.
    pub fn set_input_gain(&mut self, g: MathT) {
        self.input_gain = g as SampleT;
    }

    /// Sets the output linear gain that is applied during processing.
    pub fn set_output_gain(&mut self, g: MathT) {
        self.output_gain = g as SampleT;
    }
}

impl Sound for SimpleSound {
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

        let mut out = if let Some(b) = Rc::get_mut(&mut self.generator) {
            b.prime_input(input * self.input_gain);
            b.process()
        } else {
            Default::default()
        };

        for m in &mut self.modifier_list {
            if let Some(m) = Rc::get_mut(m) {
                m.prime_input(out);
                out = m.process();
            }
        }

        if self.is_muted {
            Default::default()
        } else {
            out * self.output_gain
        }
    }

    fn get_id(&self) -> Option<usize> {
        self.id
    }
}

/// Alias for a [`SimpleSound`] wrapped in an [`Rc`].
/// 
/// [`SimpleSound`]: struct.SimpleSound.html
/// [`Rc`]: https://doc.rust-lang.org/std/rc/struct.Rc.html
pub type SimpleSoundRc = Rc<SimpleSound>;
