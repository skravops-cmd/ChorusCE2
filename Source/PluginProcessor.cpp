#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusCE2AudioProcessor::ChorusCE2AudioProcessor()
: AudioProcessor (BusesProperties()
                  .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                  .withOutput ("Output", juce::AudioChannelSet::mono(), true)),
  apvts (*this, nullptr, "PARAMS", createParams())
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout
ChorusCE2AudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        "rate", "Rate", 0.1f, 5.0f, 1.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        "depth", "Depth", 0.0f, 1.0f, 0.5f));

    return { params.begin(), params.end() };
}

//==============================================================================
void ChorusCE2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sr = sampleRate;

    int maxDelaySamples = (int) (0.03 * sr); // 30 ms max
    delayBuffer.setSize (1, maxDelaySamples + samplesPerBlock);
    delayBuffer.clear();

    writePos = 0;
    lfoPhase = 0.0f;
}

//==============================================================================
void ChorusCE2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                            juce::MidiBuffer&)
{
    auto* rate  = apvts.getRawParameterValue ("rate");
    auto* depth = apvts.getRawParameterValue ("depth");

    float* channelData = buffer.getWritePointer (0);
    float* delayData   = delayBuffer.getWritePointer (0);

    int bufferSize = delayBuffer.getNumSamples();
    int numSamples = buffer.getNumSamples();

    for (int i = 0; i < numSamples; ++i)
    {
        // LFO
        float lfo = std::sin (lfoPhase);
        lfoPhase += juce::MathConstants<float>::twoPi * (*rate) / (float) sr;
        if (lfoPhase > juce::MathConstants<float>::twoPi)
            lfoPhase -= juce::MathConstants<float>::twoPi;

        // Delay modulation
        float delaySamples = (10.0f + lfo * (*depth) * 10.0f) * sr / 1000.0f;

        int readPos = (int) (writePos - delaySamples + bufferSize) % bufferSize;

        float dry = channelData[i];
        float wet = delayData[readPos];

        delayData[writePos] = dry;

        // Fixed mix (classic pedal style)
        channelData[i] = 0.7f * dry + 0.3f * wet;

        writePos = (writePos + 1) % bufferSize;
    }
}

//==============================================================================
juce::AudioProcessorEditor* ChorusCE2AudioProcessor::createEditor()
{
    return new ChorusCE2AudioProcessorEditor (*this);
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChorusCE2AudioProcessor();
}
