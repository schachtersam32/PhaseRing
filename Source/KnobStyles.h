/*
  ==============================================================================

    KnobStyles.h
    Created: 28 Dec 2020 4:04:17pm
    Author:  Sam Schachter

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class LabelLAF : public LookAndFeel_V4
{
public:
    LabelLAF(){}
    
    void drawLabel (Graphics& g, Label& label) override
    {
//        g.fillAll (label.findColour (Label::backgroundColourId));
        g.setColour(label.findColour(Label::backgroundColourId));
        float cornersize = 50.f;
        g.fillRoundedRectangle(label.getLocalBounds().toFloat(), cornersize);

        if (! label.isBeingEdited())
        {
            auto alpha = label.isEnabled() ? 1.0f : 0.5f;
            const Font font (getLabelFont (label));

            g.setColour (label.findColour (Label::textColourId).withMultipliedAlpha (alpha));
            g.setFont (font);

            auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());

            g.drawFittedText (label.getText(), textArea, label.getJustificationType(),
                              jmax (1, (int) ((float) textArea.getHeight() / font.getHeight())),
                              label.getMinimumHorizontalScale());

            g.setColour (label.findColour (Label::outlineColourId).withMultipliedAlpha (alpha));
        }
        else if (label.isEnabled())
        {
            g.setColour (label.findColour (Label::outlineColourId));
        }

        g.drawRoundedRectangle(label.getLocalBounds().toFloat(), cornersize, 1);
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabelLAF);
};

class GeneralLookAndFeel : public LookAndFeel_V4
{
public:
    GeneralLookAndFeel()
    {
        bodyFont = Font ("Courier New",18.0f,Font::bold);
    }
    
    Font getComboBoxFont(ComboBox &) override
    {
        return bodyFont;
    }

    Font getPopupMenuFont() override
    {
        return bodyFont;
    }

    Font getLabelFont (Label &) override
    {
        return bodyFont;
    }

    Font getTextButtonFont (TextButton &, int buttonHeight) override
    {
        return bodyFont;
    }
    
    Font getAlertWindowFont() override
    {
        return bodyFont;
    }
    
    Font getAlertWindowTitleFont() override
    {
        return bodyFont;
    }
    
    Font getAlertWindowMessageFont() override
    {
        return bodyFont;
    }
    
private:
    Font bodyFont;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneralLookAndFeel);

};

class LookAndFeel_250 : public LookAndFeel_V4
{
public:
    LookAndFeel_250(){}
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
        {
            Image myStrip;
            myStrip = ImageCache::getFromMemory (BinaryData::LFOSPEED_png, BinaryData::LFOSPEED_pngSize);
    
            //value between 0 and 1 for current amount of rotation
            const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
            const int frameIdx = static_cast<int>(ceil(sliderPos * (static_cast<double>(nFrames)-1.0)));
            
            const float radius = jmin (width / 2.0f, height / 2.0f) ;
            const float centreX = x + width * 0.5f;
            const float centreY = y + height * 0.5f;
            const float rx = centreX - radius - 1.0f;
            const float ry = centreY - radius /* - 1.0f*/;
    
            g.drawImage( myStrip, // image
                         static_cast<int>(rx),
                         static_cast<int>(ry),
                         myStrip.getWidth(),
                         myStrip.getWidth(),   // dest
                         0,
                         frameIdx*myStrip.getWidth(),
                         myStrip.getWidth(),
                         myStrip.getWidth()); // source
    
        }
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeel_250);

};

