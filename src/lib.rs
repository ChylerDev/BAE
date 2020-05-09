//! # Broad Audio Engine (BAE)
//! 
//! [![Latest version](https://img.shields.io/crates/v/bae_rs.svg)](https://crates.io/crates/bae_rs "bae_rs on crates.io")
//! [![Documentation](https://docs.rs/bae_rs/badge.svg)](https://docs.rs/bae_rs "bae_rs documentation")
//! [![License](https://img.shields.io/crates/l/bae_rs.svg)](https://github.com/ChylerDev/BAE#license "bae_rs license")
//! 
//! The Broad Audio Engine presents the ability to process sounds and audio files for applications like video games and DAWs.
//! 
//! This engine was born out of a frustration with other audio systems for their lack of readable code that results from the large monolithic structures and architecture-specific systems they contain. I do understand why these structures and systems are the way they are, but nevertheless they are a pain to work with in most cases. With this audio engine, I intend to create an interface that is well documented and has minimal time to start writing code and calculate samples.
//! 
//! To get started, there is always the [documentation], but additionally I recommend taking a look at the [`Generator`], [`Modifier`], and [`SimpleSound`] structures to get you started. In the world of BAE, sources of sound (e.g. simple sine, wav file, etc.) are called "Generators" and filters (e.g. low pass, reverb, etc.) are called "Modifiers". There's no particular reason for this naming convention other than I like them. Utilizing the [`Generator`], [`Modifier`], and [`SimpleSound`] you can get started with a simple system for generating your samples. In truth you could get your samples from a [`Generator`] on its own, and that functionality is allowed, but for more advanced sounds and systems you'll likely need a more complex way of representing those systems. For that purpose there is the [`Sound`] trait, giving the ability to operate many [`Generator`]s and [`Modifier`]s as a single unit, as you might find with some digital synthesizers.
//! 
//! [documentation]: https://docs.rs/bae_rs
//! [`Generator`]: https://docs.rs/bae_rs/0.13.2/bae_rs/generators/trait.Generator.html
//! [`Modifier`]: https://docs.rs/bae_rs/0.13.2/bae_rs/modifiers/trait.Modifier.html
//! [`Sound`]: https://docs.rs/bae_rs/0.13.2/bae_rs/sounds/trait.Sound.html
//! [`SimpleSound`]: https://docs.rs/bae_rs/0.13.2/bae_rs/sounds/simple_sound/struct.SimpleSound.html
//! 
//! ## Dependencies
//! 
//! * [`lazy_static`](https://crates.io/crates/lazy_static): For initializing large arrays at run-time for some systems that use a wavetable.
//! * [`petgrah`](https://crates.io/crates/petgraph): For the graph structure used by the [`ComplexSound`](https://docs.rs/bae_rs/0.13.2/bae_rs/sounds/complex_sound/struct.ComplexSound.html) struct.
//! * [`rand`](https://crates.io/crates/rand): To generate white noise.
//! * [`version-sync`](https://crates.io/crates/version-sync): Ensures that crate version numbers are correct in various locations.
//! * [`wav`](https://crates.io/crates/wav): To read and write WAV files.
//! 
//! ## Future Expansion
//! 
//! In no particular order
//! 
//! * Generators:
//! * Modifiers:
//! * Features:
//!   * Side-chain (?)
//!   * FFT (use FFTW?)
//!   * Read/write multiple audio formats (not just WAV)
//! 
//! ## License
//! 
//! This library is licensed under the MIT license. The loud words can be found [here](https://github.com/ChylerDev/BAE/blob/master/LICENSE)

#![warn(missing_docs)]

#![doc(html_root_url = "https://docs.rs/bae_rs/0.13.2")]

use std::vec::Vec;

/// Type to perform infrequent mathematical operations with when accuracy is needed.
pub type MathT = f64;
/// Type to calculate samples with.
pub type SampleT = f32;
/// Shorthand for a vector containing sample data.
pub type SampleTrackT = Vec<SampleT>;

pub mod channels;
pub mod debug;
pub mod generators;
pub mod modifiers;
pub mod sounds;
pub mod sample_format;
// pub mod tools;
pub mod utils;

pub use sample_format::*;
