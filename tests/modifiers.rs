extern crate ocae;

#[cfg(test)]
mod tests {
	#[test]
	fn test_adsr() {
		use ocae::modifiers::{*, adsr::*};
		use std::time::Duration;

		let mut a = ADSR::new(
			Duration::from_secs_f64(0.03125),
			Duration::from_secs_f64(0.125),
			ocae::linear_db(0.5),
			Duration::from_secs_f64(0.5)
		);

		use ocae::generators::{Generator, FreqMod, sine::*};

		let mut g = Sine::new(440.0);
		let mut t = ocae::TrackT::new();

		for i in 0..ocae::SAMPLE_RATE {
			if i == ocae::SAMPLE_RATE/2 {
				a.release();
			}

			t.push(a.process(g.process()));
		}

		let f = ".junk/modifiers/adsr.wav";
		ocae::tools::write_wav(t, f).unwrap();
	}

	#[test]
	fn test_bandpass() {
		use ocae::modifiers::bandpass::*;

		let mut bp1 = BandPass::from_corners((100.0,200.0));
		let f1 = "bandpass_100_200.wav";
		let mut bp2 = BandPass::from_corners((200.0,225.0));
		let f2 = "bandpass_200_225.wav";
		let mut bp3 = BandPass::from_corners((20.0,20000.0));
		let f3 = "bandpass_20_20k.wav";

		run_modifier(&mut bp1, f1);
		run_modifier(&mut bp2, f2);
		run_modifier(&mut bp3, f3);
	}

	#[test]
	fn test_delay() {
		use ocae::modifiers::delay::*;

		let mut d = Delay::new(std::time::Duration::from_secs_f64(0.5));

		run_modifier(&mut d, "delay.wav");
	}

	#[test]
	fn test_echo() {
		use ocae::modifiers::{*, echo::*};
		use ocae::generators::{*, sine::*};

		let mut e = Echo::new(std::time::Duration::from_secs_f64(0.25), 0.5);

		let mut g = Sine::new(440.0);
		let mut t = ocae::TrackT::new();

		for _ in 0..ocae::SAMPLE_RATE {
			t.push(e.process(g.process()*0.5));
		}

		let f = ".junk/modifiers/echo.wav";
		ocae::tools::write_wav(t, f).unwrap();
	}

	#[test]
	fn test_highpass() {
		use ocae::modifiers::highpass::*;

		let mut hp = HighPass::new(440.0, 1.0);

		run_modifier(&mut hp, "highpass.wav");
	}

	fn run_modifier(m: &mut impl ocae::modifiers::Modifier, file:&str) {
		use ocae::generators::{Generator, noise::*};

		let mut g = Noise::new();
		let mut t = ocae::TrackT::new();

		let before = std::time::Instant::now();

		for _ in 0..ocae::SAMPLE_RATE {
			t.push(m.process(g.process()));
		}

		let after = std::time::Instant::now();
		let duration = after - before;
		println!("Test generated 1s of audio in {} seconds", duration.as_secs_f32());

		let mut f = String::from(".junk/modifiers/");
		f.push_str(file);

		ocae::tools::write_wav(t, f.as_str()).unwrap();
	}
}
