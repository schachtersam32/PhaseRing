/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define WIDTH 1100
#define HEIGHT 550
#define BUTTON_WIDTH 25
#define BUTTON_HEIGHT 25
#define LABEL_WIDTH 100
#define LABEL_HEIGHT 25
//==============================================================================
PhaserXtremeAudioProcessorEditor::PhaserXtremeAudioProcessorEditor (PhaserXtremeAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p), stateComponent {p.statePresets}
{
    // callback timer
    Timer::startTimer(30);
    
    //size of plugin window
    setSize (WIDTH, HEIGHT);
    
    // button width and height
    auto tb_width = 100;
    auto tb_height = 25;
    
    // font for knobs
    bodyFont = Font ("Courier New",18.0,Font::bold);
    
    // font for title
    Resesi = Font (*tface_title);
    
    // music font for BPM divisions
    MusiSync = Font (*tface_music);
    MusiSync.setSizeAndStyle(50.0f, 0, 1, 0);
    
    // attaching slider values to appropriate parameters
    lfoSpeedSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,LFO_SPEED_ID,lfoSpeedSlider);
    feedbackSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,FEEDBACK_ID,feedbackSlider);
    cutoffSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,CUTOFF_ID,cutoffSlider);
    modDepthSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,MOD_DEPTH_ID,modDepthSlider);
    modTypeSelectorValue = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameters,MOD_TYPE_ID,modTypeSelector);
    depthSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,DEPTH_ID,depthSlider);
    bypassButtonValue = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters,BYPASS_ID,bypassButton);
    mixSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,MIX_ID,mixSlider);
    subdivisionSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,RHYTHM_ID,subdivisionSlider);
    tempoSyncButtonValue = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters,SYNC_ID,tempoSyncButton);
    cutoffLFOSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,LFO_CUTOFF_ID,cutoffLFOSlider);
    inputGainSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,IN_ID,inputGainSlider);
    outputGainSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,OUT_ID,outputGainSlider);
    saturationSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,SAT_ID,saturationSlider);
    bpmSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,BPM_ID,bpmSlider);
    modeButtonValue = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameters,MODE_ID,modeSelector);
    quadModeButtonValue = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters,QUAD_MODE_ID,quadModeButton);
    attackSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,ATTACK_ID,attackSlider);
    releaseSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,RELEASE_ID,releaseSlider);
    threshSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,THRESH_ID,threshSlider);

    // PRESET BLOCK
    addAndMakeVisible(&stateComponent);
    
    // INPUT GAIN SLIDER
    
    inputGainSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    inputGainSlider.setTextBoxStyle(Slider::TextBoxRight,false,tb_width,tb_height);
    inputGainSlider.setTextBoxIsEditable(true);
    inputGainSlider.textFromValueFunction = nullptr;
    inputGainSlider.setTextValueSuffix(" dB");
    inputGainSlider.setRange(-24.0f,6.0f);
    inputGainSlider.setNumDecimalPlacesToDisplay(1);
    inputGainSlider.setLookAndFeel(&customFeel);
    inputGainSlider.setColour(Slider::thumbColourId, myOrange);
    inputGainSlider.setColour(Slider::textBoxTextColourId, myGray);
    inputGainSlider.setColour(Slider::trackColourId, myGray);
    inputGainSlider.setColour(Slider::textBoxOutlineColourId, myGray);
    addAndMakeVisible(&inputGainSlider);
    
    customLabelStyle(inputGainLabel, inputGainSlider, "Input Gain");
    inputGainLabel.setColour(Label::textColourId, myGray);
    addAndMakeVisible(&inputGainLabel);
    
    // OUTPUT GAIN SLIDER
    
    outputGainSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    outputGainSlider.setTextBoxStyle(Slider::TextBoxRight,false,tb_width,tb_height);
    outputGainSlider.setTextBoxIsEditable(true);
    outputGainSlider.textFromValueFunction = nullptr;
    outputGainSlider.setTextValueSuffix(" dB");
    outputGainSlider.setRange(-24.0f,6.0f);
    outputGainSlider.setNumDecimalPlacesToDisplay(1);
    outputGainSlider.setLookAndFeel(&customFeel);
    outputGainSlider.setColour(Slider::thumbColourId, myOrange);
    outputGainSlider.setColour(Slider::textBoxTextColourId, myGray);
    outputGainSlider.setColour(Slider::trackColourId, myGray);
    outputGainSlider.setColour(Slider::textBoxOutlineColourId, myGray);
    addAndMakeVisible(&outputGainSlider);
    
    customLabelStyle(outputGainLabel, outputGainSlider, "Output Gain");
    outputGainLabel.setColour(Label::textColourId, myGray);
    addAndMakeVisible(&outputGainLabel);
    
    // SATURATION SLIDER
    
    customSliderStyle(saturationSlider, tb_width, tb_height);
    saturationSlider.setRange(0.0f,1.0f);
    saturationSlider.setLookAndFeel(&SaturationKnobFeel);
    addAndMakeVisible(&saturationSlider);
    
    // LFO SPEED SLIDER - FREQ MODE
    
    customSliderStyle(lfoSpeedSlider, tb_width, tb_height);
    lfoSpeedSlider.setRange(0.1f,4000.f);
    lfoSpeedSlider.setSkewFactorFromMidPoint(100.0f);
    lfoSpeedSlider.setLookAndFeel(&BigKnobFeel);
    addAndMakeVisible(&lfoSpeedSlider);
    
    // RHYTHM LABEL - BPM MODE
    
    rhythmNameLabel.setJustificationType(Justification::centred);
    rhythmNameLabel.setLookAndFeel(&LLAF);
    rhythmNameLabel.setFont(MusiSync);
    rhythmNameLabel.setColour(Label::textColourId, Colours::white);
    rhythmNameLabel.setColour(Label::backgroundColourId, myNavy);
    addAndMakeVisible(&rhythmNameLabel);
    
    // RHYTHM SLIDER - BPM MODE

    customSliderStyle(subdivisionSlider, tb_width, tb_height);
    subdivisionSlider.setRange(0,10,1);
    subdivisionSlider.setLookAndFeel(&BigKnobFeel);
    subdivisionSlider.textFromValueFunction = [this](double value)
    {
        int r = static_cast<int>(value);
        rhythmNameLabel.setText(rhythmNames[r], dontSendNotification);
        return rhythmNames[r];
    };
    addAndMakeVisible(&subdivisionSlider);
    
    // ATTACK SLIDER - ENVELOPE MODE
    
    customSliderStyle(attackSlider, tb_width, tb_height);
    attackSlider.setRange(0.01f,1.f);
    attackSlider.setSkewFactorFromMidPoint(0.5f);
    attackSlider.setLookAndFeel(&MixKnobFeel);
    addAndMakeVisible(&attackSlider);
    
    // RELEASE SLIDER - ENVELOPE MODE
    
    customSliderStyle(releaseSlider, tb_width, tb_height);
    releaseSlider.setRange(0.01f,1.f);
    releaseSlider.setSkewFactorFromMidPoint(0.5f);
    releaseSlider.setLookAndFeel(&MixKnobFeel);
    addAndMakeVisible(&releaseSlider);
    
    // THRESHOLD SLIDER - ENVELOPE MODE
    
    customSliderStyle(threshSlider, tb_width, tb_height);
    threshSlider.setRange(0.5f,5.f);
    threshSlider.setSkewFactorFromMidPoint(1.f);
    threshSlider.setLookAndFeel(&MixKnobFeel);
    addAndMakeVisible(&threshSlider);
    
    // FEEDBACK SLIDER
    
    customSliderStyle(feedbackSlider, tb_width, tb_height);
    feedbackSlider.setRange(0.0f,100.0f);
    feedbackSlider.setLookAndFeel(&FeedbackKnobFeel);
    addAndMakeVisible(&feedbackSlider);
    
    // CUTOFF SLIDER

    customSliderStyle(cutoffSlider, tb_width, tb_height);
    cutoffSlider.setRange(2000.0f,15000.f);
    cutoffSlider.setSkewFactorFromMidPoint(8000.0f);
    cutoffSlider.setLookAndFeel(&CutoffKnobFeel);
    addAndMakeVisible(&cutoffSlider);
    
    // LFO AMPLITUDE SLIDER

    customSliderStyle(modDepthSlider, tb_width, tb_height);
    modDepthSlider.setRange(0.1f,1.0f);
    modDepthSlider.setLookAndFeel(&IntensityKnobFeel);
    addAndMakeVisible(&modDepthSlider);

    // DEPTH SLIDER
    
    customSliderStyle(depthSlider, tb_width, tb_height);
    depthSlider.setRange(2,48,2);
    depthSlider.setLookAndFeel(&DepthKnobFeel);
    addAndMakeVisible(&depthSlider);
    
    // LFO TYPE SELECTOR

    modTypeSelector.addItem("Triangle",1);
    modTypeSelector.addItem("Sine",2);
    modTypeSelector.addItem("Saw",3);
    modTypeSelector.addItem("Reverse Saw",4);
    modTypeSelector.addItem("Square", 5);
    modTypeSelector.addItem("Trapezoid", 6);
    modTypeSelector.addItem("Random", 7);
    modTypeSelector.setSelectedId(1);
    modTypeSelector.setLookAndFeel(&customFeel);
    addAndMakeVisible(&modTypeSelector);

    modTypeLabel.setText("Mod Type", dontSendNotification);
    modTypeLabel.setColour(Label::textColourId, myGray);
    modTypeLabel.setJustificationType(Justification::centred);
    modTypeLabel.attachToComponent(&modTypeSelector, false);
    modTypeLabel.setFont(bodyFont);
    addAndMakeVisible(&modTypeLabel);
    
    // MODE SELECTOR
    
    modeSelector.addItem("Frequency", 1);
    modeSelector.addItem("BPM",2);
    modeSelector.addItem("Envelope",3);
    modeSelector.setSelectedId(1);
