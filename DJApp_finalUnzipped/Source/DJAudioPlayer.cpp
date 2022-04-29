/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 4 Feb 2022 5:59:15pm
    Author:  taan0

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
                            : formatManager(_formatManager),
                              sample_rate(0)
{

}

DJAudioPlayer::~DJAudioPlayer()
{
   
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    IIRFilterSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) 
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    IIRFilterSource.getNextAudioBlock(bufferToFill);
    
}

void DJAudioPlayer::releaseResources() 
{
    transportSource.releaseResources(); 
    resampleSource.releaseResources(); 
    reverbSource.releaseResources(); 
    IIRFilterSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader,
            true));
        sample_rate = reader->sampleRate;
        transportSource.setSource(newSource.get(), 0, nullptr, sample_rate);
        readerSource.reset(newSource.release());
        DBG("Loaded");

    }
}

void DJAudioPlayer::setGain(double gain)
{
    transportSource.setGain(gain);
    
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 100.0)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 100");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }

}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos > 1.0)
    {
        double ratio = pos / getWidth();
        setPosition(transportSource.getLengthInSeconds() * ratio);
        DBG("mouseX ratio-ed value: " << ratio);
    }
    else if (pos < 0)
    {
        DBG("position of time must be more than 0");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::play()
{
    DBG("DJAudioPlayer::play()");
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    DBG("DJAudioPlayer::stop()");
    transportSource.stop();
    setPositionRelative(0);
}

void DJAudioPlayer::pause()
{
    DBG("DJAudioPlayer::pause()");
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    double pos = transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    if (isnan(pos) || (transportSource.getCurrentPosition() == 0)) {
        return 0;
    }
    return pos;
}

double DJAudioPlayer::getAudioLength()
{
    return transportSource.getLengthInSeconds();
}

void DJAudioPlayer::isLooping(bool state)
{
    if (state == true)
    {
        readerSource->setLooping(true);
        DBG("DJAudioPlayer::isLooping(bool state) : state is true " );
    }
    else {
        readerSource->setLooping(false);
        DBG("DJAudioPlayer::isLooping(bool state) : state is false ");
    }
}

void DJAudioPlayer::roomSize(float val)
{
    reverbParams.roomSize = val;
    reverbSource.setParameters(reverbParams);
}

void DJAudioPlayer::dampingLevel(float val)
{
    reverbParams.damping = val;
    reverbSource.setParameters(reverbParams);
}

void DJAudioPlayer::wetLevel(float val)
{
    reverbParams.wetLevel = val;
    reverbSource.setParameters(reverbParams);
}

void DJAudioPlayer::dryLevel(float val)
{
    reverbParams.dryLevel = val;
    reverbSource.setParameters(reverbParams);
}

void DJAudioPlayer::reverbWidth(float val)
{
    reverbParams.width = val;
    reverbSource.setParameters(reverbParams);
}

void DJAudioPlayer::setIIRCoffecients(juce::String mode, int freq)
{
    if (sample_rate > 0 || isPlaying())
    {
        if (mode == "low")  IIRFilterSource.setCoefficients(IIRCoefficients::makeLowPass(sample_rate, freq));
        if (mode == "high") IIRFilterSource.setCoefficients(IIRCoefficients::makeHighPass(sample_rate, freq));
    }
}

bool DJAudioPlayer::isPlaying()
{
    return transportSource.isPlaying(); 
}

