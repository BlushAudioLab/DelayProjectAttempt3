/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DelayProjectAttemptAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DelayProjectAttemptAudioProcessorEditor (DelayProjectAttemptAudioProcessor&);
    ~DelayProjectAttemptAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayProjectAttemptAudioProcessor& processor;

    Slider mDryWetSlider;
    Slider mFeedbackSlider;
    Slider mDelayTimeSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayProjectAttemptAudioProcessorEditor)
};
