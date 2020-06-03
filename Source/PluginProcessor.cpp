/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayProjectAttempt3AudioProcessor::DelayProjectAttempt3AudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)

                       .withOutput ("Output", AudioChannelSet::stereo(), true))
{
    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    mCircularBufferWriteHead = 0.;
    mCircularBufferLength = 0.;
    mDelayTimeInSamples = 0.;
    mDelayReadHead = 0.;

}

DelayProjectAttempt3AudioProcessor::~DelayProjectAttempt3AudioProcessor()
{
    if (mCircularBufferLeft != nullptr){
        delete [] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }
    
    if (mCircularBufferRight == nullptr){
        delete [] mCircularBufferRight;
        mCircularBufferRight = nullptr;
        
    }
    
}

//==============================================================================
const String DelayProjectAttempt3AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayProjectAttempt3AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayProjectAttempt3AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayProjectAttempt3AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayProjectAttempt3AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayProjectAttempt3AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayProjectAttempt3AudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayProjectAttempt3AudioProcessor::setCurrentProgram (int index)
{
}

const String DelayProjectAttempt3AudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayProjectAttempt3AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayProjectAttempt3AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    mDelayTimeInSamples = sampleRate * 0.5;
    
    mCircularBufferLength = sampleRate * MAX_DELAY_TIME;
    
    if (mCircularBufferLeft == nullptr){
        mCircularBufferLeft = new float[mCircularBufferLength];
    }
    
    if (mCircularBufferRight == nullptr){
        mCircularBufferRight = new float[mCircularBufferLength];
        
    }
    
    mCircularBufferWriteHead = 0;
}

void DelayProjectAttempt3AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayProjectAttempt3AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet() == AudioChannelSet::stereo() &&
        layouts.getMainOutputChannelSet() == AudioChannelSet::stereo())
{
        return true;
    }
        else {
                  return false;
                   }
}
#endif

void DelayProjectAttempt3AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        
        mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i];
        mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i];
        
        mDelayReadHead = mCircularBufferWriteHead - mDelayTimeInSamples;
        
        if (mDelayReadHead < 0) {
            mDelayReadHead += mCircularBufferLength;
        }
        
        buffer.addSample(0, i, mCircularBufferLeft[(int)mDelayReadHead]);
        buffer.addSample(1, i, mCircularBufferRight[(int)mDelayReadHead]);

        
        mCircularBufferWriteHead++;
        
        if (mCircularBufferWriteHead >= mCircularBufferLength) {
            mCircularBufferWriteHead = 0;
        }
    }
}

//==============================================================================
bool DelayProjectAttempt3AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayProjectAttempt3AudioProcessor::createEditor()
{
    return new DelayProjectAttempt3AudioProcessorEditor (*this);
}

//==============================================================================
void DelayProjectAttempt3AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayProjectAttempt3AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayProjectAttempt3AudioProcessor();
}
