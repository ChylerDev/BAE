extern crate bae_rs;

#[cfg(test)]
mod tests {
    use std::fs::File;
    use bae_rs::{*, utils::*};

    const SAMPLE_RATE: usize = 48_000;
    const INV_SAMPLE_RATE: MathT = 1.0 / SAMPLE_RATE as MathT;

    #[test]
    fn test_write_wav() -> Result<(),()> {
        let mut t = bae_rs::SampleTrackT::new();

        for i in 0..16 {
            t.push((2.0 * std::f32::consts::PI * 440.0 * i as f32 * INV_SAMPLE_RATE as f32).sin());
        }

        std::fs::create_dir_all(".junk/utils").unwrap();
        WaveWriteOptions::new()
            .bps(24)?
            .r(SAMPLE_RATE as MathT)
            .clip(true)
            .write(vec![t], &mut File::create(".junk/utils/wavwrite.wav").unwrap()).unwrap();

        Ok(())
    }

    #[test]
    fn test_mono_resampler() {
        use bae_rs::{SampleFormat, Mono};
        use bae_rs::debug::*;

        let sam = vec![Mono::from_sample(0.0), Mono::from_sample(1.0), Mono::from_sample(2.0), Mono::from_sample(3.0)];

        let mut r = MonoResampler::new(sam.clone(), SAMPLE_RATE as MathT, SAMPLE_RATE as MathT / 2.0, 0,0);
        for i in 0..7 {
            let s = r.process();

            assert!(float_equal(s, i as SampleT/2.0, std::f32::EPSILON, |x| x.abs()));
        }

        let mut r = MonoResampler::new(sam.clone(), SAMPLE_RATE as MathT, SAMPLE_RATE as MathT * 2.0, 0,0);
        for i in 0..2 {
            let s = r.process();

            assert!(float_equal(s, (i*2) as SampleT, std::f32::EPSILON, |x| x.abs()));
        }

        let mut r = MonoResampler::new(sam.clone(), SAMPLE_RATE as MathT, SAMPLE_RATE as MathT, 0,0);
        r.set_playback_speed(0.5);
        for i in 0..7 {
            let s = r.process();

            assert!(float_equal(s, i as SampleT/2.0, std::f32::EPSILON, |x| x.abs()));
        }

        let mut r = MonoResampler::new(sam.clone(), SAMPLE_RATE as MathT, SAMPLE_RATE as MathT * 2.0, 0,0);
        r.set_playback_speed(0.5);
        for i in sam {
            let s = r.process();

            assert!(float_equal(s, i.into_sample(), std::f32::EPSILON, |x| x.abs()));
        }
    }
}