//    modeSelector.setItemEnabled(3, false);
    modeSelector.setLookAndFeel(&customFeel);
    addAndMakeVisible(&modeSelector);
    
    lastMode = modeSelector.getSelectedId();
    
    // MIX SLIDER
    
    customSliderStyle(mixSlider, tb_width, tb_height);
    mixSlider.setRange(0.0f, 1.0f);
    mixSlider.setLookAndFeel(&MixKnobFeel);
    addAndMakeVisible(&mixSlider);
    
    // WOBBLE SLIDER

    customSliderStyle(cutoffLFOSlider, tb_width, tb_height);
    cutoffLFOSlider.setRange(0.0f,10.f);
    cutoffLFOSlider.setLookAndFeel(&WobbleKnobFeel);
    addAndMakeVisible(&cutoffLFOSlider);

    // BYPASS BUTTON
    
    customButtonStyle(bypassButton);
    customButtonLabelStyle(bypassLabel, bypassButton, "Bypass");
    addAndMakeVisible(bypassLabel);
    
    // TEMPO SYNC BUTTON
    
    customButtonStyle(tempoSyncButton);
    customButtonLabelStyle(tempoSyncLabel, tempoSyncButton, "DAW Sync");
    addAndMakeVisible(tempoSyncLabel);
    
