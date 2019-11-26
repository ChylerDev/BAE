# OCAE Changelog

## Version 0.9.0

Major refactoring. Removed dependency on PortAudio, freeing the pure generation of audio data from any sort of output requirements, potentially extending the use of this library beyond real-time applications.

Documentation is incomplete but mostly in place, and the only current example code is contained within the test main.cpp. This will be changed by 1.0.0 release, which is planned for the end of the year.

## Version 0.0.1

This is the initial alpha release of this audio engine. Everything is still very rough and could break at any point and good performance should not be expected. As of right now I can run multiple sounds at the same time without losing real-time execution of the sound loop.

There is no documentation for now, however for future revisions (most likely starting at v1.0.0) I will have a PDF for the documentation of the engine similar to what I have for my other projects that have moved past 0.* versions.
