//! # Sounds
//! 
//! This module includes the types needed to represent a collection of
//! [`Generator`]s and [`Modifier`]s in an abstract, simple way.
//! 
//! [`Generator`]: ../generators/trait.Generator.html
//! [`Modifier`]: ../modifiers/trait.Modifier.html

use super::*;

pub mod basic_block;
pub mod block;
pub mod sound;
pub mod complex_sound;
pub mod simple_sound;

pub use basic_block::*;
pub use block::*;
pub use sound::*;
pub use complex_sound::*;
pub use simple_sound::*;
