extern crate bae;

#[cfg(test)]
mod tests {
	#[test]
	fn test_empty() {
		use bae::generators::*;

		run_generator(&mut Zero::new(), "empty.wav");
	}

	#[test]
	fn test_noise() {
		use bae::generators::*;

		run_generator(&mut Noise::new(), "noise.wav");
	}

	#[test]
	fn test_sawtooth() {
		use bae::generators::*;

		run_generator(&mut Sawtooth::new(440.0), "sawtooth.wav");
	}

	#[test]
	fn test_sine() {
		use bae::generators::*;

		run_generator(&mut Sine::new(440.0), "sine.wav");
	}

	#[test]
	fn test_square() {
		use bae::generators::*;

		run_generator(&mut Square::new(440.0), "square.wav");
	}

	#[test]
	fn test_triangle() {
		use bae::generators::*;

		run_generator(&mut Triangle::new(440.0), "triangle.wav");
	}

	#[test]
	fn test_wavplayer() {
		todo!();
	}

	fn run_generator<G>(g: &mut G, file:&str)
		where G: Clone + bae::generators::Generator<G>
	{
		let mut t = bae::TrackT::new();
	
		for _ in 0..bae::SAMPLE_RATE {
			t.push(g.process());
		}
	
		let mut f = String::from(".junk/generators/");
		f.push_str(file);

		bae::tools::write_wav(t, f.as_str()).unwrap();
	}
}
