/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PhaserFunctions.h"
#include "State.h"

#define IN_ID "IN_GAIN"
#define OUT_ID "OUT_GAIN"
#define SAT_ID "SATURATION"
#define SAT_MODE_ID "SATURATION_MODE"
#define LFO_SPEED_ID "LFO_SPEED"
#define CUTOFF_ID "CUTOFF"
#define FEEDBACK_ID "FEEDBACK"
#define MOD_DEPTH_ID "MOD_DEPTH"
#define MOD_TYPE_ID "MOD_TYPE"
#define MIX_ID "MIX"
#define DEPTH_ID "DEPTH"
#define BYPASS_ID "BYPASS"
#define RHYTHM_ID "RHYTHM"
#define SYNC_ID "SYNC"
#define LFO_CUTOFF_ID "LFO_CUTOFF"
#define BPM_ID "BPM"
#define MODE_ID "MODE"
#define QUAD_MODE_ID "QUAD_MODE"
#define ATTACK_ID "ATTACK"
#define RELEASE_ID "RELEASE"
#define THRESH_ID "THRESHOLD"

//==============================================================================
/**
*/
class PhaserXtremeAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    PhaserXtremeAudioProcessor();
    ~PhaserXtremeAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
        
    //Parameter handling
    AudioProcessorValueTreeState parameters;
    AudioProcessorValueTreeState::ParameterLayout createParameters();
    StatePresets statePresets;
    
    //for use in editor
    double getDawBPM() {return dawBPM;}

private:
    
    Phaser phaser[2];
    PhaserParameters phaserParams = phaser[0].getParameters();
    double dawBPM;
    int lastDepth;
    bool lastBypass;

    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaserXtremeAudioProcessor)
};
