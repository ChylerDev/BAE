//! # Standard Channel

use super::*;

use std::sync::Arc;
use std::collections::HashMap;
use crate::sample_format::SampleFormat;

/// Standard implementation of the [`Channel`] trait.
/// 
/// [`Channel`]: ../trait.Channel.html
#[derive(Clone)]
pub struct StandardChannel<SF>
    where SF: SampleFormat
{
    output: Vec<SF>,
    sounds: HashMap<usize, SoundSP>,
    gain: SampleT,
    id_counter: usize
}

impl<SF> StandardChannel<SF>
    where SF: SampleFormat
{
    /// Creates a new channel with the given gain.
    /// 
    /// The internal track is initialized for 10ms' worth of samples. Call
    /// [`set_process_time`] to change this.
    /// 
    /// [`set_process_time`]: ../trait.Channel.html#tymethod.set_process_time
    pub fn new(gain: MathT) -> Self {
        StandardChannel {
            output: Vec::with_capacity((0.01 * SAMPLE_RATE as MathT) as usize),
            sounds: HashMap::new(),
            gain: gain as SampleT,
            id_counter: 0
        }
    }

    fn get_id(&mut self) -> usize {
        let old = self.id_counter;

        self.id_counter += 1;

        old
    }
}

impl<SF> Channel<SF> for StandardChannel<SF>
    where SF: SampleFormat
{
    fn set_process_time(&mut self, d: Duration) {
        self.output = Vec::with_capacity((d.as_secs_f64() * SAMPLE_RATE as MathT) as usize);
    }

    fn get_output(&self) -> &Vec<SF> {
        &self.output
    }

    fn set_gain(&mut self, gain: MathT) {
        self.gain = gain as SampleT;
    }

    fn process(&mut self) {
        self.output.resize_with(self.output.len(), SF::default);

        for sample in &mut self.output {
            for mut sound in &mut self.sounds {
                *sample += SF::from_sample(Arc::get_mut(&mut sound.1).unwrap().process(Default::default()));
            }

            *sample *= self.gain;
        }
    }

    fn add_sound(&mut self, sound: &mut SoundSP) {
        let id = self.get_id();
        SoundSP::get_mut(sound).unwrap().register(id);
        self.sounds.insert(id, sound.clone());
    }

    fn remove_sound(&mut self, id: usize) {
        self.sounds.remove(&id);
    }
}
