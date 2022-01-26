/*
  ==============================================================================

    State.h
    Created: 2 Sep 2020 1:16:45pm
    Author:  Sam Schachter

  ==============================================================================
*/

#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "KnobStyles.h"

void saveStateToXML(const AudioProcessor &proc, XmlElement &xml);
void loadStateFromXML(const XmlElement &xml, AudioProcessor &proc);
void createFileIfNonExistant (const File& file); // used only when using plugin first time or if preset file gets deleted
void parseFileToXmlElement (const File& file, XmlElement& xml); //grabs preset values from XML
void writeXmlElementToFile (const XmlElement& xml, File& file); //writes preset values to XML
String getNextAvailablePresetID (const XmlElement& presetXml);

class StatePresets
{
public:
    StatePresets (AudioProcessor& proc, const String& presetFileLocation)
    : pluginProcessor {proc},
      presetFile {File::getSpecialLocation (File::userApplicationDataDirectory)
                    .getChildFile (presetFileLocation)}
    {
        parseFileToXmlElement(presetFile, presetXml);
    }

    ~StatePresets()
    {
        writeXmlElementToFile(presetXml, presetFile);
    }

    void savePreset (const String& presetName); // preset already exists? confirm overwrite
    void loadPreset (int presetID);  // grabs preset given ID
    void deletePreset(); //deletes
    StringArray getPresetNames() const;

    int getNumPresets() const {return presetXml.getNumChildElements();}
    int getCurrentPresetId() const {return currentPresetID;}

private:
    AudioProcessor& pluginProcessor;
    XmlElement presetXml {"Presets"}; // local, in-plugin representation
    File presetFile;                  // on-disk representation
    int currentPresetID {0};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StatePresets);
};

class StateComponent  : public Component,
                        public Button::Listener,
                        public ComboBox::Listener
{
public:
    StateComponent (StatePresets& sp);
    
    void paint (Graphics&) override;
    void resized() override;

private:
    StatePresets& procStatePresets;
    GeneralLookAndFeel customFeel;

    ComboBox   presetBox;
    PopupMenu  guitarMenu,drumMenu,keysMenu;
    TextButton savePresetButton;
    TextButton deletePresetButton;

    void buttonClicked (Button* clickedButton) override;
    void comboBoxChanged (ComboBox* changedComboBox) override;
    
    void refreshPresetBox(); // redraws preset box when element is added/deleted
    void ifPresetActiveShowInBox(); //puts current preset into main window
    void savePresetAlertWindow(); // creates a pop-up to name and save a custom preset
    void deletePresetAlertWindow(); //creates a pop-up to delete a custom preset
    void populateComboBox (ComboBox& comboBox, const StringArray& listItems);
    
    Colour myOrange = Colour::fromRGB(219, 78, 31);
    Colour myGray = Colour::fromRGB(60, 69, 82);
    Colour myCream = Colour::fromRGB(214, 183, 153);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateComponent);
};

#endif //STATE_H_INCLUDED
