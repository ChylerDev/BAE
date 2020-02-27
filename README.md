This project is currently being reimplemented in [rust](https://rust-lang.org), view the progress in its branch [here](tree/8-move-to-rust)

# Overly Complicated Audio Engine (OCAE)

An audio engine designed to produce audio samples using basic generators (sound sources) and modifiers (filters).

# Building

Building is done through CMake, as it is the solition I am familiar with enough to set up for multi-platform C++ projects.

Be sure you have CMake v3.8 or newer installed.

Set the repository root as the source code location, and then set the build
location of your choice.

Configure, check the options to see what (if anything) you'd like to change, then configure again.

Once you're ready to generated the project files, use the Generate command.

Currently the library has only been tested on Windows 10 (Visual Studio 2019) and Ubuntu (WSL2, GCC 7.4.0/LLVM clang 6.0.0)

# Dependencies

This project uses the [RIFF-Util library](https://gitlab.com/ChylerDev/RIFF-Util) to read and write WAVE data structures (whether loaded from disk or not).

Currently there is no option to disable it if it isn't needed, but this may be an added feature in the future.

# Usage

It should be noted that the factories within this library use std::shared_ptr, which means that any pointer-level copying will not copy the managed objects. This means that improper care of copying could yield to, for example, a generator running twice per process loop which is likely not the intended use. This could be fairly easily mitigated however by modifying the define in Macro.hpp or adding your own define before it and rebuilding the project.

# Notes

The project will build the executibles and copy the necessary shared libs (if any) to repo/bin for easy access of the binaries.

# Future Expansion

* Modifiers to add:
	* Pan
	* Fade in/out
* Features:
	* Side-chain
	* FFT?