//    customButtonStyle(bpmModeButton);
//    customButtonLabelStyle(bpmModeLabel, modeSelector, "Freq Mode");
//    addAndMakeVisible(bpmModeLabel);
    
    tempoSyncButton.onClick = [this]{tempoSyncToggle(tempoSyncButton);};
    
    // BPM SLIDER - BPM MODE
    
    customSliderStyle(bpmSlider, tb_width, tb_height);
    bpmSlider.setTextBoxStyle(Slider::TextBoxBelow, false, tb_width, tb_height);
    bpmSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    bpmSlider.setColour(Slider::IncDecButtons, myGray);
    bpmSlider.setColour(Slider::textBoxTextColourId, myGray);
    bpmSlider.setColour(Slider::textBoxOutlineColourId, myGray);
    bpmSlider.setIncDecButtonsMode(juce::Slider::IncDecButtonMode::incDecButtonsDraggable_Vertical);
    bpmSlider.setRange(30.0f, 300.0f, 1.0f);
    addAndMakeVisible(&bpmSlider);
    
    customLabelStyle(bpmLabel, bpmSlider, "BPM");
    bpmLabel.setColour(Label::textColourId, myGray);
    bpmLabel.attachToComponent(&bpmSlider, false);
    
    // QUADRATURE BUTTON
    
    customButtonStyle(quadModeButton);
    customButtonLabelStyle(quadModeLabel, quadModeButton, "Stereo");
    if (processor.getTotalNumInputChannels() == 1){
        quadModeButton.setEnabled(false);
        quadModeButton.setAlpha(0.5);
    }
    addAndMakeVisible(&quadModeLabel);
    
    // PRESET BAR LABEL

    presetLabel.setText("Presets", dontSendNotification);
    presetLabel.setLookAndFeel(&customFeel);
    presetLabel.setSize(200, 30);
    presetLabel.setFont(bodyFont);
    presetLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(&presetLabel);
    
    // TITLE CARD
    
    creatorLabel.setText("by SamSound", dontSendNotification);
    Resesi.setSizeAndStyle(18.0f, 0, 1, 0.2);
    creatorLabel.setFont(Resesi);
    creatorLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(&creatorLabel);
    
    pluginTitle.setText("The PhaseRing", dontSendNotification);
    Resesi.setSizeAndStyle(60.0f, 0, 1, 0.2);
    pluginTitle.setFont(Resesi);
    addAndMakeVisible(&pluginTitle);
}