class MixKnobLookAndFeel : public LookAndFeel_V4
{
public:
    MixKnobLookAndFeel(){}
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
        {
            Image myStrip;
            myStrip = ImageCache::getFromMemory (BinaryData::MIX_png, BinaryData::MIX_pngSize);
    
            //value between 0 and 1 for current amount of rotation
            const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
            const int frameIdx = static_cast<int>(ceil(sliderPos * (static_cast<double>(nFrames)-1.0)));// current index from 0 --> nFrames-1
    
            const float radius = jmin (width / 2.0f, height / 2.0f) ;
            const float centreX = x + width * 0.5f;
            const float centreY = y + height * 0.5f;
            const float rx = centreX - radius - 1.0f;
            const float ry = centreY - radius /* - 1.0f*/;
    
            g.drawImage( myStrip, // image
                         static_cast<int>(rx),
                         static_cast<int>(ry),
                         myStrip.getWidth(),
                         myStrip.getWidth(),   // dest
                         0,
                         frameIdx*myStrip.getWidth(),
                         myStrip.getWidth(),
                         myStrip.getWidth()); // source
    
        }
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixKnobLookAndFeel);

};

class CutoffKnobLookAndFeel : public LookAndFeel_V4
{
public:
    CutoffKnobLookAndFeel(){}
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
        {
            Image myStrip;
            myStrip = ImageCache::getFromMemory (BinaryData::CUTOFF_png, BinaryData::CUTOFF_pngSize);
    
            //value between 0 and 1 for current amount of rotation
            const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
            const int frameIdx = static_cast<int>(ceil(sliderPos * (static_cast<double>(nFrames)-1.0)));// current index from 0 --> nFrames-1
    
            const float radius = jmin (width / 2.0f, height / 2.0f) ;
            const float centreX = x + width * 0.5f;
            const float centreY = y + height * 0.5f;
            const float rx = centreX - radius - 1.0f;
            const float ry = centreY - radius /* - 1.0f*/;
    
            g.drawImage( myStrip, // image
                         static_cast<int>(rx),
                         static_cast<int>(ry),
                         myStrip.getWidth(),
                         myStrip.getWidth(),   // dest
                         0,
                         frameIdx*myStrip.getWidth(),
                         myStrip.getWidth(),
                         myStrip.getWidth()); // source
    
        }
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CutoffKnobLookAndFeel);

};

class DepthKnobLookAndFeel : public LookAndFeel_V4
{
public:
    DepthKnobLookAndFeel(){}
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
        {
            Image myStrip;
            myStrip = ImageCache::getFromMemory (BinaryData::DEPTH_png, BinaryData::DEPTH_pngSize);
    
            //value between 0 and 1 for current amount of rotation
            const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
            const int frameIdx = static_cast<int>(ceil(sliderPos * (static_cast<double>(nFrames)-1.0)));// current index from 0 --> nFrames-1
    
            const float radius = jmin (width / 2.0f, height / 2.0f) ;
            const float centreX = x + width * 0.5f;
            const float centreY = y + height * 0.5f;
            const float rx = centreX - radius - 1.0f;
            const float ry = centreY - radius /* - 1.0f*/;
    
            g.drawImage( myStrip, // image
                         static_cast<int>(rx),
                         static_cast<int>(ry),
                         myStrip.getWidth(),
                         myStrip.getWidth(),   // dest
                         0,
                         frameIdx*myStrip.getWidth(),
                         myStrip.getWidth(),
                         myStrip.getWidth()); // source
    
        }
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DepthKnobLookAndFeel);

};

class FeedbackKnobLookAndFeel : public LookAndFeel_V4
{
public:
    FeedbackKnobLookAndFeel(){}
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
        {
            Image myStrip;
            myStrip = ImageCache::getFromMemory (BinaryData::FEEDBACK_png, BinaryData::FEEDBACK_pngSize);
    
            //value between 0 and 1 for current amount of rotation
            const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
            const int frameIdx = static_cast<int>(ceil(sliderPos * (static_cast<double>(nFrames)-1.0)));// current index from 0 --> nFrames-1
    
            const float radius = jmin (width / 2.0f, height / 2.0f) ;
            const float centreX = x + width * 0.5f;
            const float centreY = y + height * 0.5f;
            const float rx = centreX - radius - 1.0f;
            const float ry = centreY - radius /* - 1.0f*/;
    
            g.drawImage( myStrip, // image
                         static_cast<int>(rx),
                         static_cast<int>(ry),
                         myStrip.getWidth(),
                         myStrip.getWidth(),   // dest
                         0,
                         frameIdx*myStrip.getWidth(),
                         myStrip.getWidth(),
                         myStrip.getWidth()); // source
    
        }
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FeedbackKnobLookAndFeel);

};

