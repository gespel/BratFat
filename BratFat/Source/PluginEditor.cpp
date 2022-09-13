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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
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

void BratFatAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
