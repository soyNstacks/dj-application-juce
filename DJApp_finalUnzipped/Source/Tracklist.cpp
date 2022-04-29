/*
  ==============================================================================

    Tracklist.cpp
    Created: 10 Feb 2022 6:54:20pm
    Author:  taan0

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Tracklist.h"
#include <filesystem>

//==============================================================================
Tracklist::Tracklist(juce::File _tracksFile) : tracksFile(_tracksFile),
                                              URL(juce::URL{ _tracksFile }),
                                              trackTitle(_tracksFile.getFileNameWithoutExtension())
{ }

