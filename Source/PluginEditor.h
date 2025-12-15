#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ChorusCE2AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    ChorusCE2AudioProcessorEditor (ChorusCE2AudioProcessor&);
    ~ChorusCE2AudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ChorusCE2AudioProcessor& processor;

    juce::Slider rateSlider;
    juce::Slider depthSlider;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> rateAttach;
    std::unique_ptr<Attachment> depthAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusCE2AudioProcessorEditor)
};
