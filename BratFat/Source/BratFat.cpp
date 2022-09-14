/*
  ==============================================================================

    BratFat.cpp
    Created: 1 Apr 2022 8:54:12pm
    Author:  stenh

  ==============================================================================
*/

#include "BratFat.h"
#include "PluginProcessor.h"

void BratFat::loadBuffer(juce::AudioBuffer<float>* input) {
    outL = input->getWritePointer(0);
    outR = input->getWritePointer(1);
    buffer = input;
}
void BratFat::loadSynthVector(std::vector<BratFat*>* synths) {
    this->synths = synths;
}
void BratFat::setSampleRate(double input) {
    sampleRate = input;
}
void BratFat::processMidi() {
    
}
void BratFat::updatePhase() {
    phase1 += ((frequency) / sampleRate) * 2.0f * 3.14159265;
    phase2 += ((frequency*(1 + fatness / 100)) / sampleRate) * 2.0f * 3.14159265;
    phase3 += ((frequency* (1 - fatness / 100)) / sampleRate) * 2.0f * 3.14159265;
}
void BratFat::process() {
    for (int sample = 0; sample < buffer->getNumSamples(); sample++) {
        processMidi();
        auto sineSample = sineSynth(phase1, 0);
        auto squareSample1 = squareSynth(phase2, 1);
        auto squareSample2 = squareSynth(phase3, 2);
        updatePhase();

        auto finalSampleL = sineSample + squareSample1 + 0.2*squareSample2;
        auto finalSampleR = sineSample + squareSample2 + 0.2*squareSample1;

        finalSampleL *= 0.3;
        finalSampleR *= 0.3;

        if (attackEnvelope <= 1) {
            attackEnvelope += attackEnvelopeAdd;
        }
        if (attackEnvelope > 1 || attackEnvelope < 0) {
            attackEnvelope = 1;
        }
        if (this->isDying == true) {
            if (releaseEnvelope > 0) {
                releaseEnvelope -= releaseEnvelopeSub;
            }
            else {
                dead = true;
                releaseEnvelope = 0;
            }
        }
        outL[sample] += finalSampleL * attackEnvelope * releaseEnvelope;
        outR[sample] += finalSampleR * attackEnvelope * releaseEnvelope;
    }

}
void BratFat::die() {
    this->isDying = true;
}
void BratFat::setGain(double input) {
    this->gain = input;
}
void BratFat::setFatness(double input) {
    this->fatness = input;
}
void BratFat::setRelease(float input) {
    this->releaseEnvelopeSub = 1 / (input * 1000);
}
void BratFat::setAttack(float input) {
    this->attackEnvelopeAdd = 1 / (input * 1000);
}
double BratFat::sineSynth(double phase, uint8_t synthNr) {
    auto sample = (float)std::sin(phase);
    return sample;   
}
double BratFat::squareSynth(double phase, uint8_t synthNr) {
    float sample = (float)std::sin(phase);
    if (sample > 0) {
        sample = .5f;
    }
    else if (sample < 0) {
        sample = -.5f;
    }
    else {
        sample = 0;
    }
    return sample;
}
void BratFat::setFrequency(double f) {
    frequency = f;
}
double BratFat::getFrequency() {
    return frequency;
}
bool BratFat::isDead() {
    return dead;
}
