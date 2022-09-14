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
    void process();
    void loadBuffer(juce::AudioBuffer<float>* input);
    void loadSynthVector(std::vector<BratFat*>* synths);
    void setSampleRate(double input);
    void setFrequency(double f);
    void setFatness(double input);
    void setRelease(float input);
    void setAttack(float input);
    double getFrequency();
    void die();
    bool isDead();
    void setGain(double input);
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
    double fatness;
    double gain;
    bool freqLock = false;
    double sampleRate;
    juce::AudioBuffer<float>* buffer;
    float* outL;
    float* outR;
    float attackEnvelope = 0;
    float attackEnvelopeAdd;
    float releaseEnvelope = 1;
    float releaseEnvelopeSub;
    bool isDying;
    bool dead = false;
    std::vector<BratFat*>* synths;
};
