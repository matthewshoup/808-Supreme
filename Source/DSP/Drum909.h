#pragma once
#include <JuceHeader.h>
#include <array>
#include <cstdint>

class Drum909
{
public:
    enum class Voice { Kick, Snare, Clap, ClosedHat, OpenHat, LowTom, MidTom, HighTom, Rimshot, Cowbell, Cymbal, Count };
    void prepare(double newSampleRate);
    void trigger(Voice voice, float velocity);
    float processSample();
private:
    struct State { float env=0.0f, aux=0.0f, velocity=1.0f; double phase1=0.0, phase2=0.0; };
    std::array<State, static_cast<size_t>(Voice::Count)> states {};
    double sampleRate=44100.0;
    uint32_t noiseState=0x90909090u;
    float noise();
    float osc(double& phase,float hz);
    float decay(float seconds) const;
};