# 808 Supreme

**Let the Bass Reign Supreme.**

808 Supreme is a JUCE/C++ drum-synthesis plugin inspired by classic analog rhythm-machine synthesis. It generates its sound in real time; no kick samples are used.

## v0.1
- VST3 and Standalone targets
- Synthesized kick voice
- MIDI note 36 trigger
- Tune, Decay, and Level controls
- Velocity-sensitive level/accent response

## Build
JUCE is fetched automatically by CMake.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Requires CMake 3.22+ and a supported C++17 toolchain. On macOS, Xcode command-line tools are required.

This project is independently developed and is not affiliated with or endorsed by Roland Corporation.
