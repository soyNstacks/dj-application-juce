/*
  ==============================================================================

    DeckGUI.cpp
    Created: 4 Feb 2022 10:15:49pm
    Author:  taan0

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _djAudioPlayer, 
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse
                )  : djAudioPlayer ( _djAudioPlayer ),
                     waveformDisplay(formatManagerToUse, cacheToUse)
{
    /** settings for widgets are done here */
    setButtonsSliders(); 
    /** start timer */
    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    /** stop timer when destroyed */
    stopTimer();
    /** reset LookAndFeel when destroyed */
    setLookAndFeel(nullptr);

}

void DeckGUI::paint (juce::Graphics& g)
{
    /** set background colour */
    g.fillAll(juce::Colour::fromRGB(0, 0, 5));

    g.setColour (juce::Colour::fromRGB(137, 139, 127));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    float rowH = getHeight() / 3;
    auto reverbArea = juce::Rectangle<float>(getWidth() * 0.1, 5, getWidth() * 0.8, rowH * 0.5);
    g.drawRect(reverbArea);     // draw rectangle around reverb component 

    g.setColour(juce::Colour::fromRGB(246, 246, 245));
    g.drawSingleLineText(highPassFreqSlider.getName(), highPassFreqSlider.getX() + 1.5f, highPassFreqSlider.getHeight() + highPassFreqSlider.getY() + 20.0f);
    g.drawSingleLineText(lowPassFreqSlider.getName(), lowPassFreqSlider.getX() + 1.0f, lowPassFreqSlider.getHeight() + lowPassFreqSlider.getY() + 20.0f);
    
}

void DeckGUI::resized()
{
    float rowH = getHeight() / 3;

    // set bounds for reverb sliders
    float reverbX = getWidth() * 0.11;
    float reverbY = 5;
    float reverbWidth = (getWidth() * 0.8) / 6.5;
    float reverbH = rowH * 0.5;
    float reverbSpace = reverbWidth / 3;
    roomSizeSlider.setBounds(reverbX, reverbY, reverbWidth, reverbH);
    dampSlider.setBounds(reverbX + reverbWidth + reverbSpace, reverbY, reverbWidth, reverbH);
    drySlider.setBounds(reverbX + (2 * (reverbWidth + reverbSpace)), reverbY, reverbWidth, reverbH);
    wetSlider.setBounds(reverbX + (3 * (reverbWidth + reverbSpace)), reverbY, reverbWidth, reverbH);
    widthSlider.setBounds(reverbX + (4 * (reverbWidth + reverbSpace)), reverbY, reverbWidth, reverbH);

    // set bounds for position slider 
    positionSlider.setBounds(getWidth() * 0.2, (reverbH + reverbY) + 10, getWidth() * 0.6, rowH * 1.5);

    // set bounds for playback buttons
    float btnSection = getWidth() * 0.04;
    float btnX = getWidth() / 2;
    float btnY = (reverbH + reverbY) + rowH * 1.5 + 20;
    float btnSpace = 10;
    loadButton.setBounds(btnX - btnSection - (btnSpace * 2) - 60, btnY, 60, btnSection);
    playButton.setBounds(btnX - btnSection - (btnSpace/2), btnY, btnSection, btnSection);
    pauseButton.setBounds(btnX + (btnSpace / 2), btnY, btnSection, btnSection);
    stopButton.setBounds(btnX + btnSection + btnSpace + btnSpace, btnY, btnSection, btnSection);
    loopButton.setBounds(btnX + ((btnSection + btnSpace) * 2) + btnSpace, btnY, btnSection, btnSection);

    // set bounds for volume slider, mute button and speed slider 
    float secondSection = reverbY + reverbH + 30;
    volumeSlider.setBounds(getWidth() * 0.05, secondSection, getWidth() / 8, rowH * 1.1);
    muteButton.setBounds(volumeSlider.getX() + (volumeSlider.getWidth() / 4), volumeSlider.getY() + volumeSlider.getHeight() + 5, volumeSlider.getWidth() / 2, volumeSlider.getHeight() * 0.25);
    speedSlider.setBounds(getWidth() * 0.1, muteButton.getY() + muteButton.getHeight(), getWidth() * 0.1, rowH / 2.5);
      
    // set bounds for IIR filter freqency sliders 
    highPassFreqSlider.setBounds(getWidth() * 0.8, secondSection - 5, getWidth() * 0.05, rowH * 1.7);
    lowPassFreqSlider.setBounds(getWidth() * 0.9, secondSection - 5, getWidth() * 0.05, rowH * 1.7);

    // set bounds for waveform display 
    float wfHeight = rowH / 2;
    waveformDisplay.setBounds(0, getHeight() - wfHeight, getWidth(), wfHeight);
 
}