PhaserXtremeAudioProcessorEditor::~PhaserXtremeAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

// FUNCTION TO SET SLIDER STYLE
void PhaserXtremeAudioProcessorEditor::customSliderStyle(Slider &s, int width, int height)
{
    s.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(Slider::NoTextBox,false,width,height);
    s.setTextBoxIsEditable(true);
    s.textFromValueFunction = nullptr;
    s.setLookAndFeel(&customFeel);
}

// FUNCTION TO SET SLIDER LABEL STYLE
void PhaserXtremeAudioProcessorEditor::customLabelStyle(Label &l, Slider &s, const String &name)
{
    l.setJustificationType(Justification::centred);
    l.setText(name, dontSendNotification);
    l.setLookAndFeel(&customFeel);
    l.setFont(bodyFont);
}

// FUNCTION TO SET BUTTON LABEL STYLE
void PhaserXtremeAudioProcessorEditor::customButtonLabelStyle(Label &l, ImageButton &b, const String &name)
{
    l.setJustificationType(Justification::centred);
    l.setText(name, dontSendNotification);
    l.setColour(Label::textColourId, myGray);
    l.setLookAndFeel(&customFeel);
    l.setFont(bodyFont);
}

// FUNCTION TO SET BUTTON STYLE
void PhaserXtremeAudioProcessorEditor::customButtonStyle (ImageButton &b)
{
    b.setClickingTogglesState(true);
    b.setImages(false, true, false, button_off, 1.0, {}, button_off, 0.5, {}, button_on, 1.0, {});
    b.setLookAndFeel(&customFeel);
    addAndMakeVisible(&b);
}

