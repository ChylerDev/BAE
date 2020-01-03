//! # Tools
//! 
//! Module for audio related functions that make life easier.

use super::*;
use std::vec::Vec;

/// Structure for the "fmt " chunk of wave files, specifying key information
/// about the enclosed data.
#[derive(Debug,Default,Copy,Clone)]
pub struct WAVHeader {
	pub audio_format:u16,
	pub channel_count:u16,
	pub sampling_rate:u32,
	pub bytes_per_second:u32,
	pub bytes_per_sample:u16,
	pub bits_per_sample:u16,
}

impl WAVHeader {
	/// Creates a new WAVHeader object.
	/// 
	/// # Parameters
	/// 
	/// * `af` - Audio format. Generally 1.
	/// * `cc` - Channel count, the number of channels each sample has. Generally 2.
	/// * `r` - Sampling rate.
	/// * `bps` - Number of bits in each (sub-channel) sample. Generally 8, 16, or 32.
	/// 
	/// # Example
	/// 
	/// ```
	/// let h = ocae::tools::WAVHeader::new(1, 2, ocae::SAMPLE_RATE as u32, 16);
	/// ```
	pub fn new(af:u16, cc:u16, r:u32, bps:u16) -> WAVHeader {
		WAVHeader {
			audio_format: af,
			channel_count: cc,
			sampling_rate: r,
			bytes_per_second: (((bps >> 3) * cc) as u32) * r,
			bytes_per_sample: ((bps >> 3) * cc) as u16,
			bits_per_sample: bps
		}
	}
}

impl Into<[u8;16]> for WAVHeader {
	/// Converts the WAVHeader object into a vector of its bytes
	/// 
	/// # Example
	/// 
	/// ```
	/// use ocae::{SAMPLE_RATE,tools::WAVHeader};
	/// let h:[u8;16] = WAVHeader::new(1, 2, SAMPLE_RATE as u32, 16).into();
	/// ```
	fn into(self) -> [u8;16] {
		let mut v:[u8;16] = [0;16];

		let b = self.audio_format.to_le_bytes();
		v[0] = b[0];
		v[1] = b[1];
		let b = self.channel_count.to_le_bytes();
		v[2] = b[0];
		v[3] = b[1];
		let b = self.sampling_rate.to_le_bytes();
		v[4] = b[0];
		v[5] = b[1];
		v[6] = b[2];
		v[7] = b[3];
		let b = self.bytes_per_second.to_le_bytes();
		v[8] = b[0];
		v[9] = b[1];
		v[10] = b[2];
		v[11] = b[3];
		let b = self.bytes_per_sample.to_le_bytes();
		v[12] = b[0];
		v[13] = b[1];
		let b = self.bits_per_sample.to_le_bytes();
		v[14] = b[0];
		v[15] = b[1];

		v
	}
}

impl From<[u8;16]> for WAVHeader {
	/// Converts an array of 16 raw bytes into a WAVHeader object. Intended for
	/// use with bytes read in from wave files.
	/// 
	/// # Parameters
	/// 
	/// * `v` - The raw bytes to convert from.
	fn from(v:[u8;16]) -> Self {
		let audio_format     = u16::from_le_bytes([v[0 ],v[1 ]]);
		let channel_count    = u16::from_le_bytes([v[2 ],v[3 ]]);
		let sampling_rate    = u32::from_le_bytes([v[4 ],v[5 ],v[6 ],v[7 ]]);
		let bytes_per_second = u32::from_le_bytes([v[8 ],v[9 ],v[10],v[11]]);
		let bytes_per_sample = u16::from_le_bytes([v[12],v[13]]);
		let bits_per_sample  = u16::from_le_bytes([v[14],v[15]]);

		WAVHeader {
			audio_format,
			channel_count,
			sampling_rate,
			bytes_per_second,
			bytes_per_sample,
			bits_per_sample,
		}
	}
}

impl From<&[u8]> for WAVHeader {
	/// Converts a slice of raw bytes into a WAVHeader object.
	/// 
	/// # Panics
	/// 
	/// This function will panic if the given slice is smaller than 16 bytes.
	/// 
	/// # Parameters
	/// 
	/// * `v` - The slice to convert from.
	fn from(v:&[u8]) -> Self {
		let mut a:[u8;16] = [0;16];
		a.copy_from_slice(&v[0..16]);
		WAVHeader::from(a)
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
/// use ocae::{TrackT,generators::{Generator,noise::Noise}};
/// let mut n = Noise::new();
/// let mut t = TrackT::new();
///
/// for _ in 0..ocae::SAMPLE_RATE {
/// 	t.push(n.process());
/// }
///
/// ocae::tools::write_wav(t, String::from(".junk/some/path/noise.wav"));
/// ```
pub fn write_wav(track:TrackT, path:String) -> std::io::Result<()> {
	use std::fs::File;

	match path.rfind('/') {
		Some(i) => std::fs::create_dir_all(String::from(path.split_at(i).0))?,
		None => ()
	};

	let w_id = riff::ChunkId::new("WAVE").unwrap();

	let h_id = riff::ChunkId::new("fmt ").unwrap();
	let h_vec:[u8;16] = WAVHeader::new(1, 2, SAMPLE_RATE as u32, 16).into();
	let h_dat = riff::Chunk::new_data(h_id, Vec::from(&h_vec[0..16]));

	let d_id = riff::ChunkId::new("data").unwrap();
	let mut d_vec = Vec::new();
	for i in track {
		let mut v:Vec<u8> = i.into();
		d_vec.append(&mut v);
	}
	let d_dat = riff::Chunk::new_data(d_id, d_vec);

	let r = riff::Chunk::new_riff(w_id, vec![h_dat, d_dat]);

	let mut f = File::create(path)?;

	riff::write_chunk(&mut f, &r)?;

	Ok(())
}

pub mod resampler;
