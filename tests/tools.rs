extern crate bae_rs;

#[cfg(test)]
mod tests {
    use std::fs::File;
    use bae_rs::tools::*;

    #[test]
    fn test_write_wav() {

        let mut t = bae_rs::TrackT::new();

        for i in 0..16 {
            t.push((2.0 * std::f32::consts::PI * 440.0 * i as f32 * bae_rs::INV_SAMPLE_RATE as f32).sin());
        }

        write_wav(vec![t], 24, &mut File::create(".junk/tools/wavwrite.wav").unwrap()).unwrap();
    }

    #[test]
    fn test_resampler() {
        use bae_rs::SampleT;
        use bae_rs::tools::*;

        let sam = vec![0.0, 1.0, 2.0, 3.0];

        let mut r = MonoResampler::new(sam.clone(), bae_rs::SAMPLE_RATE as bae_rs::MathT / 2.0, 0,0);
        for i in 0..7 {
            let s = r.process();

            assert!(f32_equal(s, i as SampleT/2.0));
        }

        let mut r = MonoResampler::new(sam.clone(), bae_rs::SAMPLE_RATE as bae_rs::MathT * 2.0, 0,0);
        for i in 0..2 {
            let s = r.process();

            assert!(f32_equal(s, (i*2) as SampleT));
        }

        let mut r = MonoResampler::new(sam.clone(), bae_rs::SAMPLE_RATE as bae_rs::MathT, 0,0);
        r.set_playback_speed(0.5);
        for i in 0..7 {
            let s = r.process();

            assert!(f32_equal(s, i as SampleT/2.0));
        }

        let mut r = MonoResampler::new(sam.clone(), bae_rs::SAMPLE_RATE as bae_rs::MathT * 2.0, 0,0);
        r.set_playback_speed(0.5);
        for i in sam {
            let s = r.process();

            assert!(f32_equal(s, i));
        }
    }

    fn f32_equal(a: f32, b: f32) -> bool {
        use std::f32::EPSILON;

        (a-b).abs() < EPSILON
    }
}
