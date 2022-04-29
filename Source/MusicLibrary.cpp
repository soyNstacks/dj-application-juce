/*
  ==============================================================================

    MusicLibrary.cpp
    Created: 2 Mar 2022 9:35:57am
    Author:  taan0

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MusicLibrary.h"

//==============================================================================
MusicLibrary::MusicLibrary(DJAudioPlayer* _djAudioPlayer,
    DeckGUI* _deck1,
    DeckGUI* _deck2,
    AudioFormatManager& formatManagerToUse

)
    : djAudioPlayer(_djAudioPlayer),
    deck1(_deck1),
    deck2(_deck2)

{
    // add column headers
    playlistLibrary.getHeader().addColumn("ID", 1, 50);
    playlistLibrary.getHeader().addColumn("Track Title", 2, 350);
    playlistLibrary.getHeader().addColumn("Duration", 3, 200);
    playlistLibrary.getHeader().addColumn("", 4, 100);
    playlistLibrary.getHeader().addColumn("", 5, 100);
    playlistLibrary.getHeader().addColumn("", 6, 100);

    playlistLibrary.setModel(this);
    playlistLibrary.setMultipleSelectionEnabled(true);

    // visibility of components and widgets 
    addAndMakeVisible(playlistLibrary);

    addAndMakeVisible(addButton);
    addAndMakeVisible(resetSearchButton);
    addAndMakeVisible(searchBar);

    // add listener to widgets 
    addButton.addListener(this);
    resetSearchButton.addListener(this);
    searchBar.addListener(this);

    searchBar.setTextToShowWhenEmpty("Enter track title",
        juce::Colours::white
    );

    // image as button 
    Image importImage = ImageCache::getFromMemory(BinaryData::openFolder_png, BinaryData::openFolder_pngSize);
    addButton.setImages(true,
        true,
        true,
        importImage, 1.0f, {},
        importImage, 0.7f, {},
        importImage, 0.2f, {},
        0.1f);

    getLookAndFeel().setColour(TableListBox::backgroundColourId, juce::Colour::fromRGB(26, 27, 26));
    getLookAndFeel().setColour(TableListBox::outlineColourId, juce::Colour::fromRGB(213, 212, 207));
    getLookAndFeel().setColour(TableHeaderComponent::backgroundColourId, juce::Colour::fromRGB(63, 63, 62));

    // restore playlist contents upon starting the application 
    restoreLibrary();
}

MusicLibrary::~MusicLibrary()
{
    // call in destructor to save data in the playlist upon closing the playlist 
    saveLibrary();
}

void MusicLibrary::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(12, 13, 11));   // fill the background

    g.setColour(juce::Colour::fromRGB(213, 212, 207));
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colour::fromRGB(26, 27, 26));
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(),
        juce::Justification::centred, true);   // draw placeholder text

}

void MusicLibrary::resized()
{
    // set bounds of components and widgets
    searchBar.setBounds(0, 0, getWidth() / 2, getHeight() / 6);
    resetSearchButton.setBounds(getWidth() / 2 + 5, 0, 65, getHeight() / 6);
    addButton.setBounds(getWidth() / 2 + 70, 0, getWidth() * 0.06, getHeight() / 6);
    playlistLibrary.setBounds(0, getHeight() / 5.5, getWidth(), getHeight() - (getHeight() / 5.5));
}

int MusicLibrary::getNumRows()
{
    // return size of vector as total number of rows to populate in table 
    return tracksVector.size();
}

void MusicLibrary::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colour::fromRGB(150, 166, 182));

    }
    else {
        g.fillAll(juce::Colour::fromRGB(26, 27, 26));
    }
}

void MusicLibrary::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    g.setColour(juce::Colours::white);

    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            // track ID
            g.drawText(juce::String(rowNumber + 1),
                2,
                0,
                width - 4,
                height,
                Justification::centredLeft,
                true);
        }
        if (columnId == 2)
        {
            // track title
            g.drawText(tracksVector[rowNumber].trackTitle,
                2,
                0,
                width - 4,
                height,
                Justification::centredLeft,
                true);
        }
        if (columnId == 3)
        {
            // track length
            g.drawText(juce::String(tracksVector[rowNumber].trackDuration),
                2,
                0,
                width - 4,
                height,
                Justification::centredLeft,
                true);
        }
    }
}

Component* MusicLibrary::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{
    if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr) // haven't created component yet
        {
            TextButton* btn = new TextButton{ "LEFT DECK" };
            btn->setSize(getLocalBounds().getWidth() * 0.8, getLocalBounds().getHeight());
            // convert from integer to string
            String id{ std::to_string(rowNumber) };
            // set ID for easier access in event listener function 
            btn->setComponentID(id);
            // set name for easier access in event listener function 
            btn->setName("loadLeft");
            btn->addListener(this);
            existingComponentToUpdate = btn;

        }
    }
    if (columnId == 5)
    {
        if (existingComponentToUpdate == nullptr) // haven't created component yet
        {
            TextButton* btn = new TextButton{ "RIGHT DECK" };
            btn->setSize(getLocalBounds().getWidth() * 0.8, getLocalBounds().getHeight());
            // convert from integer to string
            String id{ std::to_string(rowNumber) };
            // set ID for easier access in event listener function 
            btn->setComponentID(id);
            // set name for easier access in event listener function 
            btn->setName("loadRight");
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 6)
    {
        if (existingComponentToUpdate == nullptr) // haven't created component yet
        {
            TextButton* btn = new TextButton{ "DELETE" };
            btn->setSize(getLocalBounds().getWidth() * 0.8, getLocalBounds().getHeight());
            // convert from integer to string
            String id{ std::to_string(rowNumber) };
            // set ID for easier access in event listener function 
            btn->setComponentID(id);
            // set name for easier access in event listener function 
            btn->setName("delete");
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }

    return existingComponentToUpdate;
}

void MusicLibrary::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    juce::String userInput = textEditor.getText();
    DBG("textEditorReturnKeyPressed Pressed: " << " Search for track " << userInput);

    auto it = find_if(tracksVector.begin(), tracksVector.end(), [&userInput](const Tracklist& obj)
        {
            return obj.trackTitle.containsWholeWordIgnoreCase(userInput);
        });

    if (it != tracksVector.end())
    {
        // select if title found
        auto rowNumber = std::distance(tracksVector.begin(), it);
        playlistLibrary.selectRow(rowNumber);
        DBG("found");
    }
    else {
        // alert window if title not found
        DBG("Not Found");
        searchBar.clear();
        playlistLibrary.deselectAllRows();
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon,
            "Title not found.",
            "Please check if the correct title has been entered, or add to playlist.",
            "Close Pop-up");
    }
}

void MusicLibrary::textEditorTextChanged(TextEditor& textEditor)
{
    juce::String userInput = textEditor.getText();

    auto it = find_if(tracksVector.begin(), tracksVector.end(), [&userInput](const Tracklist& obj) {return obj.trackTitle.contains(userInput); });

    if (it != tracksVector.end())
    {
        // select if title found 
        auto rowNumber = std::distance(tracksVector.begin(), it);
        playlistLibrary.selectRow(rowNumber);
        DBG("found");
    }
    if (userInput == "")   // empty field
        playlistLibrary.deselectAllRows();
}

void MusicLibrary::buttonClicked(Button* button)
{
    if (button == &addButton)
    {
        FileChooser chooser("Select a file to add to playlist...", juce::File::getCurrentWorkingDirectory());
        if (chooser.browseForMultipleFilesToOpen())
        {
            for (const juce::File& chosen : chooser.getResults())
            {
                Tracklist addTrack{ chosen };

                juce::AudioFormatManager formatManager;
                formatManager.registerBasicFormats();
                std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(chosen));

                if (reader.get() != nullptr)
                {
                    auto durationInSec = (float)reader->lengthInSamples / reader->sampleRate;
                    auto convertedDuration = durationInMinSec(roundFloatToInt(durationInSec));
                    addTrack.trackDuration = convertedDuration;
                }

                tracksVector.push_back(addTrack);
            }
        }

        playlistLibrary.updateContent();
    }
    if (button == &resetSearchButton)
    {
        searchBar.clear();
        playlistLibrary.deselectAllRows();
    }
    if (button->getName() == "delete")
    {
        int id = std::stoi(button->getComponentID().toStdString());
        deleteFromPlaylist(id);
        DBG("PlaylistComponent::buttonClicked: Delete " << tracksVector[id].trackTitle << id);
    }
    if (button->getName() == "loadLeft")
    {
        int id = std::stoi(button->getComponentID().toStdString());
        DBG("PlaylistComponent::buttonClicked: Load " << tracksVector[id].trackTitle << "/nbutton->getName(): " << button->getName() << id);
        deck1->loadToDeck(tracksVector[id].URL);
    }
    if (button->getName() == "loadRight")
    {
        int id = std::stoi(button->getComponentID().toStdString());
        DBG("PlaylistComponent::buttonClicked: Load " << tracksVector[id].trackTitle << "/nbutton->getName(): " << button->getName() << id);
        deck2->loadToDeck(tracksVector[id].URL);
    }
}

juce::String MusicLibrary::durationInMinSec(int durationInSec)
{
    int seconds, minutes;
    minutes = durationInSec / 60;
    seconds = durationInSec % 60;

    juce::String mmss;
    mmss << juce::String(minutes) << ":" << juce::String(seconds);
    return mmss;
}

void MusicLibrary::saveLibrary()
{
    juce::File libraryFile = juce::File::getCurrentWorkingDirectory().getChildFile("musicLibrary.xml");

    if (!libraryFile.existsAsFile())
    {
        DBG("Creating XML file... ");
        std::ofstream musicLibrary("musicLibary.xml");
        musicLibrary.open("musicLibary.xml");

        writeToXML(libraryFile);
        musicLibrary.close();
        
        DBG("written to file and closed");

    }
    else if (libraryFile.existsAsFile())
    {
        writeToXML( libraryFile );

        DBG("File already exists and data has been successfully written to file");
    }
}

void MusicLibrary::writeToXML(const juce::File& file)
{
    juce::XmlElement rootElem("playlist");
    int id = 1;
    for (auto& track : tracksVector)
    {
        auto elem = new juce::XmlElement("track");
        elem->setAttribute("id", id);
        elem->setAttribute("fileURL", track.URL.toString(false));
        elem->setAttribute("title", track.trackTitle);
        elem->setAttribute("duration", track.trackDuration);
        id++;
        
        rootElem.addChildElement(elem);
     
    }

    rootElem.writeTo(juce::File{ file });
    
    DBG("numchild: " << rootElem.getNumChildElements());
}

void MusicLibrary::restoreLibrary()
{
    juce::File libraryFile = juce::File::getCurrentWorkingDirectory().getChildFile("musicLibrary.xml");

    if (!libraryFile.existsAsFile())
    {
        DBG("File does not exist");
        return;
    }

    else if (libraryFile.existsAsFile()) {
        juce::FileInputStream inputFile(libraryFile);
        auto xmlData = parseXML(libraryFile);
        juce::XmlElement* dataList;

        if (inputFile.openedOk())
        {
            DBG("input file openedOk()");

            if (parseXMLIfTagMatches(libraryFile, "playlist"))
            {
                dataList = xmlData->getChildByName("track"); // <track>

                for (auto* elem : xmlData->getChildIterator())
                {
                    Tracklist addTrack{ libraryFile };
                    addTrack.URL = elem->getStringAttribute("fileURL");
                    addTrack.trackDuration = elem->getStringAttribute("duration");
                    addTrack.trackTitle = elem->getStringAttribute("title");
                    addTrack.trackId = elem->getStringAttribute("id");
                    tracksVector.push_back(addTrack);
                }

                DBG("restored tracksVector size " << tracksVector.size());
            }
            else
            {
                DBG("Invalid or no playlist element");
                libraryFile.deleteFile();   // delete file if xml file has invalid or no elements
            }
        }
    }
}

void MusicLibrary::deleteFromPlaylist(int id)
{
    if (id < tracksVector.size() - 1)
    {
        tracksVector.erase(tracksVector.begin() + id);
        playlistLibrary.updateContent();

        DBG("index: " << juce::String(id) << " .... tracksVector size: " << tracksVector.size());
    }
    else if (id == tracksVector.size() - 1)
    {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon,
            "Warning: No Empty Playlist",
            "Last Track. Please do not delete.",
            "Close");
    }
}

void MusicLibrary::mouseDown(const MouseEvent& e)
{
    dragger.startDraggingComponent(this, e);
}

void MusicLibrary::mouseDrag(const MouseEvent& e)
{
    dragger.dragComponent(this, e, nullptr);
}