class IntensityKnobLookAndFeel : public LookAndFeel_V4
{
public:
    IntensityKnobLookAndFeel(){}
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
        {
            Image myStrip;
            myStrip = ImageCache::getFromMemory (BinaryData::INTENSITY_png, BinaryData::INTENSITY_pngSize);
    
            //value between 0 and 1 for current amount of rotation
            const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
            const int frameIdx = static_cast<int>(ceil(sliderPos * (static_cast<double>(nFrames)-1.0)));// current index from 0 --> nFrames-1
    
            const float radius = jmin (width / 2.0f, height / 2.0f) ;
            const float centreX = x + width * 0.5f;
            const float centreY = y + height * 0.5f;
            const float rx = centreX - radius - 1.0f;
            const float ry = centreY - radius /* - 1.0f*/;
    
            g.drawImage( myStrip, // image
                         static_cast<int>(rx),
                         static_cast<int>(ry),
                         myStrip.getWidth(),
                         myStrip.getWidth(),   // dest
                         0,
                         frameIdx*myStrip.getWidth(),
                         myStrip.getWidth(),
                         myStrip.getWidth()); // source
    
        }
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntensityKnobLookAndFeel);

};

class SaturationKnobLookAndFeel : public LookAndFeel_V4
{
public:
    SaturationKnobLookAndFeel(){}
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
        {
            Image myStrip;
            myStrip = ImageCache::getFromMemory (BinaryData::SATURATION_png, BinaryData::SATURATION_pngSize);
    
            //value between 0 and 1 for current amount of rotation
            const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
            const int frameIdx = static_cast<int>(ceil(sliderPos * (static_cast<double>(nFrames)-1.0)));// current index from 0 --> nFrames-1
    
            const float radius = jmin (width / 2.0f, height / 2.0f) ;
            const float centreX = x + width * 0.5f;
            const float centreY = y + height * 0.5f;
            const float rx = centreX - radius - 1.0f;
            const float ry = centreY - radius /* - 1.0f*/;
    
            g.drawImage( myStrip, // image
                         static_cast<int>(rx),
                         static_cast<int>(ry),
                         myStrip.getWidth(),
                         myStrip.getWidth(),   // dest
                         0,
                         frameIdx*myStrip.getWidth(),
                         myStrip.getWidth(),
                         myStrip.getWidth()); // source
    
        }
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaturationKnobLookAndFeel);

};

class WobbleKnobLookAndFeel : public LookAndFeel_V4
{
public:
    WobbleKnobLookAndFeel(){}
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
        {
            Image myStrip;
            myStrip = ImageCache::getFromMemory (BinaryData::WOBBLE_png, BinaryData::WOBBLE_pngSize);
    
            //value between 0 and 1 for current amount of rotation
            const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
            const int frameIdx = static_cast<int>(ceil(sliderPos * (static_cast<double>(nFrames)-1.0)));// current index from 0 --> nFrames-1
    
            const float radius = jmin (width / 2.0f, height / 2.0f) ;
            const float centreX = x + width * 0.5f;
            const float centreY = y + height * 0.5f;
            const float rx = centreX - radius - 1.0f;
            const float ry = centreY - radius /* - 1.0f*/;
    
            g.drawImage( myStrip, // image
                         static_cast<int>(rx),
                         static_cast<int>(ry),
                         myStrip.getWidth(),
                         myStrip.getWidth(),   // dest
                         0,
                         frameIdx*myStrip.getWidth(),
                         myStrip.getWidth(),
                         myStrip.getWidth()); // source
    
        }
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WobbleKnobLookAndFeel);

};

