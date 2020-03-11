extern crate bae_rs;

#[cfg(test)]
mod tests {
	use std::time::Duration;
	use bae_rs::{generators::*, modifiers::*, sounds::*};

	#[test]
	fn test_blocks() {
		let mut b = Block::from_generator(Sine::new(440.0));
		let mut s = Sine::new(440.0);

		for _ in 0..bae_rs::tools::seconds_to_samples(std::time::Duration::from_secs_f64(1.0/440.0)) {
			assert!((b.process() - s.process()).abs() < 1e-15);
		}

		let mut b = Block::from_modifier(LowPass::new(440.0, 1.0));
		let mut n = Noise::new();

		let mut t = bae_rs::TrackT::new();

		for _ in 0..bae_rs::tools::seconds_to_samples(Duration::from_secs_f64(0.5)) {
			b.prime_input(n.process());
			t.push(b.process());
		}

		bae_rs::tools::normalize(-1.5, &mut t);

		bae_rs::tools::write_wav(vec![t], 24, ".junk/sounds/block_NoiseLP.wav")
		.expect("Failed to write wav file");
	}

	#[test]
	fn test_simple_sounds() {
		let mut ss = SimpleSound::new(1.0, 0.5,
			BlockRc::new(Block::from_generator(Noise::new()))
		);
		ss.extend_modifiers(
			vec![
				BlockRc::new(
					Block::from_modifier(
						LowPass::new(440.0, 1.0)
					)
				),
				BlockRc::new(
					Block::from_modifier(
						HighPass::new(220.0, 1.0)
					)
				)
			]
		);

		let mut t = bae_rs::TrackT::new();

		for _ in 0..bae_rs::tools::seconds_to_samples(Duration::from_secs(4)) {
			t.push(ss.process(0.0));
		}

		bae_rs::tools::normalize(-1.5, &mut t);

		bae_rs::tools::write_wav(vec![t], 24, ".junk/sounds/simple_sounds.wav")
		.expect("Failed to write wav file");
	}

	#[test]
	fn test_complex_sounds() {
		let mut cs = ComplexSound::new(1.0, 1.0);

		let n = cs.add_block(
			BlockRc::new(
				Block::from_generator(Noise::new())
			)
		);
		let lp = cs.add_block(
			BlockRc::new(
				Block::from_modifier(LowPass::new(440.0, 1.0))
			)
		);
		let hp = cs.add_block(
			BlockRc::new(
				Block::from_modifier(HighPass::new(220.0, 1.0))
			)
		);

		cs.add_connection(cs.get_input_gain(), n);
		cs.add_connection(n, lp);
		cs.add_connection(lp, hp);
		cs.add_connection(hp, cs.get_output_gain());

		let mut t = bae_rs::TrackT::new();

		for _ in 0..bae_rs::tools::seconds_to_samples(Duration::from_secs(4)) {
			t.push(cs.process(0.0));
		}

		bae_rs::tools::normalize(-1.5, &mut t);

		bae_rs::tools::write_wav(vec![t], 24, ".junk/sounds/complex_sounds.wav")
		.expect("Failed to write wav file");
	}
}
