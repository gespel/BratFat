/*
  ==============================================================================

    BratFat.h
    Created: 1 Apr 2022 8:54:12pm
    Author:  stenh

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class BratFat {
public:
    BratFat();
    void process();
    void loadBuffer(juce::AudioBuffer<float>* input, juce::MidiBuffer* midi);
    void setSampleRate(double input);

private:
    double sineSynth(double phase, uint8_t synthNr);
    double squareSynth(double phase, uint8_t synthNr);
    void updatePhase();
    void processMidi();
    double phase1 = 0.f;
    double phase2 = 0.f;
    double phase3 = 0.f;
    double envelope = 1.f;
    double frequency = 0.f;
    double sampleRate;
    juce::AudioBuffer<float>* buffer;
    juce::MidiBuffer* midiBuffer;
    float* outL;
    float* outR;
};
