use super::*;

pub trait Generator {
    /// Generates a rendered audio sample
    fn process(&mut self) -> StereoData;
}

pub mod sine;
