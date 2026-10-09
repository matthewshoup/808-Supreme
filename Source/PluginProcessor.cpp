#include "PluginProcessor.h"
#include "PluginEditor.h"

Supreme808AudioProcessor::Supreme808AudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout Supreme808AudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("tune", "Tune",
        juce::NormalisableRange<float>(35.0f, 90.0f, 0.1f), 55.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("decay", "Decay",
        juce::NormalisableRange<float>(80.0f, 2000.0f, 1.0f, 0.45f), 650.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("level", "Level",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.8f));
    return layout;
}

void Supreme808AudioProcessor::prepareToPlay(double sampleRate, int)
{
    kick.prepare(sampleRate);
}

bool Supreme808AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto out = layouts.getMainOutputChannelSet();
    return out == juce::AudioChannelSet::mono() || out == juce::AudioChannelSet::stereo();
}

void Supreme808AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    kick.setTune(parameters.getRawParameterValue("tune")->load());
    kick.setDecay(parameters.getRawParameterValue("decay")->load());
    kick.setLevel(parameters.getRawParameterValue("level")->load());

    auto metadata = midi.begin();
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        while (metadata != midi.end() && metadata->samplePosition <= sample)
        {
            const auto message = metadata->getMessage();
            if (message.isNoteOn() && message.getNoteNumber() == 36)
                kick.trigger(message.getFloatVelocity());
            ++metadata;
        }

        const float value = kick.processSample();
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
            buffer.setSample(channel, sample, value);
    }
}

void Supreme808AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto xml = parameters.copyState().createXml())
        copyXmlToBinary(*xml, destData);
}

void Supreme808AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessorEditor* Supreme808AudioProcessor::createEditor()
{
    return new Supreme808AudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Supreme808AudioProcessor();
}
