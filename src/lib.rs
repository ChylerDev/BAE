pub type MathT = f64;
pub type SampleT = f32;

pub const SAMPLE_RATE:usize = 48000;
pub const INV_SAMPLE_RATE:MathT = 1.0/(SAMPLE_RATE as MathT);

#[derive(Debug)]
struct StereoData{
    left:SampleT,
    right:SampleT,
}

impl StereoData {
    pub fn from(l:SampleT, r:SampleT) -> StereoData {
        StereoData{
            left:l,
            right:r
        }
    }

    pub fn from_mono(x:SampleT) -> StereoData {
        StereoData{
            left: SampleT::log10(0.5)*x,
            right: SampleT::log10(0.5)*x
        }
    }

    pub fn to_mono(&self) -> SampleT {
        (self.left + self.right)/SampleT::log10(0.5)
    }
}

impl Default for StereoData {
    fn default() -> Self {
        StereoData{
            left:0.0,
            right:0.0
        }
    }
}
