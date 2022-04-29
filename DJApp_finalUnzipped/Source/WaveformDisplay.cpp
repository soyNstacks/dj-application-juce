/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 5 Feb 2022 2:37:24pm
    Author:  taan0

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
                                AudioThumbnailCache& cacheToUse)
                                : 
                                audioThumbnail(1000, formatManagerToUse, cacheToUse),
                                fileLoaded(false),
                                position(0) // set to start at 0 in case there is a random number placed in memory 
{

    audioThumbnail.addChangeListener(this);

}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(24, 28, 36));
    g.setColour(juce::Colour::fromRGB(213, 212, 207));

    if (fileLoaded)
    {
        g.setColour(juce::Colour::fromRGB(1, 185, 189));
        audioThumbnail.drawChannel(g, getLocalBounds(), 0, audioThumbnail.getTotalLength(), 0, 1.0f);

        auto area = juce::Rectangle<int>(0, 0, position * getWidth(), getHeight());
        g.setColour(juce::Colour::fromRGBA(0, 0, 0, 80));
        g.fillRect(area);
        
        g.setColour(juce::Colour::fromRGB(248, 179, 22));
        g.drawRect(position * getWidth(), 0, getWidth() / 95, getHeight());

        float triX = ( ((position * getWidth()) + (getWidth() / 100)) + (position * getWidth()) )/ 2;
        float triY = getHeight();
        Path pointerHead;
        pointerHead.addTriangle(triX, 10, triX - 10, 0, triX + 10, 0);
        pointerHead.addTriangle(triX, triY - 10, triX - 10, triY, triX + 10, triY);
        g.fillPath(pointerHead);

    }
    else {
        g.setFont(20.0f);
        g.drawText("No Track Loaded...", getLocalBounds(),
            juce::Justification::centred, true);   
    }
    auto bounds = juce::Rectangle<float> (getLocalBounds().getX(), getLocalBounds().getY(), getLocalBounds().getWidth(), getLocalBounds().getHeight());
    g.drawRoundedRectangle(bounds, 5.0f, 5.0f);
}

void WaveformDisplay::resized()
{ }

void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new URLInputSource(audioURL));
    if (fileLoaded)
    {
        DBG("wfd: loaded!");
    }
    else {
        DBG("wfd: not loaded!");
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    DBG("wfd: change received!");
    repaint();

}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
    
}
