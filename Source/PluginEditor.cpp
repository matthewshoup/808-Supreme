#include "PluginEditor.h"

Supreme808AudioProcessorEditor::Supreme808AudioProcessorEditor(Supreme808AudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    configure(tune, tuneLabel, "TUNE");
    configure(decay, decayLabel, "DECAY");
    configure(level, levelLabel, "LEVEL");

    tuneAttachment = std::make_unique<Attachment>(processor.parameters, "tune", tune);
    decayAttachment = std::make_unique<Attachment>(processor.parameters, "decay", decay);
    levelAttachment = std::make_unique<Attachment>(processor.parameters, "level", level);

    setSize(520, 280);
}

void Supreme808AudioProcessorEditor::configure(juce::Slider& slider, juce::Label& label, const juce::String& text)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 22);
    addAndMakeVisible(slider);

    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}

void Supreme808AudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff171717));
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(28.0f, juce::Font::bold));
    g.drawFittedText("808 SUPREME", 20, 18, getWidth() - 40, 36, juce::Justification::centred, 1);
    g.setFont(juce::FontOptions(14.0f));
    g.drawFittedText("LET THE BASS REIGN SUPREME", 20, 55, getWidth() - 40, 24, juce::Justification::centred, 1);
}

void Supreme808AudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(30);
    area.removeFromTop(70);
    const int width = area.getWidth() / 3;

    auto place = [&](juce::Slider& slider, juce::Label& label)
    {
        auto cell = area.removeFromLeft(width);
        label.setBounds(cell.removeFromTop(24));
        slider.setBounds(cell.reduced(8));
    };

    place(tune, tuneLabel);
    place(decay, decayLabel);
    place(level, levelLabel);
}