/** button listener */
void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button was clicked");
        djAudioPlayer->play();
        pauseButton.setEnabled(true); 
        stopButton.setEnabled(true); 
    }
    if (button == &pauseButton)
    {
        DBG("pause button was clicked");
        djAudioPlayer->pause();
    }
    if (button == &stopButton)
    {
        DBG("Stop button was clicked");
        djAudioPlayer->stop();
    }
    if (button == &loopButton)
    {
        DBG("Loop button was clicked");
        auto state = button->getToggleState();
        juce::String stateString = state ? "ON" : "OFF";
        juce::Logger::outputDebugString(button->getName() + " changed to " + stateString);
        if (state == true && djAudioPlayer->isPlaying())
        {
            djAudioPlayer->isLooping(true);
        }
        else if (state == false && djAudioPlayer->isPlaying() == false)
        {
            djAudioPlayer->isLooping(false);
        }
    }
    if (button == &loadButton)
    {
        FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {
            loadToDeck(juce::URL{ chooser.getResult() });
        }
    }
    if (button == &muteButton)
    {
        auto state = button->getToggleState();
        juce::String stateString = state ? "ON" : "OFF";
        juce::Logger::outputDebugString(button->getName() + " changed to " + stateString);
        if (state)
        {
            DBG("Mute button was clicked");
            djAudioPlayer->setGain(0.0);
            volumeSlider.setEnabled(false);
        }
        else {
            djAudioPlayer->setGain(volumeSlider.getValue());
            volumeSlider.setEnabled(true);
        }
    }
}

/** slider listener */
void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volumeSlider)
    {
        DBG("MainComponent::sliderValueChanged: gainSlider " << slider->getValue());
        djAudioPlayer->setGain(slider->getValue());

    }    
    if (slider == &speedSlider)
    {
        DBG("MainComponent::sliderValueChanged: speedSlider " << slider->getValue());
        djAudioPlayer->setSpeed(slider->getValue());
    }
    if (slider == &positionSlider)
    {
        DBG("MainComponent::sliderValueChanged: posSlider " << slider->getValue());
        djAudioPlayer->setPositionRelative(slider->getValue());
        
    }
    if (slider == &roomSizeSlider)
    {
        DBG("MainComponent::sliderValueChanged: roomSizeSlider " << slider->getValue());
        
        djAudioPlayer->roomSize(slider->getValue());
    }
    if (slider == &dampSlider)
    {
        DBG("MainComponent::sliderValueChanged: dampSlider " << slider->getValue());
        djAudioPlayer->dampingLevel(slider->getValue());
    }
    if (slider == &wetSlider)
    {
        DBG("MainComponent::sliderValueChanged: wetSlider " << slider->getValue());
        djAudioPlayer->wetLevel(slider->getValue());
    }
    if (slider == &drySlider)
    {
        DBG("MainComponent::sliderValueChanged: drySlider " << slider->getValue());
        djAudioPlayer->dryLevel(slider->getValue());
    }
    if (slider == &widthSlider)
    {
        DBG("MainComponent::sliderValueChanged: widthSlider " << slider->getValue());
        djAudioPlayer->reverbWidth(slider->getValue());
    }
    if (slider == &highPassFreqSlider)
    {
        DBG("MainComponent::sliderValueChanged: highPassFreqSlider " << slider->getValue());
        djAudioPlayer->setIIRCoffecients("low", slider->getValue());
    }
    if (slider == &lowPassFreqSlider) 
    {
        DBG("MainComponent::sliderValueChanged: highPassFreqSlider " << slider->getValue());
        djAudioPlayer->setIIRCoffecients("low", slider->getValue());

    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files) 
{
    DBG("DeckGUI::isInterestedInFileDrag ");
    return true; 
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    for (String filename : files)
    {
        DBG("DeckGUI::filesDropped " << filename);
        URL fileURL = URL{ File{filename} };
        djAudioPlayer->loadURL(fileURL);
        return;
    }
}

void DeckGUI::timerCallback()
{
    //DBG("DeckGUI::timerCallback");
    waveformDisplay.setPositionRelative(
        djAudioPlayer->getPositionRelative());
    
    if (!positionSlider.isMouseButtonDown())
        positionSlider.setValue(djAudioPlayer->getPositionRelative(), juce::NotificationType::dontSendNotification);
    else positionSlider.addListener(this);
}

void DeckGUI::loadToDeck(juce::URL audioURL)
{
    djAudioPlayer->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}

