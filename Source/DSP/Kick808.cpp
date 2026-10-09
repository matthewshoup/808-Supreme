#include "Kick808.h"
#include <cmath>

void Kick808::prepare(double newSampleRate)
{
    sampleRate = newSampleRate > 0.0 ? newSampleRate : 44100.0;
    phase = 0.0;
    amplitude = 0.0f;
    pitchEnvelope = 0.0f;
}

void Kick808::trigger(float velocity)
{
    velocityGain = juce::jlimit(0.0f, 1.0f, velocity);
    amplitude = 1.0f;
    pitchEnvelope = 1.0f;
    phase = 0.0;
}

float Kick808::processSample()
{
    if (amplitude < 0.00001f)
        return 0.0f;

    const float pitchHz = tuneHz + (115.0f * pitchEnvelope);
    phase += juce::MathConstants<double>::twoPi * pitchHz / sampleRate;
    if (phase >= juce::MathConstants<double>::twoPi)
        phase -= juce::MathConstants<double>::twoPi;

    const float body = std::sin((float) phase);
    const float ampTimeSeconds = decayMs * 0.001f;
    const float ampCoeff = std::exp(-1.0f / (ampTimeSeconds * (float) sampleRate));
    const float pitchCoeff = std::exp(-1.0f / (0.022f * (float) sampleRate));

    const float accent = 0.65f + (0.35f * velocityGain);
    const float output = body * amplitude * level * accent;

    amplitude *= ampCoeff;
    pitchEnvelope *= pitchCoeff;
    return output;
}
