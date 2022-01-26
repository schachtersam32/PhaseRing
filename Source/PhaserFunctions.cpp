// -----------------------------------------------------------------------------
//    ASPiK-Core File:  fxobjects.cpp
//
/**
    \file   fxobjects.cpp
    \author Will Pirkle
    \date   17-September-2018
    \brief  a collection of 54 objects and support structures, functions and
            enuemrations for all projects documented in:

            - Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
            - see the book for detailed explanations of theory and inner
              operations of obejcts
            - http://www.aspikplugins.com
            - http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#include <memory>
#include <math.h>
#include "PhaserFunctions.h"

/**
\brief returns the storage component S(n) for delay-free loop solutions

- NOTES:\n
the storageComponent or "S" value is used for Zavalishin's VA filters as well
as the phaser APFs (Biquad) and is only available on two of the forms: direct
and transposed canonical\n

\returns the storage component of the filter
*/
float Biquad::getS_value()
{
//    storageComponent = 0.0;
    if (parameters.biquadCalcType == biquadAlgorithm::kDirect)
    {
        // --- 1)  form output y(n) = a0*x(n) + a1*x(n-1) + a2*x(n-2) - b1*y(n-1) - b2*y(n-2)
        storageComponent = coeffArray[a1] * stateArray[x_z1] + coeffArray[a2] * stateArray[x_z2] - coeffArray[b1] * stateArray[y_z1] - coeffArray[b2] * stateArray[y_z2];
    }
    else if (parameters.biquadCalcType == biquadAlgorithm::kTransposeCanonical)
    {
        // --- 1)  form output y(n) = a0*x(n) + stateArray[x_z1]
        storageComponent = stateArray[x_z1];
    }

    return storageComponent;
}

/**
\brief process one sample through the biquad

- RULES:\n
1) do all math required to form the output y(n), reading registers as required - do NOT write registers \n
2) check for underflow, which can happen with feedback structures\n
3) lastly, update the states of the z^-1 registers in the state array just before returning\n

- NOTES:\n
the storageComponent or "S" value is used for Zavalishin's VA filters and is only
available on two of the forms: direct and transposed canonical\n

\param xn the input sample x(n)
\returns the biquad processed output y(n)
*/
float Biquad::processAudioSample(float xn)
{
    if (parameters.biquadCalcType == biquadAlgorithm::kDirect)
    {
        // --- 1)  form output y(n) = a0*x(n) + a1*x(n-1) + a2*x(n-2) - b1*y(n-1) - b2*y(n-2)
        float yn = coeffArray[a0] * xn + coeffArray[a1] * stateArray[x_z1] + coeffArray[a2] * stateArray[x_z2] - coeffArray[b1] * stateArray[y_z1] - coeffArray[b2] * stateArray[y_z2];

        // --- 2) underflow check
        checkFloatUnderflow(yn);

        // --- 3) update states
        stateArray[x_z2] = stateArray[x_z1];
        stateArray[x_z1] = xn;

        stateArray[y_z2] = stateArray[y_z1];
        stateArray[y_z1] = yn;

        // --- return value
        return coeffArray[d0] * xn + coeffArray[c0] * yn;
    }
    else if (parameters.biquadCalcType == biquadAlgorithm::kCanonical)
    {
        // --- 1)  form output y(n) = a0*w(n) + m_f_a1*stateArray[x_z1] + m_f_a2*stateArray[x_z2][x_z2];
        //
        // --- w(n) = x(n) - b1*stateArray[x_z1] - b2*stateArray[x_z2]
        float wn = xn - coeffArray[b1] * stateArray[x_z1] - coeffArray[b2] * stateArray[x_z2];

        // --- y(n):
        float yn = coeffArray[a0] * wn + coeffArray[a1] * stateArray[x_z1] + coeffArray[a2] * stateArray[x_z2];

        // --- 2) underflow check
        checkFloatUnderflow(yn);

        // --- 3) update states
        stateArray[x_z2] = stateArray[x_z1];
        stateArray[x_z1] = wn;

        // --- return value
        return coeffArray[d0] * xn + coeffArray[c0] * yn;
    }
    else if (parameters.biquadCalcType == biquadAlgorithm::kTransposeDirect)
    {
        // --- 1)  form output y(n) = a0*w(n) + stateArray[x_z1]
        //
        // --- w(n) = x(n) + stateArray[y_z1]
        float wn = xn + stateArray[y_z1];

        // --- y(n) = a0*w(n) + stateArray[x_z1]
        float yn = coeffArray[a0] * wn + stateArray[x_z1];

        // --- 2) underflow check
        checkFloatUnderflow(yn);

        // --- 3) update states
        stateArray[y_z1] = stateArray[y_z2] - coeffArray[b1] * wn;
        stateArray[y_z2] = -coeffArray[b2] * wn;

        stateArray[x_z1] = stateArray[x_z2] + coeffArray[a1] * wn;
        stateArray[x_z2] = coeffArray[a2] * wn;

        // --- return value
        return coeffArray[d0] * xn + coeffArray[c0] * yn;
    }
    else if (parameters.biquadCalcType == biquadAlgorithm::kTransposeCanonical)
    {
        // --- 1)  form output y(n) = a0*x(n) + stateArray[x_z1]
        float yn = coeffArray[a0] * xn + stateArray[x_z1];

        // --- 2) underflow check
        checkFloatUnderflow(yn);

        // --- shuffle/update
        stateArray[x_z1] = coeffArray[a1]*xn - coeffArray[b1]*yn + stateArray[x_z2];
        stateArray[x_z2] = coeffArray[a2]*xn - coeffArray[b2]*yn;

        // --- return value
        return coeffArray[d0] * xn + coeffArray[c0] * yn;
    }
    return xn; // didn't process anything :(
}

