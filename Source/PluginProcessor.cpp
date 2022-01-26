/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PhaserXtremeAudioProcessor::PhaserXtremeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
parameters (*this,nullptr,"PARAMETERS",createParameters()), statePresets {*this, "Audio/Presets/SamSound/PhaseRing/presets.xml"}
#endif
{
    parameters.state = ValueTree("PhaseRingSavedParameters");
    parameters.state.setProperty("version", JucePlugin_VersionString, nullptr);
}

PhaserXtremeAudioProcessor::~PhaserXtremeAudioProcessor()
{
}

//==============================================================================
const String PhaserXtremeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PhaserXtremeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PhaserXtremeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PhaserXtremeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PhaserXtremeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PhaserXtremeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PhaserXtremeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PhaserXtremeAudioProcessor::setCurrentProgram (int index)
{
}

const String PhaserXtremeAudioProcessor::getProgramName (int index)
{
    return {};
}

void PhaserXtremeAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PhaserXtremeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    phaser[0].reset(sampleRate);
    phaser[1].reset(sampleRate);
    lastDepth = parameters.getRawParameterValue(DEPTH_ID)->load();
    lastBypass = parameters.getRawParameterValue(BYPASS_ID)->load();
}

void PhaserXtremeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PhaserXtremeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PhaserXtremeAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    
    //get num channels for mono/stereo mode
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // get playhead position from DAW
    AudioPlayHead* playHead = this->getPlayHead();
    AudioPlayHead::CurrentPositionInfo posInfo;
    if (playHead != nullptr)
        playHead->getCurrentPosition(posInfo);
    
    // load params from tree
    int mMode = parameters.getRawParameterValue(MODE_ID)->load(); //bpm, envelope, or freq mode
    double mBPM = parameters.getRawParameterValue(BPM_ID)->load(); //BPM slider
    bool mSync = parameters.getRawParameterValue(SYNC_ID)->load(); //tempo sync mode on/off
    if (mSync) //choose DAW or plugin BPM
    {
        if (playHead != nullptr)
            mBPM = posInfo.bpm;
        else
            mBPM = 120;
            
        dawBPM = mBPM;
    }
    
    float mInputGain = parameters.getRawParameterValue(IN_ID)->load(); //input gain
    mInputGain = Decibels::decibelsToGain(mInputGain); //convert from dB to gain
    float mOutputGain = parameters.getRawParameterValue(OUT_ID)->load(); //output gain
    mOutputGain = Decibels::decibelsToGain(mOutputGain);
    
    float mSaturation = parameters.getRawParameterValue(SAT_ID)->load(); //saturation between 0 and 2
    float mLfoSpeed = parameters.getRawParameterValue(LFO_SPEED_ID)->load(); //LFO speed either for phaser or ring
    float mCutoff = parameters.getRawParameterValue(CUTOFF_ID)->load(); //cutoff freq
    float mFeedback = parameters.getRawParameterValue(FEEDBACK_ID)->load(); //feedback between 0 and 1
    float mModDepth = parameters.getRawParameterValue(MOD_DEPTH_ID)->load(); //mod depth between 0 and 1
    int mWaveform = parameters.getRawParameterValue(MOD_TYPE_ID)->load(); //waveform shape for LFO
    bool mBypass = parameters.getRawParameterValue(BYPASS_ID)->load(); //bypass
    int mDepth = parameters.getRawParameterValue(DEPTH_ID)->load(); //number of apfs
    auto mMix = (parameters.getRawParameterValue(MIX_ID)->load()); //dry/wet mix
    auto mTempoSub = parameters.getRawParameterValue(RHYTHM_ID)->load(); //rhythm type
    auto mCutoffLFO = parameters.getRawParameterValue(LFO_CUTOFF_ID)->load(); //lfo for cutoff freq
    bool mQuadMode = parameters.getRawParameterValue(QUAD_MODE_ID)->load(); //stereo quadrature mode on/off
    float mAttack = 10.0 / parameters.getRawParameterValue(ATTACK_ID)->load(); //attack for envelope filter
    float mRelease = 10.0 / parameters.getRawParameterValue(RELEASE_ID)->load(); //release for envelope filter
    float mThresh = parameters.getRawParameterValue(THRESH_ID)->load(); //thresh for envelope filter
    
    phaserParams.cutoff_Freq = mCutoff;
    phaserParams.cutoff_lfoRate = mCutoffLFO;
    phaserParams.intensity_Pct = mFeedback;
    phaserParams.lfoRate_Hz = mLfoSpeed;
    phaserParams.lfoDepth_Pct = mModDepth;
    phaserParams.waveform = mWaveform;
    phaserParams.num_apfs = mDepth;
    phaserParams.mix = mMix;
    phaserParams.bpm = mBPM;
    phaserParams.sub = static_cast<subdivision>(mTempoSub);
    phaserParams.mode = static_cast<PhaserMode>(mMode);
    phaserParams.saturation = mSaturation;
    phaserParams.quadPhase = false;
    phaserParams.attack = mAttack;
    phaserParams.release = mRelease;
    phaserParams.threshold = mThresh;
    phaser[0].setParameters(phaserParams);
    if(mQuadMode)
        phaserParams.quadPhase = true;
    phaser[1].setParameters(phaserParams);
    
    if (mDepth != lastDepth || mBypass != lastBypass)
    {
        phaser[0].reset(getSampleRate());
        phaser[1].reset(getSampleRate());
        lastDepth = mDepth;
        lastBypass = mBypass;
    }

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
    
        float inputL = buffer.getReadPointer(0)[sample], outputL;
        inputL *= mInputGain;
        outputL = phaser[0].processAudioSample(inputL);
        outputL *= mOutputGain;
        if(!mBypass)
        {
            buffer.getWritePointer(0)[sample] = outputL;
        }
        
        if (totalNumInputChannels == 2) //if stereo
        {
            float inputR = buffer.getReadPointer(1)[sample], outputR;
            inputR *= mInputGain;
            outputR = phaser[1].processAudioSample(inputR);
            outputR *= mOutputGain;
            if(!mBypass)
            {
                buffer.getWritePointer(1)[sample] = outputR;
            }
        }
    }
}


