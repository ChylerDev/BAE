//! # Tools
//! 
//! Module for audio related functions that make life easier.

use super::*;
use std::vec::Vec;

/// Structure for the "fmt " chunk of wave files, specifying key information
/// about the enclosed data.
pub struct WAVHeader {
	audio_format:u16,
	channel_count:u16,
	sampling_rate:u32,
	bytes_per_second:u32,
	bytes_per_sample:u16,
	bits_per_sample:u16,
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

impl Into<Vec<u8>> for WAVHeader {
	/// Converts the WAVHeader object into a vector of its bytes
	/// 
	/// # Example
	/// 
	/// ```
	/// let h_vec:Vec<u8> = ocae::tools::WAVHeader::new(1, 2, ocae::SAMPLE_RATE as u32, 16).into();
	/// ```
	fn into(self) -> Vec<u8> {
		let mut v = Vec::new();

		v.extend_from_slice(&self.audio_format.to_le_bytes());
		v.extend_from_slice(&self.channel_count.to_le_bytes());
		v.extend_from_slice(&self.sampling_rate.to_le_bytes());
		v.extend_from_slice(&self.bytes_per_second.to_le_bytes());
		v.extend_from_slice(&self.bytes_per_sample.to_le_bytes());
		v.extend_from_slice(&self.bits_per_sample.to_le_bytes());

		v
	}
}

/// Takes the given track and filename and writes the track data to the wavefile
/// at the given location.
/// 
/// # Parameters
/// 
/// * `track` - The track to write.
/// * `filename` - The path to the file to write to.
/// 
/// # Example
/// 
/// ```
/// use ocae::generators::Generator;
/// let mut n = ocae::generators::noise::Noise::new();
/// let mut t = ocae::TrackT::new();
///
/// for _ in 0..ocae::SAMPLE_RATE {
///	 t.push(n.process());
/// }
///
/// ocae::tools::write_wav(t, String::from(".junk/some/path/noise.wav"));
/// ```
pub fn write_wav(track:TrackT, filename:String) {
	use std::fs::File;

	let w_id = riff::ChunkId::new("WAVE").unwrap();

	let h_id = riff::ChunkId::new("fmt ").unwrap();
	let h_vec:Vec<u8> = WAVHeader::new(1, 2, SAMPLE_RATE as u32, 16).into();
	let h_dat = riff::Chunk::new_data(h_id, h_vec);

	let d_id = riff::ChunkId::new("data").unwrap();
	let mut d_vec = Vec::new();
	for i in track {
		let mut v:Vec<u8> = i.into();
		d_vec.append(&mut v);
	}
	let d_dat = riff::Chunk::new_data(d_id, d_vec);

	let r = riff::Chunk::new_riff(w_id, vec![h_dat, d_dat]);

	match filename.rfind('/') {
		Some(i) => std::fs::create_dir_all(String::from(filename.split_at(i).0)).unwrap(),
		None => ()
	};

	let mut f = File::create(filename).unwrap();

	riff::write_chunk(&mut f, &r).unwrap();
}

pub mod resampler;
