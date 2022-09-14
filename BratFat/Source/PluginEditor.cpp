/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BratFatAudioProcessorEditor::BratFatAudioProcessorEditor (BratFatAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    sliderVolume.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sliderVolume.setRange(0.0, 10.0, 0.01);
    sliderVolume.setTextValueSuffix(" Gain");
    sliderVolume.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    sliderVolume.addListener(this);
    addAndMakeVisible(&sliderVolume);

    sliderAttack.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sliderAttack.setRange(0.1, 100, 0.01);
    sliderAttack.setTextValueSuffix(" Attack");
    sliderAttack.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    sliderAttack.addListener(this);
    addAndMakeVisible(&sliderAttack);

    sliderRelease.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sliderRelease.setRange(1, 100, 0.01);
    sliderRelease.setTextValueSuffix(" Release");
    sliderRelease.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    sliderRelease.addListener(this);
    addAndMakeVisible(&sliderRelease);

    sliderFatness.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sliderFatness.setRange(0.0, 2.0, 0.01);
    sliderFatness.setTextValueSuffix(" Fatness");
    sliderFatness.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    sliderFatness.addListener(this);
    addAndMakeVisible(&sliderFatness);

    sliderFilter.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sliderFilter.setRange(0.0, 20000.0, 1);
    sliderFilter.setTextValueSuffix(" Filter");
    sliderFilter.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    sliderFilter.addListener(this);
    addAndMakeVisible(&sliderFilter);
    setSize (630, 300);
}

BratFatAudioProcessorEditor::~BratFatAudioProcessorEditor()
{
}

//==============================================================================
void BratFatAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::red);
    g.setFont (15.0f);
    g.drawFittedText ("Made by Gouod Audio", getLocalBounds(), juce::Justification::bottomLeft, 1);
}
void BratFatAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.updateParams(sliderVolume.getValue(), sliderAttack.getValue(), sliderRelease.getValue(), sliderFatness.getValue(), sliderFilter.getValue());
}
void BratFatAudioProcessorEditor::resized()
{
    sliderFatness.setBounds(5, 5, 150, 150);
    sliderFilter.setBounds(160, 5, 150, 150);
    sliderAttack.setBounds(315, 5, 100, 100);
    sliderRelease.setBounds(420, 5, 100, 100);
    sliderVolume.setBounds(525, 5, 100, 100);
}
