/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "BratFat.h"

//==============================================================================
/**
*/



class BratFatAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BratFatAudioProcessor();
    ~BratFatAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    bool isAddingFromMidiInput = false;
    //void juce::MidiInputCallback::handleIncomingMidiMessage(juce::MidiInput, const juce::MidiMessage& message) override;
    double f = 0.f;
    std::vector<BratFat*> synths;
    BratFatAudioProcessor* getThis();
    int vectorCleanCounter = 0;
    void setGainForSynths(float input);
    void setAttackForSynths(float input);
    void setReleaseForSynths(float input);
    void setFatnessForSynths(float input);
    void updateParams(float gain, float attack, float release, float fatness, float filter);
    float g = 1;
    float attack = 0.001;
    float release = 0.001;
    float fatness = 0;
    float sr;
private:
    //==============================================================================
    float gain = 1;
    float filterValue = 440.f;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>filter;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BratFatAudioProcessor)
};
