/*
  ==============================================================================

    BratFat.h
    Created: 1 Apr 2022 8:54:12pm
    Author:  stenh

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "PluginProcessor.h"

class BratFat {
public:
    BratFat(BratFatAudioProcessor* p);
    void process();
    void loadBuffer(juce::AudioBuffer<float>* input);
    void setSampleRate(double input);
    void setFrequency(double f);
    double getFrequency();
    void die();

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
    double prevFrequency = 0.f;
    bool freqLock = false;
    double sampleRate;
    juce::AudioBuffer<float>* buffer;
    float* outL;
    float* outR;
    double attackEnvelope = 0;
    double attackEnvelopeAdd = 0.00005f;
    double releaseEnvelope = 1;
    double releaseEnvelopeSub = 0.00005f;
    bool isDying;
    BratFatAudioProcessor* processor;
};
