#include "Drum808.h"
#include <cmath>

void Drum808::prepare(double sr) { sampleRate = sr > 0.0 ? sr : 44100.0; states = {}; }

float Drum808::noise()
{
    noiseState ^= noiseState << 13; noiseState ^= noiseState >> 17; noiseState ^= noiseState << 5;
    return (float)((noiseState & 0x00ffffffu) / 8388607.5 - 1.0);
}

float Drum808::osc(double& phase, float hz)
{
    phase += juce::MathConstants<double>::twoPi * hz / sampleRate;
    if (phase >= juce::MathConstants<double>::twoPi) phase -= juce::MathConstants<double>::twoPi;
    return std::sin((float)phase);
}

float Drum808::decay(float seconds) const { return std::exp(-1.0f / (seconds * (float)sampleRate)); }

void Drum808::trigger(Voice v, float velocity)
{
    auto& s = states[(size_t)v];
    s.env = 1.0f; s.aux = 1.0f; s.velocity = juce::jlimit(0.0f, 1.0f, velocity);
    s.phase1 = s.phase2 = 0.0;
}

float Drum808::processSample()
{
    float out = 0.0f;
    for (size_t i=0; i<states.size(); ++i)
    {
        auto v=(Voice)i; auto& s=states[i];
        if (s.env < 0.00001f) continue;
        float x=0.0f, d=0.99f;
        switch(v)
        {
            case Voice::Snare:
                x=(0.48f*osc(s.phase1,180.0f)+0.30f*osc(s.phase2,330.0f)+0.62f*noise())*s.env; d=decay(0.20f); break;
            case Voice::Clap:
            {
                float burst = s.aux > 0.72f ? 1.0f : (s.aux > 0.48f && s.aux < 0.58f ? 0.85f : (s.aux > 0.25f && s.aux < 0.34f ? 0.7f : 0.25f));
                x=noise()*burst*s.env; s.aux*=decay(0.045f); d=decay(0.24f); break;
            }
            case Voice::ClosedHat: x=noise()*osc(s.phase1,6310.0f)*s.env; d=decay(0.045f); break;
            case Voice::OpenHat: x=noise()*osc(s.phase1,6310.0f)*s.env; d=decay(0.42f); break;
            case Voice::LowTom: x=osc(s.phase1,105.0f)*s.env; d=decay(0.30f); break;
            case Voice::MidTom: x=osc(s.phase1,145.0f)*s.env; d=decay(0.26f); break;
            case Voice::HighTom: x=osc(s.phase1,205.0f)*s.env; d=decay(0.22f); break;
            case Voice::LowConga: x=osc(s.phase1,165.0f)*s.env; d=decay(0.18f); break;
            case Voice::MidConga: x=osc(s.phase1,230.0f)*s.env; d=decay(0.15f); break;
            case Voice::HighConga: x=osc(s.phase1,330.0f)*s.env; d=decay(0.13f); break;
            case Voice::Cowbell: x=(0.55f*(osc(s.phase1,540.0f)>0?1.0f:-1.0f)+0.45f*(osc(s.phase2,800.0f)>0?1.0f:-1.0f))*s.env; d=decay(0.20f); break;
            case Voice::Rimshot: x=(0.75f*osc(s.phase1,480.0f)+0.25f*noise())*s.env; d=decay(0.045f); break;
            case Voice::Claves: x=osc(s.phase1,2500.0f)*s.env; d=decay(0.035f); break;
            case Voice::Maracas: x=noise()*s.env; d=decay(0.055f); break;
            case Voice::Cymbal: x=(noise()*0.45f+osc(s.phase1,4100.0f)*osc(s.phase2,6200.0f)*0.55f)*s.env; d=decay(0.85f); break;
            default: break;
        }
        out += x * (0.45f + 0.55f*s.velocity) * 0.34f;
        s.env *= d;
    }
    return juce::jlimit(-1.0f,1.0f,out);
}
