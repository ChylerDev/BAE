//! # Tools
//! 
//! Module for audio related functions that make life easier.

use super::*;
use std::vec::Vec;

pub mod mono_resampler;
pub use mono_resampler::*;

/// Linear interpolation of a given value.
pub fn lerp<T>(x:T, x1:T, x2:T, y1:T, y2:T) -> T
	where T: Copy + Sized + std::ops::Add<Output=T> + std::ops::Sub<Output=T> + std::ops::Mul<Output=T> + std::ops::Div<Output=T>
{
	((y2 - y1) / (x2 - x1)) * (x - x1) + y1
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
pub fn normalize(db: MathT, t: &mut TrackT) {
	let y = t.clone();
	let mut dc = 0.0;

	for s in &y {
		dc += s;
	}

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

/// Takes the given filename and reads the track data from the wavefile at the
/// given location.
pub fn read_file(s: &str) -> std::io::Result<(wav::Header, TrackT)> {
	let (h, bd) = wav::read_file(std::path::Path::new(s))?;

	let mut t = TrackT::new();

	match bd {
		wav::BitDepth::Eight(d) => {
			for i in 0..d.len() {
				if i % h.channel_count as usize == 0 {
					t.push(sample_from_u8(d[i]));
				}
			}
		},
		wav::BitDepth::Sixteen(d) => {
			for i in 0..d.len() {
				if i % h.channel_count as usize == 0 {
					t.push(sample_from_i16(d[i]));
				}
			}
		},
		wav::BitDepth::TwentyFour(d) => {
			for i in 0..d.len() {
				if i % h.channel_count as usize == 0 {
					t.push(sample_from_i24(d[i]));
				}
			}
		},

		_ => (),
	}

	Ok((h, t))
}

/// Takes the given track and filename and writes the track data to the wavefile
/// at the given location with a given bitdepth.
/// 
/// # Parameters
/// 
/// * `track` - A vector of tracks to write. Each track is considered a channel.
/// * `bps` - The number of bits per sample. Should be 8, 16, or 24.
/// * `path` - The path to the file to write to.
/// 
/// # Errors
/// 
/// This function fails if:
/// * The given filename/path is invalid.
/// * Parent directiories within the given path couldn't be created.
/// * Creating the file fails.
/// * Writing the data to the file fails.
/// * The channels don't have equal lengths.
/// * The given vector contains no data.
/// 
/// # Example
/// 
/// ```
/// use bae_rs::{*,generators::*};
/// let mut n = Noise::new();
/// let mut t = TrackT::new();
///
/// for _ in 0..SAMPLE_RATE {
/// 	t.push(n.process());
/// }
///
/// tools::write_wav(vec![t], 16, ".junk/some/path/noise.wav");
/// ```
pub fn write_wav(tracks: Vec<TrackT>, bps: u16, path: &str) -> std::io::Result<()> {
	use std::path::Path;
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
				Path::new(path)
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
				Path::new(path)
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
				Path::new(path)
			)?;
		},
		_ => return Err(Error::new(ErrorKind::Other, "Unsupported bit depth, aborting.")),
	}

	Ok(())
}
