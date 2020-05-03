//! # Utils
//! 
//! Module for audio related functions and types that make life easier.

use super::*;
use std::vec::Vec;
use std::ops::{Add, Sub, Mul, Div};

pub mod mono_resampler;
pub use mono_resampler::*;

/// Linear interpolation (y-y1 = m * (x-x1)) of a given value.
pub fn lerp<T>(x:T, x1:T, x2:T, y1:T, y2:T) -> T
    where T: Copy + Sized + Add<Output=T> + Sub<Output=T> + Mul<Output=T> + Div<Output=T>
{
    ((y2 - y1) / (x2 - x1)) * (x - x1) + y1
}

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
pub fn clerp<T>(x:T, x1:T, x2:T, y1:T, y2:T) -> T
    where T: Copy + Sized + PartialOrd + Add<Output=T> + Sub<Output=T> + Mul<Output=T> + Div<Output=T>
{
    lerp(clamp(x, x1, x2), x1, x2, y1, y2)
}

/// Converts a given sample count to seconds.
pub fn samples_to_seconds(s: usize) -> std::time::Duration {
    std::time::Duration::from_secs_f64(s as f64 * SAMPLE_RATE as f64)
}

/// Converts the given duration to samples, rounded to the nearest sample.
pub fn seconds_to_samples(s: std::time::Duration) -> usize {
    (s.as_secs_f64() * SAMPLE_RATE as f64).round() as usize
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
/// * Anything that [`wav::read_wav`] specifies.
/// 
/// [`std::io::Result`]: https://doc.rust-lang.org/std/io/type.Result.html
/// [`wav::Header`]: https://docs.rs/wav/0.1.1/wav/struct.Header.html
/// [`TrackT`]: ../../type.TrackT.html
/// [`wav::read_wav`]: https://docs.rs/wav/0.1.1/wav/fn.read_wav.html
pub fn read_wav(s: &mut dyn std::io::Read) -> std::io::Result<(wav::Header, Vec<SampleTrackT>)> {
    let (h, bd) = wav::read_wav(s)?;

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

/// Takes the given track and filename and writes the track data to the WAV file
/// at the given location with a given bit-depth.
/// 
/// # Parameters
/// 
/// * `track` - A vector of tracks to write. Each track is considered a channel.
/// * `bps` - The number of bits per sample. Should be 8, 16, or 24.
/// * `d` - The destination to write to.
/// 
/// # Errors
/// 
/// This function fails if:
/// * Anything that [`wav::write_wav`] specifies.
/// * The channels don't have equal lengths.
/// * The given vector contains no data.
/// 
/// # Example
/// 
/// ```
/// # use std::fs::File;
/// use bae_rs::{*,generators::*, utils::*};
/// let mut n = Noise::new();
/// let mut t = TrackT::new();
///
/// for _ in 0..SAMPLE_RATE {
///     t.push(n.process());
/// }
///
/// write_wav(vec![t], 16, &mut File::create(".junk/some/path/noise.wav").unwrap());
/// ```
/// 
/// [`wav::write_wav`]: https://docs.rs/wav/0.1.1/wav/fn.write_wav.html
pub fn write_wav(tracks: Vec<SampleTrackT>, bps: u16, d: &mut dyn std::io::Write) -> std::io::Result<()> {
    use std::io::{Error, ErrorKind};
    use crate::sample_format::*;

    if tracks.len() == 0 {
        return Err(Error::new(ErrorKind::Other, "No channels given, aborting."));
    }

    let len = tracks[0].len();

    for t in &tracks {
        if t.len() != len {
            return Err(Error::new(ErrorKind::Other, "Channels have mismatching lengths, aborting."));
        }
    }

    match bps {
        8 => {
            let mut v = Vec::new();

            for i in 0..len {
                for t in &tracks {
                    v.push(sample_to_u8(t[i]));
                }
            }

            wav::write_wav(
                wav::Header::new(1, tracks.len() as u16, SAMPLE_RATE as u32, bps),
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

            wav::write_wav(
                wav::Header::new(1, tracks.len() as u16, SAMPLE_RATE as u32, bps),
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

            wav::write_wav(
                wav::Header::new(1, tracks.len() as u16, SAMPLE_RATE as u32, bps),
                wav::BitDepth::TwentyFour(v),
                d
            )?;
        },
        _ => return Err(Error::new(ErrorKind::Other, "Unsupported bit depth, aborting.")),
    }

    Ok(())
}
