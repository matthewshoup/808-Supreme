#pragma once
#include <JuceHeader.h>
#include "DSP/Kick808.h"
#include "DSP/Drum808.h"

class Supreme808AudioProcessor : public juce::AudioProcessor
{
public:
    Supreme808AudioProcessor();
    ~Supreme808AudioProcessor() override = default;
    void prepareToPlay(double,int) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout&) const override;
    void processBlock(juce::AudioBuffer<float>&,juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }
    int getNumPrograms() override { return 1; } int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {} const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int,const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*,int) override;
    juce::AudioProcessorValueTreeState parameters;
private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    Kick808 kick;
    Drum808 drums;
    void triggerNote(int,float);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Supreme808AudioProcessor)
};
