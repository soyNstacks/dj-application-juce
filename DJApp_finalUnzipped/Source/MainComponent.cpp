/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);

    formatManager.registerBasicFormats(); 

    addAndMakeVisible(showPlaylistButton);
    showPlaylistButton.setButtonText("Show Playlist");
    showPlaylistButton.setClickingTogglesState(true);
    showPlaylistButton.onClick = [this] { 
        updateToggleState(&showPlaylistButton);
    };

    getLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(149, 148, 144));

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    // reset lookAndFeel
    setLookAndFeel(nullptr);
    // close additional playlist window to prevent memory leak 
    closeAllWindows();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill); 
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
    mixerSource.removeAllInputs();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
}

void MainComponent::resized()
{
    float deckW = getWidth() / 2;
    float deckH = getHeight() - 50;

    deck1.setBounds(0, 0, deckW, deckH);
    deck2.setBounds(deckW, 0, deckW, deckH);

    showPlaylistButton.setBounds(5, getHeight() - 45, getWidth() - 10, 40);
}

void MainComponent::updateToggleState(juce::Button* button)
{
    if (button == &showPlaylistButton)
    {
        auto state = button->getToggleState();
        juce::String option = state ? "Hide Playlist" : "Show Playlist";

        if (state)
        {
            closeAllWindows();
            showPlaylist();
        }
        else {
            closeAllWindows();
        }

        button->setButtonText(option);
    }  
}

void MainComponent::showPlaylist()
{
    window = new MusicLibrary(&player1, &deck1, &deck2, formatManager);
    window->addToDesktop(ComponentPeer::windowIsTemporary);

    Rectangle<int> area(0, 0, 900, 250);

    RectanglePlacement placement(RectanglePlacement::xMid
        | RectanglePlacement::yBottom
        | RectanglePlacement::doNotResize);

    auto result = placement.appliedTo(area, juce::Desktop::getInstance().getDisplays()
        .getPrimaryDisplay()->totalArea);

    window->setBounds(result);
    window->setVisible(true);
    
}

void MainComponent::closeAllWindows()
{
    window.deleteAndZero();
}