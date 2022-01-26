/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "State.h"
#include "KnobStyles.h"

//==============================================================================
/**
*/


class PhaserXtremeAudioProcessorEditor  : public AudioProcessorEditor, private Timer
{
public:
    PhaserXtremeAudioProcessorEditor (PhaserXtremeAudioProcessor&);
    ~PhaserXtremeAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void customSliderStyle(Slider &s, int width, int height);
    void customLabelStyle(Label &l, Slider &s, const String &name);
    void customButtonLabelStyle(Label &l, ImageButton &b, const String &name);
    void customButtonStyle(ImageButton &b);
    void timerCallback() override;
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PhaserXtremeAudioProcessor& processor;
    GeneralLookAndFeel customFeel;
    LookAndFeel_250 BigKnobFeel; //for speed, rhythm
    MixKnobLookAndFeel MixKnobFeel;
    DepthKnobLookAndFeel DepthKnobFeel;
    CutoffKnobLookAndFeel CutoffKnobFeel;
    IntensityKnobLookAndFeel IntensityKnobFeel;
    SaturationKnobLookAndFeel SaturationKnobFeel;
    FeedbackKnobLookAndFeel FeedbackKnobFeel;
    WobbleKnobLookAndFeel WobbleKnobFeel;
    
    LabelLAF LLAF;

    //preset handling
    StateComponent stateComponent;
    
    //param GUI attachments
    Slider lfoSpeedSlider;
    Slider subdivisionSlider;
    Slider feedbackSlider;
    Slider cutoffSlider;
    Slider modDepthSlider;
    Slider depthSlider;
    Slider mixSlider;
    Slider cutoffLFOSlider;
    Slider inputGainSlider;
    Slider outputGainSlider;
    Slider saturationSlider;
    Slider bpmSlider;
    Slider attackSlider;
    Slider releaseSlider;
    Slider threshSlider;
    
    ImageButton bypassButton;
    ImageButton tempoSyncButton;
//    ImageButton bpmModeButton;
    ImageButton quadModeButton;
    
    ComboBox modTypeSelector;
    ComboBox modeSelector;
    
    //param labels
    Label modTypeLabel;
    Label inputGainLabel;
    Label outputGainLabel;
    Label bypassLabel;
    Label tempoSyncLabel;
    Label bpmLabel;
    Label bpmModeLabel;
    Label quadModeLabel;
    
    //title label;
    Label pluginTitle;
    Rectangle<int> presetArea;
    Label presetLabel;
    Label creatorLabel;
    
    // Style variables: fonts, colors, images
    Image background_s = ImageCache::getFromMemory(BinaryData::Background_speed_png, BinaryData::Background_speed_pngSize);
    Image background_r = ImageCache::getFromMemory(BinaryData::Background_rhythm_png, BinaryData::Background_rhythm_pngSize);
    
    Image button_on = ImageCache::getFromMemory(BinaryData::Button_On_png, BinaryData::Button_On_pngSize);
    Image button_off = ImageCache::getFromMemory(BinaryData::Button_Off_png, BinaryData::Button_Off_pngSize);
    
    Typeface::Ptr tface_title = Typeface::createSystemTypefaceFor(BinaryData::aAmbangResesi_ttf, BinaryData::aAmbangResesi_ttfSize);
    Typeface::Ptr tface_music = Typeface::createSystemTypefaceFor(BinaryData::Rhyth_otf, BinaryData::Rhyth_otfSize);
    Font bodyFont;
    Font Resesi;
    Font MusiSync;
    
    Colour myOrange = Colour::fromRGB(219, 78, 31);
    Colour myGray = Colour::fromRGB(60, 69, 82);
    Colour myCream = Colour::fromRGB(214, 183, 153);
    Colour myNavy = Colour::fromRGB(30,40,50);
    
    double lastBPM;
    int lastMode;
    Label rhythmNameLabel;
//    juce::String rhythmNames[numSubdivisions] = {"1/16th","1/8th trip","1/8th","1/4th trip","1/8th dot","1/4th","1/2 trip","1/4th dot","1/2","1/2 dot","Whole"};
    juce::String rhythmNames[numSubdivisions] = {" w", "h.", "h", "q.", "phhh", "q", "e.", "pqqq", "ry", "rTy", "dffg"};
    
    
    void tempoSyncToggle(ImageButton &b)
    {
        if (b.getToggleState())
        {
            lastBPM = bpmSlider.getValue();
            bpmSlider.setValue(processor.getDawBPM());
        }
        else if (!b.getToggleState())
        {
            bpmSlider.setValue(lastBPM);
        }
    }

    
    
public: //have to do public again so when plugin closes, GUI values are destroyed before GUI components to avoid crashes
    
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> lfoSpeedSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> feedbackSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> cutoffSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> modDepthSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::ComboBoxAttachment> modTypeSelectorValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> depthSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::ButtonAttachment> bypassButtonValue;
    std::unique_ptr <AudioProcessorValueTreeState::ButtonAttachment> tempoSyncButtonValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> mixSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> subdivisionSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> cutoffLFOSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> inputGainSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> outputGainSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> saturationSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> bpmSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::ComboBoxAttachment> modeButtonValue;
    std::unique_ptr <AudioProcessorValueTreeState::ButtonAttachment> quadModeButtonValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> attackSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> releaseSliderValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> threshSliderValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaserXtremeAudioProcessorEditor)
};
