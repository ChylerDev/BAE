extern crate bae_rs;

#[cfg(test)]
mod tests {
	#[test]
	fn test_adsr() {
		use bae_rs::modifiers::*;
		use bae_rs::generators::*;
		use bae_rs::tools::*;
		use std::time::Duration;

		let mut a = ADSR::new(
			Duration::from_secs_f64(0.03125),
			Duration::from_secs_f64(0.125),
			linear_to_db(0.5),
			Duration::from_secs_f64(0.5)
		);

		let mut g = Sine::new(440.0);
		let mut t = bae_rs::TrackT::new();

		for i in 0..bae_rs::SAMPLE_RATE {
			if i == bae_rs::SAMPLE_RATE/2 {
				a.release();
			}

			t.push(a.process(g.process()));
		}

		let f = ".junk/modifiers/adsr.wav";
		bae_rs::tools::write_wav(vec![t], 24, f).unwrap();

		// bae_rs::tools::filter_gain(
		// 	|| ADSR::new(
		// 		Duration::from_secs_f64(0.03125),
		// 		Duration::from_secs_f64(0.125),
		// 		bae_rs::linear_db(0.5),
		// 		Duration::from_secs_f64(0.5)
		// 	),
		// 	512
		// );
	}

	#[test]
	fn test_bandpass() {
		use bae_rs::modifiers::*;

		let mut bp1 = BandPass::from_corners((100.0,200.0));
		let f1 = "bandpass_100_200.wav";
		let mut bp2 = BandPass::from_corners((200.0,225.0));
		let f2 = "bandpass_200_225.wav";
		let mut bp3 = BandPass::from_corners((20.0,20000.0));
		let f3 = "bandpass_20_20k.wav";

		run_modifier(&mut bp1, f1);
		run_modifier(&mut bp2, f2);
		run_modifier(&mut bp3, f3);

		// bae_rs::tools::filter_gain(|| BandPass::from_corners((200.0,4000.0)), 512);
	}

	#[test]
	fn test_delay() {
		use bae_rs::modifiers::*;

		let mut d = Delay::new(std::time::Duration::from_secs_f64(0.5));

		run_modifier(&mut d, "delay.wav");
	}

	#[test]
	fn test_echo() {
		use bae_rs::modifiers::*;
		use bae_rs::generators::*;

		let mut e = Echo::new(std::time::Duration::from_secs_f64(0.25), 0.5);

		let mut g = Sine::new(440.0);
		let mut t = bae_rs::TrackT::new();

		for _ in 0..bae_rs::SAMPLE_RATE {
			t.push(e.process(g.process()*0.5));
		}

		let f = ".junk/modifiers/echo.wav";
		bae_rs::tools::write_wav(vec![t], 24, f).unwrap();
	}

	#[test]
	fn test_envelope() {
		use bae_rs::modifiers::*;
		use bae_rs::generators::*;
		use bae_rs::tools::*;
		use std::time::Duration;

		let mut e = Envelope::new(20.0, 20_000.0);

		let mut a = ADSR::new(
			Duration::from_secs_f64(0.03125),
			Duration::from_secs_f64(0.125),
			linear_to_db(0.5),
			Duration::from_secs_f64(0.5)
		);
		let mut g = Sine::new(440.0);
		let mut t = bae_rs::TrackT::new();

		for i in 0..bae_rs::SAMPLE_RATE {
			if i == bae_rs::SAMPLE_RATE/2 {
				a.release();
			}

			t.push(e.process(a.process(g.process())));
		}

		let f = ".junk/modifiers/envelope.wav";
		bae_rs::tools::write_wav(vec![t], 24, f).unwrap();
	}

	#[test]
	fn test_gain() {
		use bae_rs::modifiers::*;

		let mut g = Gain::new(0.125);

		run_modifier(&mut g, "gain.wav");
	}

	#[test]
	fn test_generic() {
		use bae_rs::modifiers::*;

		let mut g = Generic::new(
			{
				let mut v = Zeros::new();

				v.push_back((0, 0.69));
				v.push_back((1, 0.32));
				v.push_back((2, 0.13));
				v.push_back((6, 0.4892));

				v
			},
			{
				let mut v = Poles::new();

				v.push_back((1, 0.378_946_24));
				v.push_back((5, 0.125_012_84));
				v.push_back((300, 0.104_536_59));

				v
			},
		);

		use bae_rs::generators::*;
		let mut s = Sine::new(440.0);
		let mut t = bae_rs::TrackT::new();

		for _ in 0..bae_rs::SAMPLE_RATE {
			t.push(g.process(s.process() * 0.25));
		}

		let f = ".junk/modifiers/generic.wav";
		bae_rs::tools::write_wav(vec![t], 24, f).unwrap();
	}

	#[test]
	fn test_highpass() {
		use bae_rs::modifiers::*;

		let mut hp = HighPass::new(440.0, 1.0);

		run_modifier(&mut hp, "highpass.wav");

		// bae_rs::tools::filter_gain(|| HighPass::new(4400.0, 1.0), 512);
	}

	#[test]
	fn test_lowpass() {
		use bae_rs::{generators::*, modifiers::*};

		let mut lp = LowPass::new(440.0, 0.0);
		let mut n = Noise::new();
		let mut t = bae_rs::TrackT::new();

		for i in 0..8*bae_rs::SAMPLE_RATE {
			lp.set_resonance(i as bae_rs::MathT / (8*bae_rs::SAMPLE_RATE) as bae_rs::MathT);

			t.push(lp.process(n.process()));
		}

		let f = ".junk/modifiers/lowpass.wav";
		bae_rs::tools::write_wav(vec![t], 24, f).unwrap();
	}

	fn run_modifier(m: &mut dyn bae_rs::modifiers::Modifier, file:&str)
	{
		use bae_rs::generators::*;

		let mut g = Noise::new();
		let mut t = bae_rs::TrackT::new();

		let before = std::time::Instant::now();

		for _ in 0..bae_rs::SAMPLE_RATE {
			t.push(m.process(g.process()));
		}

		let after = std::time::Instant::now();
		let duration = after - before;
		println!("Test generated 1s of audio in {} seconds", duration.as_secs_f32());

		let mut f = String::from(".junk/modifiers/");
		f.push_str(file);

		bae_rs::tools::write_wav(vec![t], 24, f.as_str()).unwrap();
	}
}
