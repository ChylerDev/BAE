//! # Sound Channels
//! 
//! This module includes the types and traits needed to process a group of
//! [`Sound`]s to generate a single output sample.
//! 
//! [`Sound`]: ../sounds/trait.Sound.html

use super::*;
use std::sync::Arc;
use std::time::Duration;
use crate::sounds::*;

pub mod standard_channel;
pub use standard_channel::*;

/// Trait defining the simplest possible interface for a channel, with the
/// ability to process a batch of samples at a time.
pub trait Channel {
    /// Sets the amount of time [`process`] should calculate samples for. The
    /// given duration is truncated to a integer sample value.
    /// 
    /// [`process`]: trait.Channel.html#tymethod.process
    fn set_process_time(&mut self, d: Duration);

    /// Returns a reference to the internal track of samples.
    fn get_output(&self) -> &TrackT;

    /// Sets the gain of the output of the channel.
    fn set_gain(&mut self, gain: MathT);

    /// Processes the given number of samples, storing the results in the
    /// internal track of samples.
    fn process(&mut self);

    /// Adds a [`Sound`] to the [`Channel`] for processing.
    /// 
    /// [`Channel`]: trait.Channel.html
    /// [`Sound`]: ../sounds/trait.Sound.html
    fn add_sound(&mut self, sound: &mut SoundSP);

    /// Removes a [`Sound`] from the [`Channel`].
    /// 
    /// The `id` parameter can be accessed from the registered [`Sound`] itself.
    /// 
    /// [`Channel`]: trait.Channel.html
    /// [`Sound`]: ../sounds/trait.Sound.html
    fn remove_sound(&mut self, id: usize);
}

/// Alias for a [`Channel`] wrapped in a smart pointer.
/// 
/// [`Channel`]: trait.Channel.html
pub type ChannelSP = Arc<dyn Channel>;
