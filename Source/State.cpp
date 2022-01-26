/*
  ==============================================================================

    State.cpp
    Created: 2 Sep 2020 1:16:45pm
    Author:  Sam Schachter

  ==============================================================================
*/

#include "State.h"

void saveStateToXML(const AudioProcessor &proc, XmlElement &xml)
{
    xml.removeAllAttributes(); //clear xml file

    for (const auto& param : proc.getParameters()){
        if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param)){
            xml.setAttribute (p->paramID, p->getValue()); // 0to1
        }
    }
}

void loadStateFromXML(const XmlElement &xml, AudioProcessor &proc)
{
    for (const auto& param : proc.getParameters()){
        if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param)){// if not in xml set current
            p->setValueNotifyingHost (static_cast<float>(xml.getDoubleAttribute (p->paramID, p->getValue())));
        }
    }
}

void createFileIfNonExistant (const File& file)
{
    if (!file.exists())
        file.create();

    jassert (file.exists());
}

void parseFileToXmlElement (const File& file, XmlElement& xml)
{
    std::unique_ptr<XmlElement> parsed {XmlDocument::parse(file)};
    if (parsed) {
        xml = *parsed;
    }
}

void writeXmlElementToFile (const XmlElement& xml, File& file)
{
    createFileIfNonExistant (file);
    xml.writeTo(file);
}

String getNextAvailablePresetID (const XmlElement& presetXml)
{
    int newPresetIDNumber = presetXml.getNumChildElements() + 1; // 1 indexed to match ComboBox
    return "Preset" + static_cast<String> (newPresetIDNumber);   // format: preset##
}

void StatePresets::savePreset (const String& presetName) // preset already exists? confirm overwrite
{
    String newPresetID = getNextAvailablePresetID (presetXml); // presetID format: "preset##"

    std::unique_ptr<XmlElement> currentState {new XmlElement {newPresetID}};    // must be pointer as
    saveStateToXML (pluginProcessor, *currentState);                            // parent takes ownership
    currentState->setAttribute ("presetName", presetName);

    presetXml.addChildElement (currentState.release());                         // will be deleted by parent element
}

void StatePresets::loadPreset (int presetID)
{
    if (1 <= presetID && presetID <= presetXml.getNumChildElements()) // 1 indexed to match ComboBox
    {
        XmlElement loadThisChild {*presetXml.getChildElement (presetID - 1)}; // (0 indexed method)
        loadStateFromXML (loadThisChild, pluginProcessor);
    }
    currentPresetID = presetID; // allow 0 for 'no preset selected' (?)
}

void StatePresets::deletePreset()
{
    XmlElement* childToDelete {presetXml.getChildElement (currentPresetID - 1)};
    if (childToDelete)
        presetXml.removeChildElement (childToDelete, true);
}

StringArray StatePresets::getPresetNames() const
{
    StringArray names;

    forEachXmlChildElement(presetXml, child)   // should avoid macro?
    {
        String n = child->getStringAttribute("presetName");
        if (n == "")
            n = "(Unnamed preset)";
        names.add (n);
    }
    return names; // hopefully moves
}
//Component stuff

void StateComponent::populateComboBox (ComboBox& comboBox, const StringArray& listItems)
{
    auto mainMenu = comboBox.getRootMenu();
    
    mainMenu->addSubMenu("Guitar", guitarMenu);
    mainMenu->addSubMenu("Keys", keysMenu);
    mainMenu->addSubMenu("Drums", drumMenu);
    for (int i = 0; i < listItems.size(); ++i)
        comboBox.addItem (listItems[i], i + 1); // 1-indexed ID for ComboBox
}

StateComponent::StateComponent(StatePresets& sp) : procStatePresets {sp}, savePresetButton {"Save preset"}, deletePresetButton {"Delete preset"}
{
    addAndMakeVisible(savePresetButton);
    addAndMakeVisible(deletePresetButton);
    addAndMakeVisible(presetBox);
    
    savePresetButton.addListener(this);
    deletePresetButton.addListener(this);
    presetBox.addListener(this);
    
    savePresetButton.setColour(TextButton::buttonOnColourId, myGray);
    savePresetButton.setColour(TextButton::buttonColourId, myGray);
    savePresetButton.setLookAndFeel(&customFeel);
    
    deletePresetButton.setColour(TextButton::buttonOnColourId, myGray);
    deletePresetButton.setColour(TextButton::buttonColourId, myGray);
    deletePresetButton.setLookAndFeel(&customFeel);
    
    presetBox.setTextWhenNothingSelected("Load preset");
    presetBox.setLookAndFeel(&customFeel);
    refreshPresetBox();
    ifPresetActiveShowInBox();
}
void StateComponent::paint(Graphics &)
{
    
}

void StateComponent::resized()
{
    savePresetButton.setBounds(getLocalBounds().getWidth()/2.5, 2, 150, 25);
    deletePresetButton.setBounds(getLocalBounds().getWidth()/2.5 + 175, 2, 150, 25);
    presetBox.setBounds(getLocalBounds().getWidth()/2.5+350, 2, 250, 25);
}

void StateComponent::buttonClicked (Button* clickedButton)
{
    if (clickedButton == &savePresetButton)
        savePresetAlertWindow();
        
    if (clickedButton == &deletePresetButton)
        deletePresetAlertWindow();
}

void StateComponent::comboBoxChanged (ComboBox* changedComboBox)
{
    const int selectedID {changedComboBox->getSelectedId()};
    procStatePresets.loadPreset(selectedID);
}

void StateComponent::refreshPresetBox()
{
    presetBox.clear();
    StringArray presetNames {procStatePresets.getPresetNames()};
    
    populateComboBox(presetBox, presetNames);

}

void StateComponent::ifPresetActiveShowInBox()
{
    const int currentPreset = procStatePresets.getCurrentPresetId();
    const int numPresets = procStatePresets.getNumPresets();
    
    if (1 <= currentPreset && currentPreset <= numPresets)
        presetBox.setSelectedId(currentPreset);
}

void StateComponent::savePresetAlertWindow()
{
    enum choice {ok, cancel};
    
    AlertWindow alert {"Save your preset:", "", AlertWindow::AlertIconType::NoIcon};
    alert.setLookAndFeel(&customFeel);
    alert.setColour(AlertWindow::backgroundColourId, myGray);
    alert.addTextEditor("presetEditorID", "Name your preset:");
    alert.addButton("OK", choice::ok, KeyPress(KeyPress::returnKey,0,0));
    alert.addButton("Cancel", choice::cancel, KeyPress(KeyPress::escapeKey,0,0));
    
    if (alert.runModalLoop()  == choice::ok)
    {
        String presetName {alert.getTextEditorContents("presetEditorID")};
        procStatePresets.savePreset(presetName);
        refreshPresetBox();
        presetBox.setSelectedId(procStatePresets.getNumPresets());
    }
}

void StateComponent::deletePresetAlertWindow()
{
    enum choice {yes, cancel};
    
    AlertWindow alert {"Delete Preset?", "", AlertWindow::AlertIconType::NoIcon};
    alert.setLookAndFeel(&customFeel);
    alert.setColour(AlertWindow::backgroundColourId, myGray);
    alert.addButton("Delete", choice::yes, KeyPress(KeyPress::returnKey,0,0));
    alert.addButton("Cancel", choice::cancel, KeyPress(KeyPress::escapeKey,0,0));
    
    if (alert.runModalLoop() == choice::yes)
    {
        procStatePresets.deletePreset();
        refreshPresetBox();
    }
}
