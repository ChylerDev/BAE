//! # Tools
//! 
//! Module for audio related functions that make life easier.

use super::*;
use std::vec::Vec;

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
	let h_vec:[u8;16] = wav::Header::new(1, 2, SAMPLE_RATE as u32, 16).into();
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

pub mod mono_resampler;

pub use mono_resampler::*;
