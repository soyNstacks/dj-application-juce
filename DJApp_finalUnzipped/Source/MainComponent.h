/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "MusicLibrary.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

    

private:

    /** button for visibility of playlist based on user's option */
    juce::TextButton showPlaylistButton;

    AudioFormatManager formatManager; 
    AudioThumbnailCache thumbCache{ 100 };

    /** first audio player */
    DJAudioPlayer player1{ formatManager };
    /** first deck for audio GUI controls*/
    DeckGUI deck1{ &player1, formatManager, thumbCache };

    /** second audio player*/
    DJAudioPlayer player2{ formatManager };
    /** second deck for audio GUI controls */
    DeckGUI deck2{ &player2, formatManager, thumbCache };

    /** audio sources to mix */
    MixerAudioSource mixerSource;

    //==============================================================================

    /** for additional playlist window */
    juce::Component::SafePointer<Component> window;
    /** implement function for a visible playlist window */
    void showPlaylist();
    /** implement function to hide or close all windows*/
    void closeAllWindows();
    /** implement function tp update state of playlist visibility */
    void updateToggleState(juce::Button* button);       

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
