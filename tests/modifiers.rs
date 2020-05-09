extern crate bae_rs;

#[cfg(test)]
mod tests {
    use std::fs::File;
    use std::time::Duration;
    use bae_rs::{modifiers::*, generators::*, utils::*};

    const FILE_PREFIX: &'static str = ".junk/modifiers/";

        #[test]
    fn test_adsr() {
        let mut a = ADSR::new(
            Duration::from_secs_f64(0.03125),
            Duration::from_secs_f64(0.125),
            linear_to_db(0.5),
            Duration::from_secs_f64(0.5)
        );

        let mut g = Sine::new(440.0);
        let mut t = bae_rs::SampleTrackT::new();

        for i in 0..bae_rs::SAMPLE_RATE {
            if i == bae_rs::SAMPLE_RATE/2 {
                a.release();
            }

            t.push(a.process(g.process()));
        }

        let f = ".junk/modifiers/adsr.wav";
        write_wav(vec![t], 24, &mut File::create(f).unwrap(), false).unwrap();
    }

    #[test]
    fn test_bandpass() {
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
        let mut d = Delay::new(std::time::Duration::from_secs_f64(0.5));

        run_modifier(&mut d, "delay.wav");
    }

    #[test]
    fn test_echo() {
        let mut e = Echo::new(std::time::Duration::from_secs_f64(0.25), 0.5);

        let mut g = Sine::new(440.0);
        let mut t = bae_rs::SampleTrackT::new();

        for _ in 0..bae_rs::SAMPLE_RATE {
            t.push(e.process(g.process()*0.5));
        }

        let f = ".junk/modifiers/echo.wav";
        write_wav(vec![t], 24, &mut File::create(f).unwrap(), false).unwrap();
    }

    #[test]
    fn test_envelope() {
        let mut e = Envelope::new(20.0, 20_000.0);

        let mut a = ADSR::new(
            Duration::from_secs_f64(0.03125),
            Duration::from_secs_f64(0.125),
            linear_to_db(0.5),
            Duration::from_secs_f64(0.5)
        );
        let mut g = Sine::new(440.0);
        let mut t = bae_rs::SampleTrackT::new();

        for i in 0..bae_rs::SAMPLE_RATE {
            if i == bae_rs::SAMPLE_RATE/2 {
                a.release();
            }

            t.push(e.process(a.process(g.process())));
        }

        let f = ".junk/modifiers/envelope.wav";
        write_wav(vec![t], 24, &mut File::create(f).unwrap(), false).unwrap();
    }

    #[test]
    fn test_gain() {
        let mut g = Gain::new(0.125);

        run_modifier(&mut g, "gain.wav");
    }

    #[test]
    fn test_generic() {
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

        let mut s = Sine::new(440.0);
        let mut t = bae_rs::SampleTrackT::new();

        for _ in 0..bae_rs::SAMPLE_RATE {
            t.push(g.process(s.process() * 0.25));
        }

        let f = ".junk/modifiers/generic.wav";
        write_wav(vec![t], 24, &mut File::create(f).unwrap(), false).unwrap();
    }

    #[test]
    fn test_highpass() {
        let mut hp = HighPass::new(440.0, 1.0);

        run_modifier(&mut hp, "highpass.wav");
    }

    #[test]
    fn test_lowpass() {
        let mut lp = LowPass::new(440.0, 0.0);
        let mut n = Noise::new();
        let mut t = bae_rs::SampleTrackT::new();

        for i in 0..8*bae_rs::SAMPLE_RATE {
            lp.set_resonance(i as bae_rs::MathT / (8*bae_rs::SAMPLE_RATE) as bae_rs::MathT);

            t.push(lp.process(n.process()));
        }

        let f = ".junk/modifiers/lowpass.wav";
        write_wav(vec![t], 24, &mut File::create(f).unwrap(), false).unwrap();
    }

    fn run_modifier(m: &mut dyn bae_rs::modifiers::Modifier, file:&str)
    {
        let mut g = Noise::new();
        let mut t = bae_rs::SampleTrackT::new();

        let before = std::time::Instant::now();

        for _ in 0..bae_rs::SAMPLE_RATE {
            t.push(m.process(g.process()));
        }

        let after = std::time::Instant::now();
        let duration = after - before;
        println!("Test generated 1s of audio in {} seconds", duration.as_secs_f32());

        let mut f = String::from(FILE_PREFIX);
        f.push_str(file);

        write_wav(vec![t], 24, &mut File::create(f.as_str()).unwrap(), false).unwrap();
    }
}
