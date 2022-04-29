/*
  ==============================================================================

    DeckGUI.h
    Created: 4 Feb 2022 10:15:49pm
    Author:  taan0

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "SliderLookAndFeel.h"

//==============================================================================
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* djAudioPlayer,
            AudioFormatManager& formatManagerToUse, 
            AudioThumbnailCache& cacheToUse
            );
    ~DeckGUI();

    //==============================================================================

    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================

    /** implement Button::Listener */
    void buttonClicked(Button* button) override;
    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    /** implement functions to allow dragging and dropping of files into deckGUI component*/
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    void timerCallback() override; 

    //==============================================================================

    /** load audio file to deck*/
    void loadToDeck(juce::URL audioURL);

private:

    /** customised lookAndFeel of sliders */
    SpeedRotarySlider speedLookAndFeel;
    PositionRotarySlider posSliderLookAndFeel;
    ReverbSlider reverbSliderLookAndFeel;
    IIRFilterLevel IIRLevelLookAndFeel;

    //==============================================================================

    /** implement button for loading of audio */
    juce::TextButton loadButton{ "LOAD" };

    /** implement buttons for controls like stop, play, loop and mute */
    juce::ImageButton stopButton;
    juce::ImageButton playButton;
    juce::ImageButton pauseButton;
    juce::ImageButton loopButton;
    juce::ImageButton muteButton;

    /**  declaration of sliders for controls like volume, speed, position */
    juce::Slider volumeSlider, 
                speedSlider,
                positionSlider;

    /** declaration of labels to attach to sliders */
    juce::Label volumeLabel,
                speedLabel;
    
    /** declaration of sliders for IIR Filter controls */
    juce::Slider highPassFreqSlider, 
                 lowPassFreqSlider;

    /** declaration of sliders for Reverb Filter controls */
    juce::Slider roomSizeSlider,
                 dampSlider,
                 wetSlider,
                 drySlider,
                 widthSlider;

    /** declaration of labels to attach to sliders */
    juce::Label roomSizeLabel,
                dampLabel,
                wetLabel,
                dryLabel,
                widthLabel;

    //==============================================================================

    /** implement function to contain base settings of buttons and sliders */
    void setButtonsSliders();

    DJAudioPlayer* djAudioPlayer;
    WaveformDisplay waveformDisplay;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
