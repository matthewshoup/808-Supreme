#pragma once
#include <JuceHeader.h>
#include <array>
#include <cstdint>

class Drum808
{
public:
    enum class Voice { Snare, Clap, ClosedHat, OpenHat, LowTom, MidTom, HighTom,
                       LowConga, MidConga, HighConga, Cowbell, Rimshot, Claves,
                       Maracas, Cymbal, Count };

    void prepare(double newSampleRate);
    void trigger(Voice voice, float velocity);
    float processSample();

private:
    struct State { float env=0.0f, aux=0.0f, velocity=1.0f; double phase1=0.0, phase2=0.0; };
    std::array<State, static_cast<size_t>(Voice::Count)> states {};
    double sampleRate=44100.0;
    uint32_t noiseState=0x12345678u;

    float noise();
    float osc(double& phase, float hz);
    float decay(float seconds) const;
};
