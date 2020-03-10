# Broad Audio Engine (BAE)

The Broad Audio Engine presents the ability to process sounds and wav files for applications like video games and DAWs.

For more information please see the [documentation](https://docs.rs/bae_rs)

|   |                                       |
|--:|---------------------------------------|
|1  |[Rust and C++](#1-rust-and-c)          |
|2  |[Future Expansion](#2-future-expansion)|

## 1. Rust and C++

This library is programmed and maintained in parallel in both Rust and C++, and can each be found in their respective branches.  

The rust crate can be found [here](https://crates.io/crates/bae_rs).

SoonTM this will be split off into two different README's on the separate branches. I'm probably gonna forget to change that.

## 2. Future Expansion

* Modifiers to add:
  * Pan (C++-specific)
  * Fade in/out
* Features:
  * Side-chain (?)
  * FFT (use FFTW?)
  * Refactor to use multiple audio formats (mono, stereo, 2.1, 5.1, 7.1, etc.) (means refactoring code to output mono and transform into other format) (C++-specific)
