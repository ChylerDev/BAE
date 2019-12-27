use std::vec::Vec;

pub mod generator;
pub mod tools;

pub type MathT = f64;
pub type SampleT = f32;
pub type TrackT = std::vec::Vec<StereoData>;

pub const SAMPLE_RATE:u64 = 48000;
pub const INV_SAMPLE_RATE:MathT = 1.0/(SAMPLE_RATE as MathT);

#[derive(Debug,Copy,Clone)]
pub struct StereoData{
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
            left: SampleT::sqrt(0.5)*x,
            right: SampleT::sqrt(0.5)*x
        }
    }

    pub fn to_mono(&self) -> SampleT {
        (self.left + self.right)/SampleT::sqrt(0.5)
    }
}

impl Into<Vec<u8>> for StereoData {
    fn into(self) -> Vec<u8> {
        let mut v = Vec::new();

        let n = ((self.left * 0x8000 as SampleT) as i16).to_le_bytes();
        v.push(n[0]);
        v.push(n[1]);

        let n = ((self.right * 0x8000 as SampleT) as i16).to_le_bytes();
        v.push(n[0]);
        v.push(n[1]);

        v
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
