/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 4 Feb 2022 5:59:15pm
    Author:  taan0

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class DJAudioPlayer : public AudioSource, 
                      public juce::AudioAppComponent
{
public:

    DJAudioPlayer(AudioFormatManager& formatManager);
    ~DJAudioPlayer();

    //==============================================================================

    /** prepare audio source for playing. */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    /** called repeatedly to fetch subsequent blocks of audio data. */
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    /**  implemented for source to release anything no longer needed after playback has stopped.*/
    void releaseResources() override;

    //==============================================================================

    /** load audio via URL */
    void loadURL(URL audioURL);
    /** implement function to adjust volume of audio */
    void setGain(double gain); 
    /** implement function to adjust speed of audio */
    void setSpeed(double ratio);
    /** implement function to adjust position of audio */
    void setPosition(double posInSecs);
    /** implement function for conversion of slider value to audio length-adjustable value */
    void setPositionRelative(double pos);
    /** start playing the file */
    void play();
    /** stop playing the file and return to position 0*/
    void stop(); 
    /** pause the player */
    void pause(); 
    /** get relative position of the playhead */
    double getPositionRelative();
    /** get length or duration of audio */
    double getAudioLength();
    /** implement function to set audio loop */
    void isLooping(bool state);
    /** bool to check whether audio source is playing */
    bool isPlaying();

    //==============================================================================

    /** implement function to set room size parameter for Reverb Filter */
    void roomSize(float val);
    /** implement function to set damping parameter for Reverb Filter */
    void dampingLevel(float val);
    /** implement function to set wet level parameter for Reverb Filter */
    void wetLevel(float val);
    /** implement function to set dry level parameter for Reverb Filter */
    void dryLevel(float val);
    /** implement function to width parameter for Reverb Filter */
    void reverbWidth(float val);
   
    //==============================================================================

    /** implement function to set IIR Filter coefficients */
    void setIIRCoffecients(juce::String mode, int freq);
    float sample_rate; 

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;

    AudioTransportSource transportSource;
    /** Creates ResamplingAudioSource on transportSource */
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

    /** Creates ReverbAudioSource on resampleSource */
    juce::ReverbAudioSource reverbSource{ &resampleSource, false };
    /** Creates IIRFilterAudioSource on reverbSource */
    juce::IIRFilterAudioSource IIRFilterSource{ &reverbSource, false };
    
    /** to hold parameters being used by a Reverb object */
    juce::Reverb::Parameters reverbParams;


    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DJAudioPlayer)
};