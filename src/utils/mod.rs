//! # Utils
//! 
//! Module for audio related functions and types that make life easier.

use super::*;
use std::vec::Vec;
use std::ops::{Add, Sub, Mul, Div};

pub mod mono_resampler;
pub use mono_resampler::*;

/// Linear interpolation (y-y1 = m * (x-x1)) of a given value.
#[inline]
pub fn lerp<T>(x:T, x1:T, x2:T, y1:T, y2:T) -> T
    where T: Copy + Sized + Add<Output=T> + Sub<Output=T> + Mul<Output=T> + Div<Output=T>
{
    ((y2 - y1) / (x2 - x1)) * (x - x1) + y1
}

#[inline]
fn clamp<T>(x: T, mut x1: T, mut x2: T) -> T
    where T: Copy + Sized + PartialOrd
{
    if x1 > x2 {
        std::mem::swap(&mut x1, &mut x2);
    }

    if x > x2 {
        x2
    } else if x < x1 {
        x1
    } else {
        x
    }
}

/// Clamped linear interpolation (y-y1 = m * (x-x1)) of a given value. The input
/// `x` is clamped to the range [`x1`,`x2`]. If `x1` is greater than `x2`, they
/// are swapped.
#[inline]
pub fn clerp<T>(x:T, x1:T, x2:T, y1:T, y2:T) -> T
    where T: Copy + Sized + PartialOrd + Add<Output=T> + Sub<Output=T> + Mul<Output=T> + Div<Output=T>
{
    lerp(clamp(x, x1, x2), x1, x2, y1, y2)
}

/// Converts a given sample count to seconds.
pub fn samples_to_seconds(s: usize, r: MathT) -> std::time::Duration {
    std::time::Duration::from_secs_f64(s as f64 * r as f64)
}

/// Converts the given duration to samples, rounded to the nearest sample.
pub fn seconds_to_samples(s: std::time::Duration, r: MathT) -> usize {
    (s.as_secs_f64() * r as f64).round() as usize
}

/// Converts from a linear gain value to a decibel (dBFS) value.
pub fn linear_to_db(g: MathT) -> MathT {
    20.0 * g.log10()
}

/// Converts from a decibel (dBFS) to a linear gain value
pub fn db_to_linear(db: MathT) -> MathT {
    10.0_f64.powf(db/20.0)
}

/// Normalizes the given audio track to have a peak value at the given dBFS
/// value.
pub fn normalize(db: MathT, t: &mut SampleTrackT) {
    let y = t.clone();
    let mut dc = 0.0;

    for s in &y {
        dc += s;
    }

    dc /= y.len() as SampleT;

    let mut max = 0.0;

    for s in y {
        if (s - dc).abs() > max {
            max = (s - dc).abs();
        }
    }

    let factor = db_to_linear(db) as SampleT / max;

    for s in t {
        *s = (*s - dc) * factor;
    }
}

/// Takes the given path and reads the track data from the WAV file at the given
/// location.
/// 
/// # Parameters/Returns
/// 
/// * `s` - The source to read from.
/// * Returned value is a [`std::io::Result`] with the `Ok` data being a tuple
/// of a [`wav::Header`] and a vector of [`TrackT`]s.
/// 
/// # Errors
/// 
/// This function fails if:
/// * Anything that [`wav::read`] specifies.
/// 
/// [`std::io::Result`]: https://doc.rust-lang.org/std/io/type.Result.html
/// [`wav::Header`]: https://docs.rs/wav/0.4.0/wav/struct.Header.html
/// [`TrackT`]: ../../type.TrackT.html
/// [`wav::read`]: https://docs.rs/wav/0.4.0/wav/fn.read.html
pub fn read_wav(s: &mut dyn std::io::Read) -> std::io::Result<(wav::Header, Vec<SampleTrackT>)> {
    let (h, bd) = wav::read(s)?;

    let mut tracks = Vec::new();
    for _ in 0..h.channel_count {
        tracks.push(SampleTrackT::new());
    }

    match bd {
        wav::BitDepth::Eight(d) => {
            for i in 0..d.len() {
                tracks[i % h.channel_count as usize].push(sample_from_u8(d[i]));
            }
        },
        wav::BitDepth::Sixteen(d) => {
            for i in 0..d.len() {
                tracks[i % h.channel_count as usize].push(sample_from_i16(d[i]));
            }
        },
        wav::BitDepth::TwentyFour(d) => {
            for i in 0..d.len() {
                tracks[i % h.channel_count as usize].push(sample_from_i24(d[i]));
            }
        },

        _ => (),
    }

    Ok((h, tracks))
}

