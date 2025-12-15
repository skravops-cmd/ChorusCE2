#include "PluginEditor.h"

//==============================================================================
ChorusCE2AudioProcessorEditor::ChorusCE2AudioProcessorEditor
    (ChorusCE2AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    auto setup = [this] (juce::Slider& s)
    {
        s.setSliderStyle (juce::Slider::Rotary);
        s.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible (s);
    };

    setup (rateSlider);
    setup (depthSlider);

    rateAttach  = std::make_unique<Attachment> (processor.apvts, "rate", rateSlider);
    depthAttach = std::make_unique<Attachment> (processor.apvts, "depth", depthSlider);

    setSize (200, 300);
}

//==============================================================================
void ChorusCE2AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff3fa9c5)); // CE-2 blue
    g.setColour (juce::Colours::black);
    g.setFont (18.0f);
    g.drawText ("CHORUS CE-2", getLocalBounds().removeFromTop (40),
                juce::Justification::centred);
}

void ChorusCE2AudioProcessorEditor::resized()
{
    rateSlider .setBounds (30, 60, 60, 60);
    depthSlider.setBounds (110, 60, 60, 60);
}
