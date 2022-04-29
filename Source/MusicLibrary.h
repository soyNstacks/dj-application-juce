/*
  ==============================================================================

    MusicLibrary.h
    Created: 2 Mar 2022 9:35:57am
    Author:  taan0

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "Tracklist.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

//==============================================================================

class MusicLibrary : public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener,
    public juce::TextEditor::Listener
{
public:
    MusicLibrary(
        DJAudioPlayer* djAudioPlayer,
        DeckGUI* deck1,
        DeckGUI* deck2,
        AudioFormatManager& formatManagerToUse);
    ~MusicLibrary() override;

    //==============================================================================

    void paint(juce::Graphics&) override;
    void resized() override;

    //====================== functions to paint ====================================

    void paintRowBackground(Graphics&,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;

    void paintCell(Graphics&,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber,
        int columnId,
        bool isRowSelected,
        Component* existingComponentToUpdate) override;

    //==============================================================================

    /** get total number of rows to populate */
    int getNumRows() override;

    //==============================================================================

    /** button listener */
    void buttonClicked(Button* button) override;

    /** mouse listener for draggable component*/
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;

private:

    DJAudioPlayer* djAudioPlayer;
    DeckGUI* deck1;
    DeckGUI* deck2;

    /** music library as a draggable component */
    ComponentDragger dragger;

    //==============================================================================

    /** listeners for TextEditor */
    void textEditorReturnKeyPressed(TextEditor& textEditor);
    void textEditorTextChanged(TextEditor&);

    //==============================================================================

    /** button declaration */
    juce::ImageButton addButton;
    juce::TextButton resetSearchButton{ "Clear" };
    juce::TextEditor searchBar;

    //==============================================================================

    /** TableListBox declaration to create a playlist table */
    TableListBox playlistLibrary;
    /** declaration for vector of Tracklist  */
    std::vector<Tracklist> tracksVector;


    //==============================================================================

    std::unique_ptr<juce::XmlElement> xmlData;

    /** implement function to restore playlist upon restarting application */
    void restoreLibrary();
    /** implement function to write data to file */
    void writeToXML(const juce::File& file);
    /** implement function to save and display data upon closing application */
    void saveLibrary();
    /** implement function to delete track and its data */
    void deleteFromPlaylist(int id);
    /** implement to return string for audio length */
    juce::String durationInMinSec(int durationInSec);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MusicLibrary)
};