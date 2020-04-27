extern crate bae_rs;

#[cfg(test)]
mod tests {
    use bae_rs::generators::*;
    use bae_rs::debug::*;

    #[test]
    fn test_empty() {
        let mut z = Zero::new();

        for _ in 0..bae_rs::SAMPLE_RATE {
            assert!(float_equal(z.process(), 0.0, std::f32::EPSILON, |x| x.abs()));
        }
    }

    // #[test]
    // fn test_noise() {
    //     use fftw::array::AlignedVec;
    //     use fftw::plan::*;
    //     use fftw::types::*;
    //     use std::f32::consts::PI;

    //     let n = 128;
    //     let mut plan: R2CPlan32 = R2CPlan32::aligned(&[n], Flag::Measure).unwrap();
    // }

    #[test]
    fn test_sawtooth() {
        let mut s = Sawtooth::new(440.0);
        assert!(float_equal(440.0, s.get_frequency(), std::f64::EPSILON, |x| x.abs()));

        let time = std::time::Duration::from_secs_f64(1.0/s.get_frequency());
        let samples = bae_rs::utils::seconds_to_samples(time);

        let omega = |t: f32, x: f32| 2.0 / t * x;
        let phi = |t: f32, x: f32| -2.0 * (x / t + 0.5).floor();

        println!("Samples: {}", samples);
        for i in 0..samples {
            let y = s.process();
            let o = omega(
                1.0/s.get_frequency() as f32,
                i as f32 * bae_rs::INV_SAMPLE_RATE as f32
            );
            let p = phi(
                1.0/s.get_frequency() as f32,
                i as f32 * bae_rs::INV_SAMPLE_RATE as f32
            );
            let n = o + p;
            assert!(float_equal(y, n, std::f32::EPSILON * 10.0, |x| x.abs()));
        }
    }

    #[test]
    fn test_sine() {
        let mut s = Sine::new(440.0);
        assert!(float_equal(440.0, s.get_frequency(), std::f64::EPSILON, |x| x.abs()));

        let time = std::time::Duration::from_secs_f64(1.0/s.get_frequency());
        let samples = bae_rs::utils::seconds_to_samples(time);

        let omega = |f: f32, i: f32| f * 2.0 * std::f32::consts::PI * bae_rs::INV_SAMPLE_RATE as f32 * i;

        for i in 0..samples {
             let y = s.process();
             let n = omega(s.get_frequency() as f32, i as f32).sin();
             assert!(float_equal(y, n, std::f32::EPSILON * 10.0, |x| x.abs()));
        }
    }

    #[test]
    fn test_square() {
        let mut s = Square::new(440.0);
        assert!(float_equal(440.0, s.get_frequency(), std::f64::EPSILON, |x| x.abs()));

        let time = std::time::Duration::from_secs_f64(1.0/s.get_frequency());
        let samples = bae_rs::utils::seconds_to_samples(time);

        let omega = (-2.0 * s.get_frequency() * bae_rs::INV_SAMPLE_RATE) as f32;

        for i in 0..samples {
            let y = s.process();
            let n = (omega * i as f32 + 1.0).ceil() * 2.0 - 1.0;
            assert!(float_equal(y, n, std::f32::EPSILON * 10.0, |x| x.abs()));
        }
    }

    // #[test]
    // fn test_triangle() {
    //     let mut t = Triangle::new(440.0);
    //     assert!(float_equal(440.0, t.get_frequency(), std::f64::EPSILON, |x| x.abs()));

    //     let time = std::time::Duration::from_secs_f64(1.0/t.get_frequency());
    //     let samples = bae_rs::utils::seconds_to_samples(time);

    //     let omega = |x: f32| 
    // }
}
