/*
  ==============================================================================

    BratFat.cpp
    Created: 1 Apr 2022 8:54:12pm
    Author:  stenh

  ==============================================================================
*/

#include "BratFat.h"

BratFat::BratFat() 
{

}
void BratFat::loadBuffer(juce::AudioBuffer<float>* input, juce::MidiBuffer* midi) {
    outL = input->getWritePointer(0);
    outR = input->getWritePointer(1);
    buffer = input;
    midiBuffer = midi;
}
void BratFat::setSampleRate(double input) {
    sampleRate = input;
}
void BratFat::processMidi() {
    for (const auto metadata : (*midiBuffer))
    {
        auto message = metadata.getMessage();
        const auto time = metadata.samplePosition;

        if (message.isNoteOn())
        {
            envelope = 1;
            frequency = juce::MidiMessage::getMidiNoteInHertz(message.getNoteNumber());
        }
        if (message.isNoteOff())
        {
            if (envelope < 0.01) {
                envelope = 0;
            }
            else
            {
                envelope -= 0.001;
            }
        }

    }
}
void BratFat::updatePhase() {
    phase1 += ((frequency) / sampleRate) * 2.0f * 3.14159265;
    phase2 += ((frequency*1.0001) / sampleRate) * 2.0f * 3.14159265;
    phase3 += ((frequency*0.999) / sampleRate) * 2.0f * 3.14159265;
}
void BratFat::process() {
    for (int sample = 0; sample < buffer->getNumSamples(); sample++) {
        processMidi();
        auto sineSample = sineSynth(phase1, 0);
        auto squareSample1 = squareSynth(phase2, 1);
        auto squareSample2 = squareSynth(phase3, 2);
        updatePhase();

        auto finalSampleL = sineSample + squareSample1 + 0.2*squareSample2;
        auto finalSampleR = sineSample + squareSample2+ 0.2*squareSample1;

        finalSampleL *= 0.3;
        finalSampleR *= 0.3;

        outL[sample] = finalSampleL * envelope;
        outR[sample] = finalSampleR * envelope;
      
        
    }

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

