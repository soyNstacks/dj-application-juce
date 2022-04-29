/*
  ==============================================================================

    WaveformDisplay.h
    Created: 5 Feb 2022 2:37:24pm
    Author:  taan0

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <JuceHeader.h>

//==============================================================================

class WaveformDisplay  : public juce::Component,
                         public juce::ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay();

    //==============================================================================

    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================

    void changeListenerCallback(ChangeBroadcaster* source) override;

    //==============================================================================
    /** implement with URL the loading of track into waveform display */
    void loadURL(juce::URL audioURL);
    /** set relative position of the playhead */
    void setPositionRelative(double pos); 

private:

    AudioThumbnail audioThumbnail;

    /** boolean to check whether file has been successfully loaded onto deck */
    bool fileLoaded; 
    /** declaration for position */
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
