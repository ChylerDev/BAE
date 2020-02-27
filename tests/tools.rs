extern crate bae;

#[cfg(test)]
mod tests {
	#[test]
	fn test_write_wav() {
		use bae::tools::*;

		let mut t = bae::TrackT::new();

		for i in 0..16 {
			t.push((2.0 * std::f32::consts::PI * 440.0 * i as f32 * bae::INV_SAMPLE_RATE as f32).sin());
		}

		write_wav(t, ".junk/tools/wavwrite.wav").unwrap();
	}

	#[test]
	fn test_resampler() {
		use bae::tools::*;
		use std::f32::EPSILON;

		let sam = vec![0.0, 1.0, 2.0, 3.0];

		let mut r = MonoResampler::new(sam.clone(), bae::SAMPLE_RATE/2, 0,0);
		for i in 0..7 {
			let s = r.process();

			assert!((s - i as bae::SampleT/2.0).abs() < EPSILON);
		}

		let mut r = MonoResampler::new(sam.clone(), bae::SAMPLE_RATE * 2, 0,0);
		for i in 0..2 {
			let s = r.process();

			assert!((s - (i * 2) as bae::SampleT).abs() < EPSILON);
		}

		let mut r = MonoResampler::new(sam.clone(), bae::SAMPLE_RATE, 0,0);
		r.set_playback_speed(0.5);
		for i in 0..7 {
			let s = r.process();

			assert!((s - i as bae::SampleT/2.0).abs() < EPSILON);
		}

		let mut r = MonoResampler::new(sam.clone(), bae::SAMPLE_RATE * 2, 0,0);
		r.set_playback_speed(0.5);
		for i in sam {
			let s = r.process();

			assert!((s - i).abs() < EPSILON);
		}
	}
}
