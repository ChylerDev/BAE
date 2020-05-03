extern crate bae_rs;

#[cfg(test)]
mod tests {
    use std::fs::File;
    use bae_rs::utils::*;

    #[test]
    fn test_write_wav() {

        let mut t = bae_rs::SampleTrackT::new();

        for i in 0..16 {
            t.push((2.0 * std::f32::consts::PI * 440.0 * i as f32 * bae_rs::INV_SAMPLE_RATE as f32).sin());
        }

        std::fs::create_dir_all(".junk/utils").unwrap();
        write_wav(vec![t], 24, &mut File::create(".junk/utils/wavwrite.wav").unwrap()).unwrap();
    }

    #[test]
    fn test_resampler() {
        use bae_rs::SampleT;
        use bae_rs::{SampleFormat, Mono};
        use bae_rs::debug::*;

        let sam = vec![Mono::from_sample(0.0), Mono::from_sample(1.0), Mono::from_sample(2.0), Mono::from_sample(3.0)];

        let mut r = MonoResampler::new(sam.clone(), bae_rs::SAMPLE_RATE as bae_rs::MathT / 2.0, 0,0);
        for i in 0..7 {
            let s = r.process();

            assert!(float_equal(s, i as SampleT/2.0, std::f32::EPSILON, |x| x.abs()));
        }

        let mut r = MonoResampler::new(sam.clone(), bae_rs::SAMPLE_RATE as bae_rs::MathT * 2.0, 0,0);
        for i in 0..2 {
            let s = r.process();

            assert!(float_equal(s, (i*2) as SampleT, std::f32::EPSILON, |x| x.abs()));
        }

        let mut r = MonoResampler::new(sam.clone(), bae_rs::SAMPLE_RATE as bae_rs::MathT, 0,0);
        r.set_playback_speed(0.5);
        for i in 0..7 {
            let s = r.process();

            assert!(float_equal(s, i as SampleT/2.0, std::f32::EPSILON, |x| x.abs()));
        }

        let mut r = MonoResampler::new(sam.clone(), bae_rs::SAMPLE_RATE as bae_rs::MathT * 2.0, 0,0);
        r.set_playback_speed(0.5);
        for i in sam {
            let s = r.process();

            assert!(float_equal(s, i.into_sample(), std::f32::EPSILON, |x| x.abs()));
        }
    }
}
