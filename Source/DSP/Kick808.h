#pragma once
#include <JuceHeader.h>

class Kick808
{
public:
    void prepare(double newSampleRate);
    void trigger(float velocity);
    float processSample();

    void setTune(float hz) noexcept { tuneHz = juce::jlimit(35.0f, 90.0f, hz); }
    void setDecay(float ms) noexcept { decayMs = juce::jlimit(80.0f, 2000.0f, ms); }
    void setLevel(float value) noexcept { level = juce::jlimit(0.0f, 1.0f, value); }

private:
    double sampleRate = 44100.0;
    double phase = 0.0;
    float tuneHz = 55.0f;
    float decayMs = 650.0f;
    float level = 0.8f;
    float amplitude = 0.0f;
    float pitchEnvelope = 0.0f;
    float velocityGain = 1.0f;
};
