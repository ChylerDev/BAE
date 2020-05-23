//! # WAV
//!
//! A wave data player.

use super::*;
use crate::sample_format::MonoTrackT;
use crate::utils::mono_resampler::*;

/// Struct for playing wave files.
#[derive(Clone)]
pub struct MonoWav {
    resam: MonoResampler,
}

impl MonoWav {
    /// Constructs from a given path.
    pub fn from_source(s: &mut dyn std::io::Read, sample_rate: MathT) -> Self {
        let (h, t) = utils::read_wav(s).expect("File could not be read");

        let mut mt = MonoTrackT::new();

        for s in t.first().expect("No audio data read") {
            mt.push(Mono::from_sample(*s));
        }

        MonoWav::from_track(sample_rate, h.sampling_rate as MathT, mt)
    }

    /// Converts from the given track and source sample rate.
    pub fn from_track(sample_rate: MathT, source_sampling_rate: MathT, t: MonoTrackT) -> Self {
        MonoWav {
            resam: MonoResampler::new(t, sample_rate, source_sampling_rate, 0, 0),
        }
    }

    /// Borrows the resampler object.
    pub fn get(&self) -> &MonoResampler {
        &self.resam
    }

    /// Mutably borrows the resampler object.
    pub fn get_mut(&mut self) -> &mut MonoResampler {
        &mut self.resam
    }
}

impl Generator for MonoWav {
    fn process(&mut self) -> SampleT {
        self.resam.process()
    }
}
