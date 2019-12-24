pub trait Generator {
    /// Generates a rendered audio sample
    pub fn Process(&self) -> StereoData {
        StereoData::default()
    }
}
