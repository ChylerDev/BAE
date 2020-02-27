extern crate bae_rs;

#[cfg(test)]
mod tests {
	#[test]
	fn test_blocks() {
		use bae_rs::{generators::*, modifiers::*, sounds::{basic_block::*, block::*}};

		let mut b = GeneratorBlock::from_generator(Sine::new(440.0));
		let mut s = Sine::new(440.0);

		for _ in 0..bae_rs::seconds_to_samples(std::time::Duration::from_secs_f64(1.0/440.0)) {
			assert!((b.process() - s.process()).abs() < 1e-15);
		}

		let mut b = ModifierBlock::from_modifier(LowPass::new(440.0, 1.0));
		let mut n = Noise::new();

		let mut t = bae_rs::TrackT::new();

		for _ in 0..bae_rs::seconds_to_samples(std::time::Duration::from_secs_f64(0.5)) {
			b.prime_input(n.process());
			t.push(b.process());
		}

		bae_rs::tools::write_wav(t, ".junk/sounds/block_NoiseLP.wav")
		.expect("Failed to write wave file");
	}

	#[test]
	fn test_simple_sounds() {

	}

	#[test]
	fn test_complex_sounds() {

	}
}