//==============================================================================
bool PhaserXtremeAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* PhaserXtremeAudioProcessor::createEditor()
{
    return new PhaserXtremeAudioProcessorEditor (*this);
}

//==============================================================================
void PhaserXtremeAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml (parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PhaserXtremeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
 
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (parameters.state.getType()))
        {
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
        }
    }
}

AudioProcessorValueTreeState::ParameterLayout PhaserXtremeAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters; //creates a vector of parameters in order for the XML file
    
    // format: (ID, "String identifier", minval, maxval, defaultval)
    
    parameters.push_back(std::make_unique<AudioParameterFloat>(IN_ID,"Input Gain", -24.0f, 6.0f, 0.0f)); //In gain param
    parameters.push_back(std::make_unique<AudioParameterFloat>(OUT_ID,"Output Gain", -24.0f, 6.0f, 0.0f)); //Out gain param
    parameters.push_back(std::make_unique<AudioParameterFloat>(SAT_ID,"Grit", 0.0f, 1.0f, 0.0f)); //In gain param
    parameters.push_back(std::make_unique<AudioParameterFloat>(LFO_SPEED_ID,"Speed", 0.1f, 4000.0f, 4.0f)); //LFO Speed param
    parameters.push_back(std::make_unique<AudioParameterFloat>(FEEDBACK_ID,"Feedback", 0.3f, 0.9f, 0.5f)); //Feedback param
    parameters.push_back(std::make_unique<AudioParameterFloat>(CUTOFF_ID,"Cutoff", 4000.0f, 12000.0f, 6000.f)); //Cutoff param
    parameters.push_back(std::make_unique<AudioParameterFloat>(MOD_DEPTH_ID,"LFO Amplitude", 0.01f, 0.9f, 0.5f)); //Mod Depth param
    parameters.push_back(std::make_unique<AudioParameterInt>(MOD_TYPE_ID, "Mod Type",0,6,0)); //Mod type param
    parameters.push_back(std::make_unique<AudioParameterInt>(DEPTH_ID,"Depth",2,48,8)); //Depth
    parameters.push_back(std::make_unique<AudioParameterBool>(BYPASS_ID,"Bypass",false)); //Bypass
    parameters.push_back(std::make_unique<AudioParameterFloat>(MIX_ID, "Mix", 0.0f, 1.f, 0.5f)); //mix
    parameters.push_back(std::make_unique<AudioParameterInt>(RHYTHM_ID, "Rhythm",0,10,0)); //Mod type param
    parameters.push_back(std::make_unique<AudioParameterBool>(SYNC_ID,"DAW Sync",false)); //Tempo sync on/off
    parameters.push_back(std::make_unique<AudioParameterFloat>(LFO_CUTOFF_ID,"Wobble", 0.0f, 10.0f, 0.0f)); //LFO Speed param
    parameters.push_back(std::make_unique<AudioParameterFloat>(BPM_ID,"BPM",30.0f,300.0f,120.0f));
    parameters.push_back(std::make_unique<AudioParameterInt>(MODE_ID,"Mode",0,2,0)); //BPM vs Freq mode
    parameters.push_back(std::make_unique<AudioParameterBool>(QUAD_MODE_ID,"Stereo Mode",false)); //BPM vs Freq mode
    parameters.push_back(std::make_unique<AudioParameterFloat>(ATTACK_ID, "Attack", 0.01f, 1.f, 0.5f)); //Attack speed
    parameters.push_back(std::make_unique<AudioParameterFloat>(RELEASE_ID, "Release", 0.01f, 1.f, 0.5f)); //release speed
    parameters.push_back(std::make_unique<AudioParameterFloat>(THRESH_ID, "Threshold", 0.5f, 5.0f, 1.0f)); //Attack/release speed
    
    return { parameters.begin(), parameters.end() };
    
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PhaserXtremeAudioProcessor();
}
