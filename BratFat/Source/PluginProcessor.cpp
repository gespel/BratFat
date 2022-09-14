/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BratFatAudioProcessor::BratFatAudioProcessor() : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )

{
}

BratFatAudioProcessor::~BratFatAudioProcessor()
{
}


//==============================================================================
const juce::String BratFatAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BratFatAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BratFatAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}
BratFatAudioProcessor* BratFatAudioProcessor::getThis() {
    return this;
}
bool BratFatAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BratFatAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BratFatAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BratFatAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BratFatAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BratFatAudioProcessor::getProgramName (int index)
{
    return {};
}

void BratFatAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}



//==============================================================================
void BratFatAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, filterValue);
    this->sr = sampleRate;
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    filter.prepare(spec);
    filter.reset();
}

void BratFatAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BratFatAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BratFatAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    for (const auto metadata : (midiMessages))
    {
        auto message = metadata.getMessage();
        const auto time = metadata.samplePosition;

        if (message.isNoteOn())
        {
            BratFat* x = new BratFat();
            x->setFrequency(juce::MidiMessage::getMidiNoteInHertz(message.getNoteNumber()));
            x->setSampleRate(getSampleRate());
            x->setGain(this->g);
            x->setAttack(this->attack);
            x->setFatness(this->fatness);
            x->setRelease(this->release);
            synths.push_back(x);
        }
        else if (message.isNoteOff())
        {
            for (int i = 0; i < synths.size(); i++) {
                if (synths[i]->getFrequency() == juce::MidiMessage::getMidiNoteInHertz(message.getNoteNumber())) {
                    BratFat* x = synths[i];
                    x->die();
                }
            }

        }
    }
    for (int i = 0; i < synths.size(); i++) {
        synths[i]->loadBuffer(&buffer);
        synths[i]->process();
    }
    if (vectorCleanCounter == 10000) {
        for (int i = 0; i < synths.size(); i++) {
            if (synths[i]->isDead()) {
                BratFat* a = synths[i];
                synths.erase(synths.begin() + i);
                delete a;
            }
        }
        vectorCleanCounter = 0;
    }
    vectorCleanCounter++;

    juce::dsp::AudioBlock<float> block(buffer);
    filter.process(juce::dsp::ProcessContextReplacing<float>(block));

}
//==============================================================================
bool BratFatAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}
void BratFatAudioProcessor::updateParams(float gain, float attack, float release, float fatness, float filter) {
    for (int i = 0; i < synths.size(); i++) {
        synths[i]->setGain(gain);
        synths[i]->setAttack(attack);
        synths[i]->setRelease(release);
        synths[i]->setFatness(fatness);
    }
    this->g = gain;
    this->attack = attack;
    this->release = release;
    this->fatness = fatness;
    this->filterValue = filter;
    *this->filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(this->sr, filterValue);
}
void BratFatAudioProcessor::setGainForSynths(float input) {
    for (int i = 0; i < synths.size(); i++) {
        synths[i]->setGain(input);
    }
}
void BratFatAudioProcessor::setAttackForSynths(float input) {

}
void BratFatAudioProcessor::setReleaseForSynths(float input) {

}
void BratFatAudioProcessor::setFatnessForSynths(float input) {

}
juce::AudioProcessorEditor* BratFatAudioProcessor::createEditor()
{
    return new BratFatAudioProcessorEditor (*this);
}

//==============================================================================
void BratFatAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BratFatAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

/*void BratFatAudioProcessor::handleIncomingMidiMessage(juce::MidiInput, const juce::MidiMessage& message)
{
}*/

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BratFatAudioProcessor();
}