/** widget settings */
void DeckGUI::setButtonsSliders()
{
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(loopButton);

    addAndMakeVisible(lowPassFreqSlider);
    addAndMakeVisible(highPassFreqSlider);

    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);
    volumeLabel.setText("Gain", juce::dontSendNotification);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);

    addAndMakeVisible(waveformDisplay);

    loadButton.addListener(this);
    playButton.addListener(this);
    stopButton.addListener(this);
    pauseButton.addListener(this);
    loopButton.addListener(this);
    muteButton.addListener(this);

    volumeSlider.addListener(this);
    speedSlider.addListener(this);

    highPassFreqSlider.addListener(this);
    lowPassFreqSlider.addListener(this);

    volumeSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.00001, 100.0);
    positionSlider.setRange(0.0, 1.0);
    highPassFreqSlider.setRange(1000, 20000);
    lowPassFreqSlider.setRange(20, 10000);

    positionSlider.setLookAndFeel(&posSliderLookAndFeel);
    positionSlider.setSliderStyle(juce::Slider::Rotary);
    positionSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

    speedSlider.setLookAndFeel(&speedLookAndFeel);
    speedSlider.setSliderStyle(Slider::Rotary);
    speedSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    speedSlider.setPopupDisplayEnabled(true, true, this);

    volumeSlider.setSliderStyle(Slider::LinearBarVertical);
    volumeSlider.setLookAndFeel(&IIRLevelLookAndFeel);
    volumeSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    volumeSlider.setPopupDisplayEnabled(true, true, this);
    volumeSlider.setTextValueSuffix(" db");

    highPassFreqSlider.setSliderStyle(Slider::LinearVertical);
    highPassFreqSlider.setLookAndFeel(&IIRLevelLookAndFeel);
    highPassFreqSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    highPassFreqSlider.setPopupDisplayEnabled(true, true, this);
    highPassFreqSlider.setTextValueSuffix(" Hz");
    lowPassFreqSlider.setSliderStyle(Slider::LinearVertical);
    lowPassFreqSlider.setLookAndFeel(&IIRLevelLookAndFeel);
    lowPassFreqSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    lowPassFreqSlider.setPopupDisplayEnabled(true, true, this);
    lowPassFreqSlider.setTextValueSuffix(" Hz");

    highPassFreqSlider.setName("HIGH");
    lowPassFreqSlider.setName("LOW");

    Image pauseImage = ImageCache::getFromMemory(BinaryData::pause_png, BinaryData::pause_pngSize);
    pauseButton.setImages(true,
        true,
        true,
        pauseImage, 0.7f, Colours::white,
        pauseImage, 1.0f, Colours::white,
        pauseImage, 0.2f, Colours::white.withAlpha(0.8f),
        0.1f);
    
    Image stopImage = ImageCache::getFromMemory(BinaryData::stopButton_png, BinaryData::stopButton_pngSize);
    stopButton.setImages(true,
        true,
        true,
        stopImage, 0.7f, Colours::white,
        stopImage, 1.0f, Colours::white,
        stopImage, 0.2f, Colours::white.withAlpha(0.8f),
        0.1f);

    Image playImage = ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize);
    playButton.setImages(true,
        true,
        true,
        playImage, 0.7f, Colours::white,
        playImage, 1.0f, Colours::white,
        playImage, 0.2f, Colours::white.withAlpha(0.7f),
        0.5f);

    Image loopImage = ImageCache::getFromMemory(BinaryData::loop_png, BinaryData::loop_pngSize);
    loopButton.setImages(true,
        true,
        true,
        loopImage, 0.7f, Colours::white,
        loopImage, 1.0f, Colours::white,
        loopImage, 0.2f, Colours::white.withAlpha(0.7f),
        0.5f);
    loopButton.setClickingTogglesState(true);

    Image muteImage = ImageCache::getFromMemory(BinaryData::muteButton_png, BinaryData::muteButton_pngSize);
    muteButton.setImages(true,
        true,
        true,
        muteImage, 0.7f, Colours::white,
        muteImage, 1.0f, Colours::white,
        muteImage, 0.2f, Colours::white.withAlpha(0.7f),
        0.5f);
    muteButton.setClickingTogglesState(true);

    addAndMakeVisible(roomSizeSlider);
    addAndMakeVisible(dampSlider);
    addAndMakeVisible(wetSlider);
    addAndMakeVisible(drySlider);
    addAndMakeVisible(widthSlider);

    roomSizeSlider.setName("Room Size"); 
    dampSlider.setName("Damping"); 
    wetSlider.setName("Wet"); 
    drySlider.setName("Dry");
    widthSlider.setName("Width");

    roomSizeSlider.addListener(this);
    dampSlider.addListener(this);
    wetSlider.addListener(this);
    drySlider.addListener(this);
    widthSlider.addListener(this);

    roomSizeSlider.setSliderStyle(juce::Slider::Rotary);
    dampSlider.setSliderStyle(juce::Slider::Rotary);
    wetSlider.setSliderStyle(juce::Slider::Rotary);
    drySlider.setSliderStyle(juce::Slider::Rotary);
    widthSlider.setSliderStyle(juce::Slider::Rotary);

    roomSizeSlider.setLookAndFeel(&reverbSliderLookAndFeel);
    dampSlider.setLookAndFeel(&reverbSliderLookAndFeel);
    wetSlider.setLookAndFeel(&reverbSliderLookAndFeel);
    drySlider.setLookAndFeel(&reverbSliderLookAndFeel);
    widthSlider.setLookAndFeel(&reverbSliderLookAndFeel);

    roomSizeSlider.setRange(0.0f, 1.0f);
    dampSlider.setRange(0.0f, 1.0f);
    wetSlider.setRange(0.0f, 1.0f);
    drySlider.setRange(0.0f, 1.0f);
    widthSlider.setRange(0.0f, 1.0f);

    roomSizeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    dampSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    wetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    drySlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    widthSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
}