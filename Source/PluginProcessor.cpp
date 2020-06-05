/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayProjectAttemptAudioProcessor::DelayProjectAttemptAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)

                       .withOutput ("Output", AudioChannelSet::stereo(), true))
{
    addParameter(mDryWetParameter = new AudioParameterFloat("drywet", "Dry Wet", 0.0, 1.0, 0.5));
    
    addParameter(mFeedbackParameter = new AudioParameterFloat("feedback", "Feedback Amount", 0.0, 0.98, 0.5));
    
    addParameter(mDelayTimeParameter = new AudioParameterFloat("delaytime", "Delay Time", 0.01, MAX_DELAY_TIME, 0.2));
    
    
    mDelayTimeSmoothed = 0;
    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    mCircularBufferWriteHead = 0.;
    mCircularBufferLength = 0.;
    mDelayTimeInSamples = 0.;
    mDelayReadHead = 0.;
    
    mFeedbackLeft = 0;
    mFeedbackRight = 0;
    
    mDelayTimeSmoothed = 0;
    
    

}

DelayProjectAttemptAudioProcessor::~DelayProjectAttemptAudioProcessor()
{
    if (mCircularBufferLeft != nullptr){
        delete [] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }
    
 
    if (mCircularBufferRight != nullptr){
        delete [] mCircularBufferRight;
        mCircularBufferRight = nullptr;
        
    }
}

//==============================================================================
const String DelayProjectAttemptAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayProjectAttemptAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayProjectAttemptAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayProjectAttemptAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayProjectAttemptAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayProjectAttemptAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayProjectAttemptAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayProjectAttemptAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayProjectAttemptAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayProjectAttemptAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayProjectAttemptAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    mDelayTimeInSamples = sampleRate * *mDelayTimeParameter;
    
    mCircularBufferLength = sampleRate * MAX_DELAY_TIME;
    
   
    
    if (mCircularBufferLeft == nullptr){
        mCircularBufferLeft = new float[mCircularBufferLength];
    }
        
    zeromem(mCircularBufferLeft, mCircularBufferLength * sizeof(float)); //setting all the bytes to zero in left channel
    
    if (mCircularBufferRight == nullptr){
        mCircularBufferRight = new float[mCircularBufferLength];
    }
    
     zeromem(mCircularBufferRight, mCircularBufferLength * sizeof(float)); //setting all the bytes to zero in right channel
    
    mCircularBufferWriteHead = 0;
    
    mDelayTimeSmoothed = *mDelayTimeParameter;
}

void DelayProjectAttemptAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayProjectAttemptAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DelayProjectAttemptAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
        
        
        
        mDelayTimeSmoothed = mDelayTimeSmoothed - 0.001 * (mDelayTimeSmoothed - *mDelayTimeParameter);
        
        mDelayTimeInSamples = getSampleRate() * *mDelayTimeParameter;
        
        mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
        mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedbackRight;
        
        mDelayReadHead = mCircularBufferWriteHead - mDelayTimeInSamples;
        
        if (mDelayReadHead < 0) {
            mDelayReadHead += mCircularBufferLength;
        }
        
        int readHead_x = (int)mDelayReadHead; //casting the read head value as an integer
        
        int readHead_x1 = readHead_x + 1; // getting the next index in the array for the readhead
        
        
        
        
        
        float readHeadFloat = mDelayReadHead - readHead_x; // we are now subtracting the integer version of the read head from the original floating point version
        
        /*now we make sure that the next sample is not at the end of the read head*/
        if (readHead_x1 >= mCircularBufferLength){
            readHead_x1 -= mCircularBufferLength;
        }
        
        
        float delay_sample_left = lin_interp(mCircularBufferLeft[readHead_x], mCircularBufferLeft[readHead_x1], readHeadFloat);
        float delay_sample_right = lin_interp(mCircularBufferRight[readHead_x], mCircularBufferRight[readHead_x1], readHeadFloat);
        
        
        mFeedbackLeft = delay_sample_left * *mFeedbackParameter;
        mFeedbackRight = delay_sample_right * *mFeedbackParameter;
        
        
        mCircularBufferWriteHead++;
        
        buffer.setSample(0, i, buffer.getSample(0, i) * (1 - *mDryWetParameter) + delay_sample_left * *mDryWetParameter);
        buffer.setSample(1, i, buffer.getSample(1, i) * (1 - *mDryWetParameter) + delay_sample_right * *mDryWetParameter);
        
        if (mCircularBufferWriteHead >= mCircularBufferLength) {
            mCircularBufferWriteHead = 0;
        }
    }
}

//==============================================================================
bool DelayProjectAttemptAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayProjectAttemptAudioProcessor::createEditor()
{
    return new DelayProjectAttemptAudioProcessorEditor (*this);
}

//==============================================================================
void DelayProjectAttemptAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayProjectAttemptAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayProjectAttemptAudioProcessor();
}

float DelayProjectAttemptAudioProcessor::lin_interp(float sample_x, float sample_x1, float inPhase)
{
  return (1 - inPhase) * sample_x + inPhase * sample_x1;
}