void PhaserXtremeAudioProcessorEditor::timerCallback()
{
    
    // CALLBACK THAT CHANGES GUI
    
    if (modeSelector.getSelectedId() == 2) //BPM mode
    {
        if(modeSelector.getSelectedId() != lastMode)
            repaint();
        
        lfoSpeedSlider.setVisible(false);
        removeChildComponent(&lfoSpeedSlider);
        
        attackSlider.setVisible(false);
        removeChildComponent(&attackSlider);
        releaseSlider.setVisible(false);
        removeChildComponent(&releaseSlider);
        threshSlider.setVisible(false);
        removeChildComponent(&threshSlider);
        
        subdivisionSlider.setVisible(true);
        addChildComponent(&subdivisionSlider);
        
        rhythmNameLabel.setVisible(true);
        int r = static_cast<int>(subdivisionSlider.getValue());
        rhythmNameLabel.setText(rhythmNames[r], dontSendNotification);

        bpmModeLabel.setText("BPM Mode", dontSendNotification);
        bpmSlider.setEnabled(true);
        bpmSlider.setAlpha(1.0);
        
        tempoSyncButton.setEnabled(true);
        tempoSyncButton.setAlpha(1.0);
        
        lastMode = 2;
    }
    else if (modeSelector.getSelectedId() == 1) // FREQ MODE
    {
        if(modeSelector.getSelectedId() != lastMode)
            repaint();
        
        subdivisionSlider.setVisible(false);
        removeChildComponent(&subdivisionSlider);
        
        attackSlider.setVisible(false);
        removeChildComponent(&attackSlider);
        releaseSlider.setVisible(false);
        removeChildComponent(&releaseSlider);
        threshSlider.setVisible(false);
        removeChildComponent(&threshSlider);
        
        lfoSpeedSlider.setVisible(true);
        addChildComponent(&lfoSpeedSlider);
        
        rhythmNameLabel.setVisible(false);
        
        bpmModeLabel.setText("Freq mode", dontSendNotification);
        bpmSlider.setEnabled(false);
        bpmSlider.setAlpha(0.5);
        
        tempoSyncButton.setEnabled(false);
        tempoSyncButton.setAlpha(0.5);
        tempoSyncButton.setToggleState(false, dontSendNotification);
        
        lastMode = 1;
    }
    
    else if (modeSelector.getSelectedId() == 3) // ENVELOPE MODE
    {
        if(modeSelector.getSelectedId() != lastMode)
            repaint();
        
        subdivisionSlider.setVisible(false);
        removeChildComponent(&subdivisionSlider);
        
        lfoSpeedSlider.setVisible(false);
        removeChildComponent(&lfoSpeedSlider);
        
        attackSlider.setVisible(true);
        addChildComponent(&attackSlider);
        releaseSlider.setVisible(true);
        addChildComponent(&releaseSlider);
        threshSlider.setVisible(true);
        addChildComponent(&threshSlider);
        
        rhythmNameLabel.setVisible(false);
        
        bpmSlider.setEnabled(false);
        bpmSlider.setAlpha(0.5);
        
        tempoSyncButton.setEnabled(false);
        tempoSyncButton.setAlpha(0.5);
        tempoSyncButton.setToggleState(false, dontSendNotification);
        
        lastMode = 3;
        
    }
    
    // BYPASS ON/OFF
    if (bypassButton.getToggleState())
        bypassButton.setButtonText("Bypass ON");
    else
        bypassButton.setButtonText("Bypass OFF");
    
    // TEMPO SYNC ON/OFF
    if(tempoSyncButton.getToggleState())
    {
        bpmSlider.setValue(processor.getDawBPM());
    }

}

//==============================================================================
void PhaserXtremeAudioProcessorEditor::paint (Graphics& g)
{
    // WILL REPAINT WHEN MODE SWITCHES, NEED TO IMPLEMENT GUI FOR 3RD MODE STILL

    if(modeSelector.getSelectedId() == 2) // BPM MODE
        g.drawImageAt(background_r, 0, 0);
    else if(modeSelector.getSelectedId() == 1) // FREQ MODE
        g.drawImageAt(background_s, 0, 0);
    else
        g.drawImageAt(background_s, 0, 0); // ENVELOPE MODE
    g.setColour(myOrange);
    g.fillRect(presetArea);

}

