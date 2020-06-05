/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayProjectAttemptAudioProcessorEditor::DelayProjectAttemptAudioProcessorEditor (DelayProjectAttemptAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    auto& params = processor.getParameters(); //get the list of parameters we can use
    
    /*This bit is is the Dry Wet Slider*/
    
    AudioParameterFloat* dryWetParameter = (AudioParameterFloat*)params.getUnchecked(0); //set to 0 because it is the 1st parameter - indexing from 0
    
    mDryWetSlider.setBounds(0, 0, 100, 100); //sets position and size
    mDryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag); //sets style
    mDryWetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0); //gets rid of the slider text box from the UI
    mDryWetSlider.setRange(dryWetParameter->range.start, dryWetParameter->range.end); //sets the range
    mDryWetSlider.setValue(*dryWetParameter); //sets the slider value to the current value of the dry wet parameter in the processor
    addAndMakeVisible(mDryWetSlider); //makes the slider visible on the screen

    mDryWetSlider.onValueChange = [this, dryWetParameter] { *dryWetParameter = mDryWetSlider.getValue(); }; //lambda function to update slider when the parameter value is changed
    mDryWetSlider.onDragStart = [dryWetParameter] { dryWetParameter->beginChangeGesture(); }; //lambda for what happens when you start dragging the slider
    mDryWetSlider.onDragEnd = [dryWetParameter] { dryWetParameter->endChangeGesture(); }; //lambda for what happens when you stop moving the slider
    
    /*This bit is the Feedback Slider*/
    
    AudioParameterFloat* feedbackParameter = (AudioParameterFloat*)params.getUnchecked(1); //set to 1 because it is the 2nd parameter - indexing from 0
    
    mFeedbackSlider.setBounds(100, 0, 100, 100); //sets the position and size
    mFeedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag); //sets style
    mFeedbackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0); //gets rid of the slider text box from the UI
    mFeedbackSlider.setRange(feedbackParameter->range.start, feedbackParameter->range.end); //sets the range
    mFeedbackSlider.setValue(*feedbackParameter); //sets the slider value to the current value of the dry wet parameter in the processor
    addAndMakeVisible(mFeedbackSlider); //makes the slider visible on the screen
    
    mFeedbackSlider.onValueChange = [this, feedbackParameter] { *feedbackParameter = mFeedbackSlider.getValue(); }; //lambda function to update slider when the parameter value is changed
    mFeedbackSlider.onDragStart = [feedbackParameter] { feedbackParameter->beginChangeGesture(); }; //lambda for what happens when you start dragging the slider
    mFeedbackSlider.onDragEnd = [feedbackParameter] { feedbackParameter->endChangeGesture(); }; //lambda for what happens when you stop moving the slider
    
    /*This bit is the Feedback Slider*/
    
    AudioParameterFloat* delayTimeParameter = (AudioParameterFloat*)params.getUnchecked(2); //set to 2 because it is the 3rd parameter - indexing from 0
    
    mDelayTimeSlider.setBounds(200, 0, 100, 100); //sets the position and size
    mDelayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag); //sets style
    mDelayTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0); //gets rid of the slider text box from the UI
    mDelayTimeSlider.setRange(delayTimeParameter->range.start, delayTimeParameter->range.end); //sets the range
    mDelayTimeSlider.setValue(*delayTimeParameter); //sets the slider value to the current value of the dry wet parameter in the processor
    addAndMakeVisible(mDelayTimeSlider); //makes the slider visible on the screen
    
    mDelayTimeSlider.onValueChange = [this, delayTimeParameter] { *delayTimeParameter = mDelayTimeSlider.getValue(); }; //lambda function to update slider when the parameter value is changed
    mDelayTimeSlider.onDragStart = [delayTimeParameter] { delayTimeParameter->beginChangeGesture(); }; //lambda for what happens when you start dragging the slider
    mDelayTimeSlider.onDragEnd = [delayTimeParameter] { delayTimeParameter->endChangeGesture(); }; //lambda for what happens when you stop moving the slider
    
    
    Slider mDelayTimeSlider;
}

DelayProjectAttemptAudioProcessorEditor::~DelayProjectAttemptAudioProcessorEditor()
{
}

//==============================================================================
void DelayProjectAttemptAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Wilson Guitars", getLocalBounds(), Justification::centred, 1);
}

void DelayProjectAttemptAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