/// Structure representing the options available to configure the format of the
/// wave file resulting from a call to [`WaveWriteOptions::write`][0], letting
/// you control the bits per sample, sampling rate, and whether or not the track
/// given to [`WaveWriteOptions::write`][0] will be clipped.
/// 
/// This struct uses a builder pattern, allowing you to chain the methods that
/// set the internal parameters, and then write the values at the end.
/// 
/// [0]: #method.write
#[derive(Default)]
pub struct WaveWriteOptions {
    bps: u16,
    r: MathT,
    clip: bool,
}

impl WaveWriteOptions {
    /// Creates new waveWriteOptions object.
    pub fn new() -> Self {
        Default::default()
    }

    /// Sets the bits per sample value.
    /// 
    /// Succeeds if bps is one of either 8, 16, or 24, fails otherwise.
    pub fn bps<'a>(&'a mut self, bps: u16) -> Result<&'a mut WaveWriteOptions,()> {
        if bps == 8 || bps == 16 || bps == 24 {
            self.bps = bps;
            Ok(self)
        } else {
            Err(())
        }
    }

    /// Sets the sampling rate.
    pub fn r<'a>(&'a mut self, r: MathT) -> &'a mut WaveWriteOptions {
        self.r = r;
        self
    }

    /// Sets whether or not values outside the range of \[-1,1\] will be clipped or not.
    pub fn clip<'a>(&'a mut self, clip: bool) -> &'a mut WaveWriteOptions {
        self.clip = clip;
        self
    }

    /// Takes the given options and tracks and writes the formatted WAV data to
    /// the passed `io::Write` object.
    /// 
    /// # Parameters
    /// 
    /// * `tracks` - A vector of tracks to write. Each track is considered a channel.
    /// * `d` - The `io::Write` object to write to.
    /// 
    /// # Errors
    /// 
    /// This function will return an error under the following conditions:
    /// * Anything that [`wav::write`] specifies.
    /// * The channels don't have equal lengths.
    /// * The given vector of channels contains no data.
    /// 
    /// # Example
    /// 
    /// ```
    /// # use std::fs::File;
    /// # use bae_rs::{*, generators::*, utils::*};
    ///
    /// let mut t = SampleTrackT::new();
    /// let mut opt = WaveWriteOptions::new();
    ///
    /// /* snip */
    ///
    /// opt.write(vec![t], &mut File::create("some_file.wav").unwrap());
    /// ```
    /// 
    /// [`wav::write`]: https://docs.rs/wav/0.4.0/wav/fn.write.html
    pub fn write(&self, mut tracks: Vec<SampleTrackT>, d: &mut dyn std::io::Write) -> std::io::Result<()> {
        use std::io::{Error, ErrorKind};
        use crate::sample_format::*;

        if tracks.len() == 0 {
            return Err(Error::new(ErrorKind::Other, "No channels given, aborting."));
        }

        let len = tracks[0].len();

        for t in &mut tracks {
            if t.len() != len {
                return Err(Error::new(ErrorKind::Other, "Channels have mismatching lengths, aborting."));
            }
            if self.clip {
                for s in t {
                    if *s > 1.0 {
                        *s = 1.0;
                    } else if *s < -1.0 {
                        *s = -1.0;
                    }
                }
            }
        }

        match self.bps {
            8 => {
                let mut v = Vec::new();

                for i in 0..len {
                    for t in &tracks {
                        v.push(sample_to_u8(t[i]));
                    }
                }

                wav::write(
                    wav::Header::new(1, tracks.len() as u16, self.r as u32, self.bps),
                    wav::BitDepth::Eight(v),
                    d
                )?;
            },
            16 => {
                let mut v = Vec::new();

                for i in 0..len {
                    for t in &tracks {
                        v.push(sample_to_i16(t[i]));
                    }
                }

                wav::write(
                    wav::Header::new(1, tracks.len() as u16, self.r as u32, self.bps),
                    wav::BitDepth::Sixteen(v),
                    d
                )?;
            },
            24 => {
                let mut v = Vec::new();

                for i in 0..len {
                    for t in &tracks {
                        v.push(sample_to_i24(t[i]));
                    }
                }

                wav::write(
                    wav::Header::new(1, tracks.len() as u16, self.r as u32, self.bps),
                    wav::BitDepth::TwentyFour(v),
                    d
                )?;
            },
            _ => return Err(Error::new(ErrorKind::InvalidData, "Unsupported bit depth, aborting.")),
        }

        Ok(())
    }
}
