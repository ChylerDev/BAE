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

/// Takes the given track and filename and writes the track data to the wavefile
/// at the given location.
/// 
/// # Parameters
/// 
/// * `track` - The track to write.
/// * `path` - The path to the file to write to.
/// 
/// # Errors
/// 
/// This function fails if:
/// * The given filename/path is invalid.
/// * Parent directiories within the given path couldn't be created.
/// * Creating the file fails.
/// * Writing the data to the file fails.
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
/// tools::write_wav(t, ".junk/some/path/noise.wav");
/// ```
pub fn write_wav(track:TrackT, path: &str) -> std::io::Result<()> {
	use std::fs::File;

	if let Some(i) = path.rfind('/') {
		std::fs::create_dir_all(String::from(path.split_at(i).0))?;
	}

	let w_id = riff::ChunkId::new("WAVE").unwrap();

	let h_id = riff::ChunkId::new("fmt ").unwrap();
	let h_vec:[u8;16] = wav::Header::new(1, 1, SAMPLE_RATE as u32, 16).into();
	let h_dat = riff::Chunk::new_data(h_id, Vec::from(&h_vec[0..16]));

	let d_id = riff::ChunkId::new("data").unwrap();
	let mut d_vec = Vec::new();
	for i in track {
		let mut v:Vec<u8> = Vec::new();
		v.extend(((i * 32768_f32) as i16).to_le_bytes().iter());
		d_vec.append(&mut v);
	}
	let d_dat = riff::Chunk::new_data(d_id, d_vec);

	let r = riff::Chunk::new_riff(w_id, vec![h_dat, d_dat]);

	let mut f = File::create(path)?;

	riff::write_chunk(&mut f, &r)?;

	Ok(())
}
