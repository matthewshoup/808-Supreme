#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class Supreme808AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit Supreme808AudioProcessorEditor(Supreme808AudioProcessor&);
    ~Supreme808AudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    Supreme808AudioProcessor& processor;
    juce::Slider tune, decay, level;
    juce::Label tuneLabel, decayLabel, levelLabel;
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> tuneAttachment, decayAttachment, levelAttachment;

    void configure(juce::Slider&, juce::Label&, const juce::String&);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Supreme808AudioProcessorEditor)
};
