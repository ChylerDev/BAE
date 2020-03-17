# BAE Rust Changelog

* Updated `wav` dependency to 0.3, and mirrored the upgrade to relavant systems, moving away from Paths and Files to io::Read and io::Write.

## Version 0.11.0

* Renamed `StereoData` to `Stereo`.
* Removed `riff` crate dependency, reworked `write_wav` to use `wav::write_wav` and handle multiple channels.
* Reworked `read_wav` to work with any number of channels.
* Moved the `Sound` trait into `src/sounds/mod.rs` and deleted `src/sounds/sound.rs`.
* Renamed `Block` to `StandardBlock` and `BasicBlock` to `Block`.
* Moved `Block` into `src/sounds/mod.rs` and deleted `src/sounds/sound.rs`.
* Fixed bug in `normalize` function.

## Version 0.10.0

* Reorganized project to use trait objects instead of CRTP everywhere.
* Added/moved some utility functions to the `tools` module.
* Removed `Name` debug trait.
* Fixed `MonoWav`.

## Version 0.9.3

* Initial release