void PhaserXtremeAudioProcessorEditor::resized()
{
    
    // Placing components on GUI - super messy but it works.
    // NOTE: this will get redone soon
    auto area = getLocalBounds();
    presetArea = area.removeFromTop(30);
    presetLabel.setBounds(presetArea);
    stateComponent.setBounds(presetArea);
    
    auto bt_side = 250;
    auto knob_space = 20;
    
    lfoSpeedSlider.setBounds(2, area.getHeight()/3.5, bt_side, bt_side);
    subdivisionSlider.setBounds(2, area.getHeight()/3.5, bt_side, bt_side);
    rhythmNameLabel.setBounds(75, 395, LABEL_WIDTH, LABEL_HEIGHT+25);
    
    bypassButton.setBounds(1033, area.getHeight()-30, BUTTON_WIDTH, BUTTON_HEIGHT);
    bypassLabel.setBounds(1000, area.getHeight()-10, LABEL_WIDTH, LABEL_HEIGHT);
    
    modTypeSelector.setBounds(10, area.getHeight()-30, 150,25);
    
    bt_side -= 50;
    
    cutoffSlider.setBounds(220+knob_space,area.getHeight()/3,bt_side,bt_side);
    
    modeSelector.setBounds(30, presetArea.getHeight()+10, 120, 25);
    bpmModeLabel.setBounds(450, area.getHeight()-10, LABEL_WIDTH, LABEL_HEIGHT);
    
    quadModeButton.setBounds(450, area.getHeight()-30, BUTTON_WIDTH, BUTTON_HEIGHT);
    quadModeLabel.setBounds(415, area.getHeight()-10, LABEL_WIDTH, LABEL_HEIGHT);
    
    bpmSlider.setBounds(210, area.getHeight()-30, 100, 50);
    
    bt_side -= 25;
    
    depthSlider.setBounds(415+knob_space, area.getHeight()/2.8, bt_side, bt_side);
    
    tempoSyncButton.setBounds(363, area.getHeight()-30, BUTTON_WIDTH, BUTTON_HEIGHT);
    tempoSyncLabel.setBounds(330, area.getHeight()-10, LABEL_WIDTH, LABEL_HEIGHT);
    
    cutoffLFOSlider.setBounds(420,presetArea.getHeight()+30, 100,100);
    
    bt_side -= 25;
    
    modDepthSlider.setBounds(580+knob_space, area.getHeight()/2.57, bt_side, bt_side);
    
    bt_side -= 25;
    
    feedbackSlider.setBounds(720+knob_space, area.getHeight()/2.44, bt_side, bt_side);
    
    bt_side -= 10;
    
    saturationSlider.setBounds(840+knob_space, area.getHeight()/2.32, bt_side, bt_side);
    
    bt_side -= 5;
    
    mixSlider.setBounds(950+knob_space, area.getHeight()/2.3, bt_side, bt_side);
    attackSlider.setBounds(10, area.getHeight()/3.5, bt_side, bt_side);
    releaseSlider.setBounds(130, area.getHeight()/3.5, bt_side, bt_side);
    threshSlider.setBounds(80, area.getHeight()/2, bt_side, bt_side);
    
    inputGainSlider.setBounds(700, area.getHeight()-60, 300, 40);
    outputGainSlider.setBounds(700, area.getHeight()-10, 300, 40);
    inputGainLabel.setBounds(670, area.getHeight()-70, 300, 20);
    outputGainLabel.setBounds(670, area.getHeight()-20, 300, 20);
    
    pluginTitle.setBounds(600, presetArea.getHeight()+30, 450, 100);
    creatorLabel.setBounds(895, presetArea.getHeight()+110, 150, 30);
    
}
