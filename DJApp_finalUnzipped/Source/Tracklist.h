/*
  ==============================================================================

    Tracklist.h
    Created: 10 Feb 2022 6:54:20pm
    Author:  taan0

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Tracklist  
{
public:
    Tracklist(juce::File _tracksFile);

    /** declaration of variables */
    juce::File tracksFile;
    juce::URL URL;
    juce::String trackTitle;
    juce::String trackDuration;
    juce::String trackId;

};