// --- returns true if coeffs were updated
void Biquad::calculateFilterCoeffs(float fc)
{
    // --- clear coeff array
//    memset(&coeffArray[0], 0, sizeof(float)*numCoeffs);
    clearCoeffArray();

    // --- set default pass-through
    coeffArray[a0] = 1.0;
    coeffArray[c0] = 1.0;
    coeffArray[d0] = 0.0;

    // --- grab these variables, to make calculations look more like the book
//    filterAlgorithm algorithm = parameters.algorithm;
//    float fc = parameters.fc;
    float Q = parameters.Q;
    
    float theta_c = 2.0*kPi*fc / static_cast<float>(sampleRate);
    float BW = fc / Q;
    float argTan = kPi*BW / static_cast<float>(sampleRate);
    if (argTan >= 0.95*kPi / 2.0) argTan = 0.95*kPi / 2.0;

    float alphaNumerator = tanf(argTan) - 1.0;
    float alphaDenominator = tanf(argTan) + 1.0;
    float alpha = alphaNumerator / alphaDenominator;
    float beta = -cosf(theta_c);

    // --- update coeffs
    coeffArray[a0] = -alpha;
    coeffArray[a1] = beta*(1.0 - alpha);
    coeffArray[a2] = 1.0;
    coeffArray[b1] = beta*(1.0 - alpha);
    coeffArray[b2] = -alpha;
}


SignalGenData LFO::renderAudioOutput()
{
    // --- always first!
    checkAndWrapModulo(modCounter, phaseInc);
    
    float frame = modCounter*numFrames;
    int frameInt = floor(frame);
    float frameFrac = frameInt - frame;

    // --- QP output always follows location of current modulo; first set equal
    modCounterQP = modCounter;

    // --- then, advance modulo by quadPhaseInc = 0.25 = 90 degrees, AND wrap if needed
    advanceAndCheckWrapModulo(modCounterQP, 0.25);
    
    float frameQP = modCounterQP*numFrames;
    int frameQPInt = floor(frameQP);
    float frameQPFrac = frameQPInt - frame;

    generatorWaveform waveform = lfoParameters.waveform;

    // --- calculate the oscillator value
    if (waveform == generatorWaveform::kSin)
    {
        // --- calculate normal angle
        float angle = modCounter*2.0*kPi - kPi;

        // --- norm output with parabolicSine approximation
        output.normalOutput = parabolicSine(-angle);

        // --- calculate QP angle
        angle = modCounterQP*2.0*kPi - kPi;

        // --- calc QP output
        output.quadPhaseOutput_pos = parabolicSine(-angle);
    }
    else if (waveform == generatorWaveform::kTriangle || waveform == generatorWaveform::kTrapezoid)
    {
        // triv saw
        output.normalOutput = unipolarToBipolar(modCounter);

        // bipolar triangle
        output.normalOutput = 2.0*fabs(output.normalOutput) - 1.0;
        
        if (waveform == generatorWaveform::kTrapezoid){ // create trapezoid
            output.normalOutput *= 2;
            boundValue(output.normalOutput, 0.0f, 1.0f);
        }

        // -- quad phase
        output.quadPhaseOutput_pos = unipolarToBipolar(modCounterQP);

        // bipolar triagle
        output.quadPhaseOutput_pos = 2.0*fabs(output.quadPhaseOutput_pos) - 1.0;
        
        if (waveform == generatorWaveform::kTrapezoid){
            output.quadPhaseOutput_pos *= 3;
            boundValue(output.quadPhaseOutput_pos, 0.0f, 1.0f);
        }
    }
    else if (waveform == generatorWaveform::kSaw || waveform == generatorWaveform::kReverseSaw)
    {

        output.normalOutput = doLinearInterpolation(sawTable[frameInt], sawTable[frameInt+1], frameFrac);
        output.quadPhaseOutput_pos = doLinearInterpolation(sawTable[frameQPInt], sawTable[frameQPInt+1], frameQPFrac);
        
        if(waveform == generatorWaveform::kReverseSaw)
        {
            output.normalOutput *= -0.9;
            output.quadPhaseOutput_pos *= -0.9;
        }
    }
    else if (waveform == generatorWaveform::kSquare)
    {
        float angle = modCounter*2.0*kPi - kPi;
        float sine = parabolicSine(angle);
        
        output.normalOutput = atanWaveShaper(sine, 200);
        
        angle = modCounterQP*2.0*kPi - kPi;
        sine = parabolicSine(angle);
        
        output.quadPhaseOutput_pos = atanWaveShaper(sine, 200);
    }
    else if (waveform == generatorWaveform::kRandom)
    {
        if(wrapped)
        {
            output.normalOutput = Random::getSystemRandom().nextFloat();
            wrapped = false;
        }
        if(wrappedQP)
        {
            output.quadPhaseOutput_pos = Random::getSystemRandom().nextFloat();
            wrappedQP = false;
        }
    }

    // --- invert two main outputs to make the opposite versions
    output.quadPhaseOutput_neg = -output.quadPhaseOutput_pos;
    output.invertedOutput = -output.normalOutput;

    // --- setup for next sample period
    advanceModulo(modCounter, phaseInc);

    return output;
}

void AudioDetector::setAttackTime(double attack_in_ms)
{

    audioDetectorParameters.attackTime_mSec = attack_in_ms;
    attackTime = expf(TLD_AUDIO_ENVELOPE_ANALOG_TC / (attack_in_ms * sampleRate * 0.001));
}

void AudioDetector::setReleaseTime(double release_in_ms)
{

    audioDetectorParameters.releaseTime_mSec = release_in_ms;
    releaseTime = expf(TLD_AUDIO_ENVELOPE_ANALOG_TC / (release_in_ms * sampleRate * 0.001));
}
