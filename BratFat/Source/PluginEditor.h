/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BratFatAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    BratFatAudioProcessorEditor (BratFatAudioProcessor&);
    ~BratFatAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    juce::Slider sliderVolume;
    juce::Slider sliderFatness;
    juce::Slider sliderFilter;
    juce::Slider sliderAttack;
    juce::Slider sliderRelease;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BratFatAudioProcessor& audioProcessor;
    void sliderValueChanged(juce::Slider* slider) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BratFatAudioProcessorEditor)
};
