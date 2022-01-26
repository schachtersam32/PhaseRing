// -----------------------------------------------------------------------------
//    ASPiK-Core File:  fxobjects.h
//
/**
    \file   fxobjects.h
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
#pragma once

#include <memory>
#include <math.h>
#include <JuceHeader.h>
//#include "guiconstants.h"
#include <time.h>       /* time */

/** @file fxobjects.h
\brief HELLO LOOK
*/

// --- constants & enumerations
//
// ---  by placing outside the class declaration, these will also be avaialble to anything
//      that #includes this file (sometimes needed)
const float kSmallestPositiveFloatValue = 1.175494351e-38;         /* min positive value */
const float kSmallestNegativeFloatValue = -1.175494351e-38;         /* min negative value */
const float kSqrtTwo = pow(2.0, 0.5);
const float kMinFilterFrequency = 20.0;
const float kMaxFilterFrequency = 20480.0; // 10 octaves above 20 Hz
const float ARC4RANDOMMAX = 4294967295.0;  // (2^32 - 1)
const float kPi = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899;
const float kTwoPi = 2.0*3.14159265358979323846264338327950288419716939937510582097494459230781640628620899;
const int numFrames = 960;
const float sawTable[numFrames] = {0.000000, 0.002083, 0.004167, 0.006250, 0.008333, 0.010417, 0.012500, 0.014583, 0.016667, 0.018750, 0.020833, 0.022917, 0.025000, 0.027083, 0.029167, 0.031250, 0.033333, 0.035417, 0.037500, 0.039583, 0.041667, 0.043750, 0.045833, 0.047917, 0.050000, 0.052083, 0.054167, 0.056250, 0.058333, 0.060417, 0.062500, 0.064583, 0.066667, 0.068750, 0.070833, 0.072917, 0.075000, 0.077083, 0.079167, 0.081250, 0.083333, 0.085417, 0.087500, 0.089583, 0.091667, 0.093750, 0.095833, 0.097917, 0.100000, 0.102083, 0.104167, 0.106250, 0.108333, 0.110417, 0.112500, 0.114583, 0.116667, 0.118750, 0.120833, 0.122917, 0.125000, 0.127083, 0.129167, 0.131250, 0.133333, 0.135417, 0.137500, 0.139583, 0.141667, 0.143750, 0.145833, 0.147917, 0.150000, 0.152083, 0.154167, 0.156250, 0.158333, 0.160417, 0.162500, 0.164583, 0.166667, 0.168750, 0.170833, 0.172917, 0.175000, 0.177083, 0.179167, 0.181250, 0.183333, 0.185417, 0.187500, 0.189583, 0.191667, 0.193750, 0.195833, 0.197917, 0.200000, 0.202083, 0.204167, 0.206250, 0.208333, 0.210417, 0.212500, 0.214583, 0.216667, 0.218750, 0.220833, 0.222917, 0.225000, 0.227083, 0.229167, 0.231250, 0.233333, 0.235417, 0.237500, 0.239583, 0.241667, 0.243750, 0.245833, 0.247917, 0.250000, 0.252083, 0.254167, 0.256250, 0.258333, 0.260417, 0.262500, 0.264583, 0.266667, 0.268750, 0.270833, 0.272917, 0.275000, 0.277083, 0.279167, 0.281250, 0.283333, 0.285417, 0.287500, 0.289583, 0.291667, 0.293750, 0.295833, 0.297917, 0.300000, 0.302083, 0.304167, 0.306250, 0.308333, 0.310417, 0.312500, 0.314583, 0.316667, 0.318750, 0.320833, 0.322917, 0.325000, 0.327083, 0.329167, 0.331250, 0.333333, 0.335417, 0.337500, 0.339583, 0.341667, 0.343750, 0.345833, 0.347917, 0.350000, 0.352083, 0.354167, 0.356250, 0.358333, 0.360417, 0.362500, 0.364583, 0.366667, 0.368750, 0.370833, 0.372917, 0.375000, 0.377083, 0.379167, 0.381250, 0.383333, 0.385417, 0.387500, 0.389583, 0.391667, 0.393750, 0.395833, 0.397917, 0.400000, 0.402083, 0.404167, 0.406250, 0.408333, 0.410417, 0.412500, 0.414583, 0.416667, 0.418750, 0.420833, 0.422917, 0.425000, 0.427083, 0.429167, 0.431250, 0.433333, 0.435417, 0.437500, 0.439583, 0.441667, 0.443750, 0.445833, 0.447917, 0.450000, 0.452083, 0.454167, 0.456250, 0.458333, 0.460417, 0.462500, 0.464583, 0.466667, 0.468750, 0.470833, 0.472917, 0.475000, 0.477083, 0.479167, 0.481250, 0.483333, 0.485417, 0.487500, 0.489583, 0.491667, 0.493750, 0.495833, 0.497917, 0.500000, 0.502083, 0.504167, 0.506250, 0.508333, 0.510417, 0.512500, 0.514583, 0.516667, 0.518750, 0.520833, 0.522917, 0.525000, 0.527083, 0.529167, 0.531250, 0.533333, 0.535417, 0.537500, 0.539583, 0.541667, 0.543750, 0.545833, 0.547917, 0.550000, 0.552083, 0.554167, 0.556250, 0.558333, 0.560417, 0.562500, 0.564583, 0.566667, 0.568750, 0.570833, 0.572917, 0.575000, 0.577083, 0.579167, 0.581250, 0.583333, 0.585417, 0.587500, 0.589583, 0.591667, 0.593750, 0.595833, 0.597917, 0.600000, 0.602083, 0.604167, 0.606250, 0.608333, 0.610417, 0.612500, 0.614583, 0.616667, 0.618750, 0.620833, 0.622917, 0.625000, 0.627083, 0.629167, 0.631250, 0.633333, 0.635417, 0.637500, 0.639583, 0.641667, 0.643750, 0.645833, 0.647917, 0.650000, 0.652083, 0.654167, 0.656250, 0.658333, 0.660417, 0.662500, 0.664583, 0.666667, 0.668750, 0.670833, 0.672917, 0.675000, 0.677083, 0.679167, 0.681250, 0.683333, 0.685417, 0.687500, 0.689583, 0.691667, 0.693750, 0.695833, 0.697917, 0.700000, 0.702083, 0.704167, 0.706250, 0.708333, 0.710417, 0.712500, 0.714583, 0.716667, 0.718750, 0.720833, 0.722917, 0.725000, 0.727083, 0.729167, 0.731250, 0.733333, 0.735417, 0.737500, 0.739583, 0.741667, 0.743750, 0.745833, 0.747917, 0.750000, 0.752083, 0.754167, 0.756250, 0.758333, 0.760417, 0.762500, 0.764583, 0.766667, 0.768750, 0.770833, 0.772917, 0.775000, 0.777083, 0.778309, 0.779483, 0.780603, 0.781666, 0.782670, 0.783613, 0.784490, 0.785299, 0.786037, 0.786701, 0.787287, 0.787793, 0.788214, 0.788547, 0.788789, 0.788936, 0.788983, 0.788928, 0.788765, 0.788492, 0.788104, 0.787596, 0.786965, 0.786207, 0.785316, 0.784288, 0.783119, 0.781805, 0.780340, 0.778721, 0.776942, 0.775000, 0.772888, 0.770603, 0.768140, 0.765494, 0.762661, 0.759636, 0.756415, 0.752993, 0.749365, 0.745527, 0.741475, 0.737204, 0.732711, 0.727991, 0.723040, 0.717855, 0.712432, 0.706767, 0.700858, 0.694700, 0.688290, 0.681627, 0.674707, 0.667528, 0.660087, 0.652383, 0.644414, 0.636179, 0.627675, 0.618903, 0.609861, 0.600550, 0.590968, 0.581116, 0.570996, 0.560606, 0.549949, 0.539027, 0.527840, 0.516391, 0.504682, 0.492716, 0.480496, 0.468026, 0.455309, 0.442350, 0.429152, 0.415721, 0.402062, 0.388181, 0.374082, 0.359772, 0.345258, 0.330546, 0.315643, 0.300557, 0.285295, 0.269864, 0.254274, 0.238532, 0.222647, 0.206628, 0.190484, 0.174224, 0.157858, 0.141394, 0.124844, 0.108216, 0.091522, 0.074770, 0.057972, 0.041137, 0.024276, 0.007399, -0.009483, -0.026359, -0.043220, -0.060055, -0.076853, -0.093605, -0.110300, -0.126927, -0.143478, -0.159941, -0.176307, -0.192567, -0.208712, -0.224731, -0.240615, -0.256357, -0.271948, -0.287378, -0.302640, -0.317726, -0.332629, -0.347341, -0.361855, -0.376165, -0.390264, -0.404146, -0.417805, -0.431235, -0.444433, -0.457392, -0.470109, -0.482579, -0.494799, -0.506765, -0.518474, -0.529923, -0.541110, -0.552033, -0.562690, -0.573079, -0.583200, -0.593051, -0.602633, -0.611944, -0.620986, -0.629758, -0.638262, -0.646498, -0.654467, -0.662171, -0.669611, -0.676790, -0.683710, -0.690374, -0.696783, -0.702941, -0.708851, -0.714516, -0.719939, -0.725124, -0.730074, -0.734794, -0.739287, -0.743558, -0.747610, -0.751448, -0.755076, -0.758498, -0.761720, -0.764745, -0.767578, -0.770223, -0.772686, -0.774971, -0.777083, -0.779026, -0.780804, -0.782424, -0.783888, -0.785203, -0.786371, -0.787399, -0.788290, -0.789049, -0.789680, -0.790187, -0.790576, -0.790849, -0.791011, -0.791067, -0.791019, -0.790873, -0.790631, -0.790297, -0.789876, -0.789371, -0.788784, -0.788120, -0.787382, -0.786573, -0.785696, -0.784754, -0.783749, -0.782686, -0.781566, -0.780392, -0.779167, -0.777083, -0.775000, -0.772917, -0.770833, -0.768750, -0.766667, -0.764583, -0.762500, -0.760417, -0.758333, -0.756250, -0.754167, -0.752083, -0.750000, -0.747917, -0.745833, -0.743750, -0.741667, -0.739583, -0.737500, -0.735417, -0.733333, -0.731250, -0.729167, -0.727083, -0.725000, -0.722917, -0.720833, -0.718750, -0.716667, -0.714583, -0.712500, -0.710417, -0.708333, -0.706250, -0.704167, -0.702083, -0.700000, -0.697917, -0.695833, -0.693750, -0.691667, -0.689583, -0.687500, -0.685417, -0.683333, -0.681250, -0.679167, -0.677083, -0.675000, -0.672917, -0.670833, -0.668750, -0.666667, -0.664583, -0.662500, -0.660417, -0.658333, -0.656250, -0.654167, -0.652083, -0.650000, -0.647917, -0.645833, -0.643750, -0.641667, -0.639583, -0.637500, -0.635417, -0.633333, -0.631250, -0.629167, -0.627083, -0.625000, -0.622917, -0.620833, -0.618750, -0.616667, -0.614583, -0.612500, -0.610417, -0.608333, -0.606250, -0.604167, -0.602083, -0.600000, -0.597917, -0.595833, -0.593750, -0.591667, -0.589583, -0.587500, -0.585417, -0.583333, -0.581250, -0.579167, -0.577083, -0.575000, -0.572917, -0.570833, -0.568750, -0.566667, -0.564583, -0.562500, -0.560417, -0.558333, -0.556250, -0.554167, -0.552083, -0.550000, -0.547917, -0.545833, -0.543750, -0.541667, -0.539583, -0.537500, -0.535417, -0.533333, -0.531250, -0.529167, -0.527083, -0.525000, -0.522917, -0.520833, -0.518750, -0.516667, -0.514583, -0.512500, -0.510417, -0.508333, -0.506250, -0.504167, -0.502083, -0.500000, -0.497917, -0.495833, -0.493750, -0.491667, -0.489583, -0.487500, -0.485417, -0.483333, -0.481250, -0.479167, -0.477083, -0.475000, -0.472917, -0.470833, -0.468750, -0.466667, -0.464583, -0.462500, -0.460417, -0.458333, -0.456250, -0.454167, -0.452083, -0.450000, -0.447917, -0.445833, -0.443750, -0.441667, -0.439583, -0.437500, -0.435417, -0.433333, -0.431250, -0.429167, -0.427083, -0.425000, -0.422917, -0.420833, -0.418750, -0.416667, -0.414583, -0.412500, -0.410417, -0.408333, -0.406250, -0.404167, -0.402083, -0.400000, -0.397917, -0.395833, -0.393750, -0.391667, -0.389583, -0.387500, -0.385417, -0.383333, -0.381250, -0.379167, -0.377083, -0.375000, -0.372917, -0.370833, -0.368750, -0.366667, -0.364583, -0.362500, -0.360417, -0.358333, -0.356250, -0.354167, -0.352083, -0.350000, -0.347917, -0.345833, -0.343750, -0.341667, -0.339583, -0.337500, -0.335417, -0.333333, -0.331250, -0.329167, -0.327083, -0.325000, -0.322917, -0.320833, -0.318750, -0.316667, -0.314583, -0.312500, -0.310417, -0.308333, -0.306250, -0.304167, -0.302083, -0.300000, -0.297917, -0.295833, -0.293750, -0.291667, -0.289583, -0.287500, -0.285417, -0.283333, -0.281250, -0.279167, -0.277083, -0.275000, -0.272917, -0.270833, -0.268750, -0.266667, -0.264583, -0.262500, -0.260417, -0.258333, -0.256250, -0.254167, -0.252083, -0.250000, -0.247917, -0.245833, -0.243750, -0.241667, -0.239583, -0.237500, -0.235417, -0.233333, -0.231250, -0.229167, -0.227083, -0.225000, -0.222917, -0.220833, -0.218750, -0.216667, -0.214583, -0.212500, -0.210417, -0.208333, -0.206250, -0.204167, -0.202083, -0.200000, -0.197917, -0.195833, -0.193750, -0.191667, -0.189583, -0.187500, -0.185417, -0.183333, -0.181250, -0.179167, -0.177083, -0.175000, -0.172917, -0.170833, -0.168750, -0.166667, -0.164583, -0.162500, -0.160417, -0.158333, -0.156250, -0.154167, -0.152083, -0.150000, -0.147917, -0.145833, -0.143750, -0.141667, -0.139583, -0.137500, -0.135417, -0.133333, -0.131250, -0.129167, -0.127083, -0.125000, -0.122917, -0.120833, -0.118750, -0.116667, -0.114583, -0.112500, -0.110417, -0.108333, -0.106250, -0.104167, -0.102083, -0.100000, -0.097917, -0.095833, -0.093750, -0.091667, -0.089583, -0.087500, -0.085417, -0.083333, -0.081250, -0.079167, -0.077083, -0.075000, -0.072917, -0.070833, -0.068750, -0.066667, -0.064583, -0.062500, -0.060417, -0.058333, -0.056250, -0.054167, -0.052083, -0.050000, -0.047917, -0.045833, -0.043750, -0.041667, -0.039583, -0.037500, -0.035417, -0.033333, -0.031250, -0.029167, -0.027083, -0.025000, -0.022917, -0.020833, -0.018750, -0.016667, -0.014583, -0.012500, -0.010417, -0.008333, -0.006250, -0.004167, -0.002083};


#define NEGATIVE       0
#define POSITIVE       1

// ------------------------------------------------------------------ //
// --- FUNCTIONS ---------------------------------------------------- //
// ------------------------------------------------------------------ //

/**
@checkFloatUnderflow
\ingroup FX-Functions

@brief Perform underflow check; returns true if we did underflow (user may not care)

\param value - the value to check for underflow
\return true if overflowed, false otherwise
*/
inline bool checkFloatUnderflow(float& value)
{
    bool retValue = false;
    if (value > 0.0 && value < kSmallestPositiveFloatValue)
    {
        value = 0.0;
        retValue = true;
    }
    else if (value < 0.0 && value > kSmallestNegativeFloatValue)
    {
        value = 0.0;
        retValue = true;
    }
    return retValue;
}

/**
@doLinearInterpolation
\ingroup FX-Functions

@brief performs linear interpolation of x distance between two (x,y) points;
returns interpolated value

\param x1 - the x coordinate of the first point
\param x2 - the x coordinate of the second point
\param y1 - the y coordinate of the first point
\param y2 - the 2 coordinate of the second point
\param x - the interpolation location
\return the interpolated value or y1 if the x coordinates are unusable
*/
inline float doLinearInterpolation(float x1, float x2, float y1, float y2, float x)
{
    float denom = x2 - x1;
    if (denom == 0)
        return y1; // --- should not ever happen

    // --- calculate decimal position of x
    float dx = (x - x1) / (x2 - x1);

    // --- use weighted sum method of interpolating
    return dx*y2 + (1 - dx)*y1;
}

/**
@doLinearInterpolation
\ingroup FX-Functions

@brief performs linear interpolation of fractional x distance between two adjacent (x,y) points;
returns interpolated value

\param y1 - the y coordinate of the first point
\param y2 - the 2 coordinate of the second point
\param x - the interpolation location as a fractional distance between x1 and x2 (which are not needed)
\return the interpolated value or y2 if the interpolation is outside the x interval
*/
inline float doLinearInterpolation(float y1, float y2, float fractional_X)
{
    // --- check invalid condition
    if (fractional_X >= 1.0) return y2;

    // --- use weighted sum method of interpolating
    return fractional_X*y2 + (1.0 - fractional_X)*y1;
}

/**
@doLagrangeInterpolation
\ingroup FX-Functions

@brief implements n-order Lagrange Interpolation

\param x - Pointer to an array containing the x-coordinates of the input values
\param y - Pointer to an array containing the y-coordinates of the input values
\param n - the order of the interpolator, this is also the length of the x,y input arrays
\param xbar - The x-coorinates whose y-value we want to interpolate
\return the interpolated value
*/
inline float doLagrangeInterpolation(float* x, float* y, int n, float xbar)
{
    int i, j;
    float fx = 0.0;
    float l = 1.0;
    for (i = 0; i<n; i++)
    {
        l = 1.0;
        for (j = 0; j<n; j++)
        {
            if (j != i)
                l *= (xbar - x[j]) / (x[i] - x[j]);
        }
        fx += l*y[i];
    }
    return (fx);
}


/**
@boundValue
\ingroup FX-Functions

@brief  Bound a value to min and max limits

\param value - value to bound
\param minValue - lower bound limit
\param maxValue - upper bound limit
*/
inline void boundValue(float& value, float minValue, float maxValue)
{
    value = fmin(value, maxValue);
    value = fmax(value, minValue);
}

inline void smoothStep(float edge0, float edge1, float &x)
{
    x = (x - edge0)/(edge1 - edge0);
    boundValue(x, 0.0, 1.0);
    x = x*x*(3-2*x);
}

/**
@doUnipolarModulationFromMin
\ingroup FX-Functions

@brief Perform unipolar modulation from a min value up to a max value using a unipolar modulator value

\param unipolarModulatorValue - modulation value on range [0.0, +1.0]
\param minValue - lower modulation limit
\param maxValue - upper modulation limit
\return the modulated value
*/
inline float doUnipolarModulationFromMin(float unipolarModulatorValue, float minValue, float maxValue)
{
    // --- UNIPOLAR bound
    boundValue(unipolarModulatorValue, 0.0, 1.0);

    // --- modulate from minimum value upwards
    return unipolarModulatorValue*(maxValue - minValue) + minValue;
}

/**
@doUnipolarModulationFromMax
\ingroup FX-Functions

@brief Perform unipolar modulation from a max value down to a min value using a unipolar modulator value

\param unipolarModulatorValue - modulation value on range [0.0, +1.0]
\param minValue - lower modulation limit
\param maxValue - upper modulation limit
\return the modulated value
*/
inline float doUnipolarModulationFromMax(float unipolarModulatorValue, float minValue, float maxValue)
{
    // --- UNIPOLAR bound
    boundValue(unipolarModulatorValue, 0.0, 1.0);
//    smoothStep(0.0, 1.0, unipolarModulatorValue);

    // --- modulate from maximum value downwards
    return maxValue - (1.0 - unipolarModulatorValue)*(maxValue - minValue);
}

/**
@doBipolarModulation
\ingroup FX-Functions

@brief Perform bipolar modulation about a center that his halfway between the min and max values

\param bipolarModulatorValue - modulation value on range [-1.0, +1.0]
\param minValue - lower modulation limit
\param maxValue - upper modulation limit
\return the modulated value
*/
inline float doBipolarModulation(float bipolarModulatorValue, float minValue, float maxValue)
{
    // --- BIPOLAR bound
    boundValue(bipolarModulatorValue, -1.0, 1.0);
//    smoothStep(-1.0, 1.0, bipolarModulatorValue);

    // --- calculate range and midpoint
    float halfRange = (maxValue - minValue) / 2.0;
    float midpoint = halfRange + minValue;

    return bipolarModulatorValue*(halfRange) + midpoint;
}

/**
@unipolarToBipolar
\ingroup FX-Functions

@brief calculates the bipolar [-1.0, +1.0] value FROM a unipolar [0.0, +1.0] value

\param value - value to convert
\return the bipolar value
*/
inline float unipolarToBipolar(float value)
{
    return 2.0*value - 1.0;
}

/**
@bipolarToUnipolar
\ingroup FX-Functions

@brief calculates the unipolar [0.0, +1.0] value FROM a bipolar [-1.0, +1.0] value

\param value - value to convert
\return the unipolar value
*/
inline float bipolarToUnipolar(float value)
{
    return 0.5*value + 0.5;
}

/**
@rawTo_dB
\ingroup FX-Functions

@brief calculates dB for given input

\param raw - value to convert to dB
\return the dB value
*/
inline float raw2dB(float raw)
{
    return 20.0*log10(raw);
}

/**
@dBTo_Raw
\ingroup FX-Functions

@brief converts dB to raw value

\param dB - value to convert to raw
\return the raw value
*/
inline float dB2Raw(float dB)
{
    return pow(10.0, (dB / 20.0));
}

/**
@peakGainFor_Q
\ingroup FX-Functions

@brief calculates the peak magnitude for a given Q

\param Q - the Q value
\return the peak gain (not in dB)
*/
inline float peakGainFor_Q(float Q)
{
    // --- no resonance at or below unity
    if (Q <= 0.707) return 1.0;
    return (Q*Q) / (pow((Q*Q - 0.25), 0.5));
}

/**
@dBPeakGainFor_Q
\ingroup FX-Functions

@brief calculates the peak magnitude in dB for a given Q

\param Q - the Q value
\return the peak gain in dB
*/
inline float dBPeakGainFor_Q(float Q)
{
    return raw2dB(peakGainFor_Q(Q));
}

/**
@doWhiteNoise
\ingroup FX-Functions

@brief calculates a random value between -1.0 and +1.0
\return the random value on the range [-1.0, +1.0]
*/
inline float doWhiteNoise()
{
    float noise = 0.0;

#if defined _WINDOWS || defined _WINDLL
    // fNoise is 0 -> 32767.0
    noise = (float)rand();

    // normalize and make bipolar
    noise = 2.0*(noise / 32767.0) - 1.0;
#else
    // fNoise is 0 -> ARC4RANDOMMAX
    noise = (float)arc4random();

    // normalize and make bipolar
    noise = 2.0*(noise / ARC4RANDOMMAX) - 1.0;
#endif

    return noise;
}

/**
@sgn
\ingroup FX-Functions

@brief calculates sgn( ) of input
\param xn - the input value
\return -1 if xn is negative or +1 if xn is 0 or greater
*/
inline float sgn(float xn)
{
    return (xn > 0) - (xn < 0);
}

/**
@calcWSGain
\ingroup FX-Functions

@brief calculates gain of a waveshaper
\param xn - the input value
\param saturation  - the saturation control
\param asymmetry  - the degree of asymmetry
\return gain value
*/
inline float calcWSGain(float xn, float saturation, float asymmetry)
{
    float g = ((xn >= 0.0 && asymmetry > 0.0) || (xn < 0.0 && asymmetry < 0.0)) ? saturation * (1.0 + 4.0*fabs(asymmetry)) : saturation;
    return g;
}

/**
@atanWaveShaper
\ingroup FX-Functions

@brief calculates arctangent waveshaper
\param xn - the input value
\param saturation  - the saturation control
\return the waveshaped output value
*/
inline float atanWaveShaper(float xn, float saturation)
{
    if (saturation < 0.0001)
        saturation = 0.0001;
    return atan(saturation*xn) / atan(saturation);
}

/**
@tanhWaveShaper
\ingroup FX-Functions

@brief calculates hyptan waveshaper
\param xn - the input value
\param saturation  - the saturation control
\return the waveshaped output value
*/
inline float tanhWaveShaper(float xn, float saturation)
{
    if (saturation < 0.0001)
    saturation = 0.0001;
    return tanh(saturation*xn) / tanh(saturation);
}

/**
@softClipWaveShaper
\ingroup FX-Functions

@brief calculates hyptan waveshaper
\param xn - the input value
\param saturation  - the saturation control
\return the waveshaped output value
*/
inline float softClipWaveShaper(float xn, float saturation)
{
    if (saturation < 0.0001)
    saturation = 0.0001;
    // --- normalized soft clipper from Reiss book
    return sgn(xn)*(1.0 - exp(-fabs(saturation*xn)))/(1.0 - exp(-fabs(saturation)));
}

////**
///@fuzzExp1WaveShaper
///\ingroup FX-Functions
///
///@brief calculates fuzz exp1 waveshaper
///\param xn - the input value
///\param saturation  - the saturation control
///\return the waveshaped output value
///*/
inline float fuzzExp1WaveShaper(float xn, float saturation, float asymmetry)
{
    // --- setup gain
    float wsGain = calcWSGain(xn, saturation, asymmetry);
    float yn = xn;
    if (wsGain > 0)
        yn = sgn(xn)*(1.0 - exp(-fabs(wsGain*xn))) / (1.0 - exp(-wsGain));
    return yn;
}

inline float arrayaWaveShaper(float xn)
{
    return xn*(3.0 / 2.0)*(1.0 - (xn*xn) / 3.0);
}

inline float tubeWaveShaper(float xn, float saturation)
{
    float yn = xn;
    xn = xn*(1+saturation);
//    if (xn > 0.0)
//        yn = arrayaWaveShaper(xn);
//    else
//        yn = tanhWaveShaper(xn, saturation);
    
    if (xn >= -1 && xn < -0.08905)
        yn = -0.75*(1 - powf(1 - (fabs(xn) - 0.032847),12.0)+ 0.3333*(fabs(xn) - 0.032847)) + 0.01;
    else if (xn >= -0.08905 && xn < 0.320018)
        yn = (xn*xn)* -6.153 + 3.9375*xn;
    else if (xn >= 0.320018 && xn < 1)
        yn = 0.630035;
    
    return yn;
}

inline float movingAverage(float xn, float yn_1, float alpha)
{
    return alpha*xn + (1 - alpha) * yn_1;
}

// ------------------------------------------------------------------ //
// --- INTERFACES --------------------------------------------------- //
// ------------------------------------------------------------------ //

/**
\struct SignalGenData
\ingroup Structures
\brief
This is the output structure for audio generator objects that can render up to four outputs.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
// --- structure to send output data from signal gen; you can add more outputs here
struct SignalGenData
{
    SignalGenData() {}

    float normalOutput = 0.0;            ///< normal
    float invertedOutput = 0.0;        ///< inverted
    float quadPhaseOutput_pos = 0.0;    ///< 90 degrees out
    float quadPhaseOutput_neg = 0.0;    ///< -90 degrees out
};

// ------------------------------------------------------------------ //
// --- OBJECTS ------------------------------------------------------ //
// ------------------------------------------------------------------ //
/*
Class Declarations :

class name : public IAudioSignalProcessor
    - IAudioSignalProcessor functions
    - member functions that may be called externally
    - mutators & accessors
    - helper functions(may be private / protected if needed)
    - protected member functions
*/

/**
\enum filterCoeff
\ingroup Constants-Enums
\brief
Use this enum to easily access coefficents inside of arrays.

- enum filterCoeff { a0, a1, a2, b1, b2, c0, d0, numCoeffs };

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
enum filterCoeff { a0, a1, a2, b1, b2, c0, d0, numCoeffs };

/**
\enum stateReg
\ingroup Constants-Enums
\brief
Use this enum to easily access z^-1 state values inside of arrays. For some structures, fewer storage units are needed. They are divided as follows:

- Direct Forms: we will allow max of 2 for X (feedforward) and 2 for Y (feedback) data
- Transpose Forms: we will use ONLY the x_z1 and x_z2 registers for the 2 required delays
- enum stateReg { x_z1, x_z2, y_z1, y_z2, numStates };

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/

// --- state array index values
//     z^-1 registers;
//        Direct Forms: we will allow max of 2 for X (feedforward) and 2 for Y (feedback) data
//        Transpose Forms: we will use ONLY the x_z1 and x_z2 registers for the 2 required delays
enum stateReg { x_z1, x_z2, y_z1, y_z2, numStates };

/**
\enum biquadAlgorithm
\ingroup Constants-Enums
\brief
Use this strongly typed enum to easily set the biquad calculation type

- enum class biquadAlgorithm { kDirect, kCanonical, kTransposeDirect, kTransposeCanonical }; //  4 types of biquad calculations, constants (k)

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/

// --- type of calculation (algorithm)
enum class biquadAlgorithm { kDirect, kCanonical, kTransposeDirect, kTransposeCanonical }; //  4 types of biquad calculations, constants (k)
enum class filterAlgorithm {kAPF1, kAPF2, kLPF1, kHPF1, kLPF2, kHPF2}; // --- you will add more here...

/**
\struct BiquadParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the Biquad object. Default version defines the biquad structure used in the calculation.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
struct BiquadParameters
{
    BiquadParameters () {}

    /** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
    BiquadParameters& operator=(const BiquadParameters& params)
    {
        if (this == &params)
            return *this;

        biquadCalcType = params.biquadCalcType;
        algorithm = params.algorithm;
        fc = params.fc;
        Q = params.Q;
        return *this;
    }

    biquadAlgorithm biquadCalcType = biquadAlgorithm::kDirect; ///< biquad structure to use
    filterAlgorithm algorithm = filterAlgorithm::kAPF2; ///< filter algorithm
    double fc = 100.0; ///< filter cutoff or center frequency (Hz)
    double Q = 0.707; ///< filter Q
};

/**
\class Biquad
\ingroup FX-Objects
\brief
The Biquad object implements a first or second order H(z) transfer function using one of four standard structures: Direct, Canonical, Transpose Direct, Transpose Canonical.

Audio I/O:
- Processes mono input to mono output.

Control I/F:
- Use BiquadParameters structure to get/set object params.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class Biquad
{
public:
    Biquad() {sampleRate = 44100.0;}        /* C-TOR */
    ~Biquad() {}    /* D-TOR */
    
        // --- IAudioSignalProcessor FUNCTIONS --- //
    //
    /** reset: clear out the state array (flush delays); can safely ignore sampleRate argument - we don't need/use it */
    bool reset(double _sampleRate)
    {
//        memset(&stateArray[0], 0, sizeof(float)*numStates);
        FloatVectorOperations::clear(stateArray, numStates);
        sampleRate = _sampleRate;
//        calculateFilterCoeffs(parameters.fc);
        storageComponent = 0.0;
        return true;  // handled = true
    }

    /** return false: this object only processes samples */
    bool canProcessAudioFrame() { return false; }

    /** process input x(n) through biquad to produce return value y(n) */
    /**
    \param xn input
    \return the processed sample
    */
    float processAudioSample(float xn);

    /** get parameters: note use of custom structure for passing param data */
    /**
    \return BiquadParameters custom data structure
    */
    BiquadParameters getParameters() { return parameters; }

    /** set parameters: note use of custom structure for passing param data */
    /**
    \param BiquadParameters custom data structure
    */
    void setParameters(const BiquadParameters& _parameters, bool reCalcCoeffs)
    {
        if (parameters.algorithm != _parameters.algorithm || parameters.fc != _parameters.fc || parameters.Q != _parameters.Q || parameters.biquadCalcType != _parameters.biquadCalcType)
        {
            // --- save new params
            parameters = _parameters;
        }
        else
            return;

        // --- don't allow 0 or (-) values for Q
        if (parameters.Q <= 0)
            parameters.Q = 0.707;

        // --- update coeffs
        if (reCalcCoeffs)
            calculateFilterCoeffs(parameters.fc);
    }

    // --- MUTATORS & ACCESSORS --- //
    /** set the coefficient array NOTE: passing by pointer to array; allows us to use "array notation" with pointers i.e. [ ] */
    void setCoefficients(float* coeffs){
        // --- fast block memory copy:
        memcpy(&coeffArray[0], &coeffs[0], sizeof(float)*numCoeffs);
//        FloatVectorOperations::copy(&coeffArray[0],&coeffs,sizeof(float)*numCoeffs);
    }

    /** get the coefficient array for read/write access to the array (not used in current objects) */
    float* getCoefficients()
    {
        return &coeffArray[0];
    }

    /** get the state array for read/write access to the array (used only in direct form oscillator) */
    float* getStateArray()
    {
        return &stateArray[0];
    }

    /** get the structure G (gain) value for Harma filters; see 2nd Ed FX book */
    float getG_value() { return coeffArray[a0]; }

    /** get the structure S (storage) value for Harma filters; see 2nd Ed FX book */
    float getS_value();// { return storageComponent; }
    
    void clearCoeffArray()
    {
        FloatVectorOperations::clear(coeffArray, numCoeffs);
    }

protected:
    
    void calculateFilterCoeffs(float fc);
    
    double sampleRate;
    /** array of coefficients */
    float coeffArray[numCoeffs] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

    /** array of state (z^-1) registers */
    float stateArray[numStates] = { 0.0, 0.0, 0.0, 0.0 };

    /** type of calculation (algorithm  structure) */
    BiquadParameters parameters;

    /** for Harma loop resolution */
    float storageComponent = 0.0;
};

/**
\enum generatorWaveform
\ingroup Constants-Enums
\brief
Use this strongly typed enum to easily set the oscillator waveform

- enum  generatorWaveform { kTriangle, kSin, kSaw };

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
enum class generatorWaveform { kTriangle, kSin, kSaw, kReverseSaw, kSquare, kTrapezoid, kRandom};

/**
\struct OscillatorParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the LFO and DFOscillator objects.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
struct OscillatorParameters
{
    OscillatorParameters() {}
    /** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
    OscillatorParameters& operator=(const OscillatorParameters& params)    // need this override for collections to work
    {
        if (this == &params)
            return *this;

        waveform = params.waveform;
        frequency_Hz = params.frequency_Hz;
        return *this;
    }

    // --- individual parameters
    generatorWaveform waveform = generatorWaveform::kTriangle; ///< the current waveform
    float frequency_Hz = 0.0;    ///< oscillator frequency
};

/**
\class LFO
\ingroup FX-Objects
\brief
The LFO object implements a mathematically perfect LFO generator for modulation uses only. It should not be used for
audio frequencies except for the sinusoidal output which, though an approximation, has very low TDH.

Audio I/O:
- Output only object: low frequency generator.

Control I/F:
- Use OscillatorParameters structure to get/set object params.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class LFO
{
public:
    LFO() {}    /* C-TOR */
    ~LFO() {}                /* D-TOR */

    /** reset members to initialized state */
    bool reset(double _sampleRate)
    {
        sampleRate = _sampleRate;
        phaseInc = lfoParameters.frequency_Hz / static_cast<float>(sampleRate);

        // --- timebase variables
        modCounter = 0.0;            ///< modulo counter [0.0, +1.0]
        modCounterQP = 0.25;        ///<Quad Phase modulo counter [0.0, +1.0]

        return true;
    }

    /** get parameters: note use of custom structure for passing param data */
    /**
    \return OscillatorParameters custom data structure
    */
    OscillatorParameters getParameters(){ return lfoParameters; }

    /** set parameters: note use of custom structure for passing param data */
    /**
    \param OscillatorParameters custom data structure
    */
    void setParameters(const OscillatorParameters& params)
    {
        if(params.frequency_Hz != lfoParameters.frequency_Hz)
            // --- update phase inc based on osc freq and fs
            phaseInc = params.frequency_Hz / static_cast<float>(sampleRate);

        lfoParameters = params;
    }

    /** render a new audio output structure */
    SignalGenData renderAudioOutput();

protected:
    // --- parameters
    OscillatorParameters lfoParameters; ///< obejcgt parameters
    SignalGenData output;

    // --- sample rate
    double sampleRate = 0.0;            ///< sample rate

    // --- timebase variables
    float modCounter = 0.0;            ///< modulo counter [0.0, +1.0]
    float phaseInc = 0.0;                ///< phase inc = fo/fs
    float modCounterQP = 0.25;            ///<Quad Phase modulo counter [0.0, +1.0]
    bool wrapped = false, wrappedQP = false; ///For random waveform

    /** check the modulo counter and wrap if needed */
    inline bool checkAndWrapModulo(float& moduloCounter, float phaseInc)
    {
        // --- for positive frequencies
        if (phaseInc > 0 && moduloCounter >= 1.0)
        {
            moduloCounter -= 1.0;
            wrapped = true;
            return true;
        }

        // --- for negative frequencies
        if (phaseInc < 0 && moduloCounter <= 0.0)
        {
            moduloCounter += 1.0;
            wrapped = true;
            return true;
        }
        
        return false;
    }

    /** advanvce the modulo counter, then check the modulo counter and wrap if needed */
    inline bool advanceAndCheckWrapModulo(float& moduloCounter, float phaseInc)
    {
        // --- advance counter
        moduloCounter += phaseInc;

        // --- for positive frequencies
        if (phaseInc > 0 && moduloCounter >= 1.0)
        {
            moduloCounter -= 1.0;
            wrappedQP = false;
            return true;
        }

        // --- for negative frequencies
        if (phaseInc < 0 && moduloCounter <= 0.0)
        {
            moduloCounter += 1.0;
            wrappedQP = false;
            return true;
        }

        return false;
    }

    /** advanvce the modulo counter */
    inline void advanceModulo(float& moduloCounter, float phaseInc) { moduloCounter += phaseInc; }

    const float B = 4.0 / kPi;
    const float C = -4.0 / (kPi* kPi);
    const float P = 0.225;
    /** parabolic sinusoidal calcualtion; NOTE: input is -pi to +pi http://devmaster.net/posts/9648/fast-and-accurate-sine-cosine */
    inline float parabolicSine(float angle)
    {
        float y = B * angle + C * angle * fabs(angle);
        y = P * (y * fabs(y) - y) + y;
        return y;
    }
};

//Audio Detector
const double TLD_AUDIO_ENVELOPE_ANALOG_TC = -0.99967234081320612357829304641019; // ln(36.7%)

struct AudioDetectorParameters
{
    AudioDetectorParameters() {}
    /** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
    AudioDetectorParameters& operator=(const AudioDetectorParameters& params)    // need this override for collections to work
    {
        if (this == &params)
            return *this;
        attackTime_mSec = params.attackTime_mSec;
        releaseTime_mSec = params.releaseTime_mSec;
        clampToUnityMax = params.clampToUnityMax;
        return *this;
    }

    // --- individual parameters
    double attackTime_mSec = 0.0; ///< attack time in milliseconds
    double releaseTime_mSec = 0.0;///< release time in milliseconds
    bool clampToUnityMax = true;///< clamp output to 1.0 (set false for true log detectors)
};

class AudioDetector
{
public:
    AudioDetector() {}    /* C-TOR */
    ~AudioDetector() {}    /* D-TOR */

public:
    /** set sample rate dependent time constants and clear last envelope output value */
    bool reset(double _sampleRate)
    {
        setSampleRate(_sampleRate);
        lastEnvelope = 0.0;
        return true;
    }
    
    float processAudioSample(float xn)
    {
        float input = fabs(xn*xn); //rectify input
        
        float currEnv = 0.0; // store current env
        
        if (input > lastEnvelope) //either attack or release based on previous env
            currEnv = (1.0 - attackTime)*input + attackTime*lastEnvelope;
        else
            currEnv = (1.0 - releaseTime)*input + releaseTime*lastEnvelope;
        
        checkFloatUnderflow(currEnv); //check underflow since feedback structure
        
        // --- store envelope prior to sqrt for RMS version
        lastEnvelope = currEnv;
        
        // --- if RMS, do the SQRT
        currEnv = pow(currEnv, 0.5);
        
        return currEnv;
    }
    
    /** get parameters: note use of custom structure for passing param data */
    /**
    \return AudioDetectorParameters custom data structure
    */
    AudioDetectorParameters getParameters()
    {
        return audioDetectorParameters;
    }

    /** set parameters: note use of custom structure for passing param data */
    /**
    \param AudioDetectorParameters custom data structure
    */
    void setParameters(const AudioDetectorParameters& parameters)
    {
        audioDetectorParameters = parameters;

        // --- update structure
        setAttackTime(audioDetectorParameters.attackTime_mSec);
        setReleaseTime(audioDetectorParameters.releaseTime_mSec);

    }

    /** set sample rate - our time constants depend on it */
    void setSampleRate(double _sampleRate)
    {
        if (sampleRate == _sampleRate)
            return;

        sampleRate = _sampleRate;

        // --- recalculate RC time-constants
        setAttackTime(audioDetectorParameters.attackTime_mSec);
        setReleaseTime(audioDetectorParameters.releaseTime_mSec);
    }

protected:
    AudioDetectorParameters audioDetectorParameters; ///< parameters for object
    double attackTime = 0.0;    ///< attack time coefficient
    double releaseTime = 0.0;    ///< release time coefficient
    float sampleRate = 0.0;    ///< stored sample rate
    double lastEnvelope = 0.0;    ///< output register

    /** set our internal atack time coefficients based on times and sample rate */
    void setAttackTime(double attack_in_ms);

    /** set our internal release time coefficients based on times and sample rate */
    void setReleaseTime(double release_in_ms);
};


/**
\enum filterAlgorithm
\ingroup Constants-Enums
\brief
Use this strongly typed enum to easily set the filter algorithm for the AudioFilter object or any other object that requires filter definitions.

- filterAlgorithm { kLPF1P, kLPF1, kHPF1, kLPF2, kHPF2, kBPF2, kBSF2, kButterLPF2, kButterHPF2, kButterBPF2, kButterBSF2, kMMALPF2, kMMALPF2B, kLowShelf, kHiShelf, kNCQParaEQ, kCQParaEQ, kLWRLPF2, kLWRHPF2, kAPF1, kAPF2, kResonA, kResonB, kMatchLP2A, kMatchLP2B, kMatchBP2A, kMatchBP2B, kImpInvLP1, kImpInvLP2 };

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
enum subdivision {oneSixteenth, tripletEighth, oneEighth, tripletFourth, dottedEighth, oneFourth, tripletHalf, dottedFourth, oneHalf, dottedHalf, whole, numSubdivisions};

struct tempoSyncParameters
{
    tempoSyncParameters() {}
    
    tempoSyncParameters& operator=(const tempoSyncParameters& params)
    {
        if (this == &params)
            return *this;
        
        sub = params.sub;
        bpm = params.bpm;
        
        return *this;
    }
    
    subdivision sub = oneFourth;
    double bpm = 0.0;
};

class tempoSync
{
public:
    tempoSync(){}
    ~tempoSync(){}
    
    tempoSyncParameters getParameters() {return parameters;}
    
    void setParameters(const tempoSyncParameters &params)
    {
        if (params.sub != parameters.sub || params.bpm != parameters.bpm)
            parameters = params;
    }
    
    float getNoteLengthInHz()
    {
        float wholeNoteInHz = parameters.bpm/60.0 * 0.25;
        float noteLengthInHz = wholeNoteInHz * timeMultiplier[parameters.sub];
        
        return noteLengthInHz;
    }
    
protected:
    
    tempoSyncParameters parameters;
    
    float sampleRate;
    float timeMultiplier[numSubdivisions] = {1.0, 1.3333333, 2.0, 2.6666666, 3.0, 4.0, 5.3333333, 6.0, 8.0, 12.0, 16.0};
};
/**
\struct PhaseShifterParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the PhaseShifter object.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/

enum class PhaserMode {frequency, bpm, envelope};
struct PhaserParameters
{
    PhaserParameters() {}
    /** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
    PhaserParameters& operator=(const PhaserParameters& params)
    {
        if (this == &params)
            return *this;

        lfoRate_Hz = params.lfoRate_Hz;
        cutoff_lfoRate = params.cutoff_lfoRate;
        lfoDepth_Pct = params.lfoDepth_Pct;
        intensity_Pct = params.intensity_Pct;
        cutoff_Freq = params.cutoff_Freq;
        waveform = params.waveform;
        num_apfs = params.num_apfs;
        mix = params.mix;
        saturation = params.saturation;
        quadPhase = params.quadPhase;

        mode = params.mode;
        sub = params.sub;
        bpm = params.bpm;
        
        attack = params.attack;
        release = params.release;
        threshold = params.threshold;
        
        return *this;
    }

    // --- individual parameters
    float lfoRate_Hz = 0.0;    ///< phaser LFO rate in Hz
    float cutoff_lfoRate = 0.0;
    float lfoDepth_Pct = 0.0;    ///< phaser LFO depth in %
    float intensity_Pct = 0.0;    ///< phaser feedback in %
    float cutoff_Freq = 0.0;    ///< allpass cutoff Freq
    int waveform = 0;           ///< waveform
    int num_apfs = 4;
    subdivision sub = oneFourth;
    PhaserMode mode = PhaserMode::frequency;
    float mix = 0.5;
    float saturation = 0.001;
    bool quadPhase = false;
    float attack = 0.0;
    float release = 0.0;
    float threshold = 0.0;
    double bpm = 0.0;
};

// --- constants for Phaser
const unsigned int MAX_PHASER_STAGES = 48;
/**
\class PhaseShifter
\ingroup FX-Objects
\brief
The PhaseShifter object implements a six-stage phaser.

Audio I/O:
- Processes mono input to mono output.

Control I/F:
- Use BiquadParameters structure to get/set object params.

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class Phaser
{
public:
    Phaser(void) {
        OscillatorParameters lfoparams = lfo.getParameters();
        lfoparams.waveform = generatorWaveform::kTriangle;    // kTriangle LFO for phaser
        lfo.setParameters(lfoparams);
        cutoffLFO.setParameters(lfoparams);
        
        tempoSyncParameters tempoParams = tSync.getParameters();
        tempoParams.sub = oneFourth;
        tempoParams.bpm = 0.0;
        tSync.setParameters(tempoParams);

        biquadParams = apf[0].getParameters();
        biquadParams.biquadCalcType = biquadAlgorithm::kTransposeCanonical;
        biquadParams.algorithm = filterAlgorithm::kAPF2; // can also use 2nd order
        biquadParams.Q = 0.001; //use low Q if using 2nd order APFs
        
        AudioDetectorParameters envParams = envelopeFollower.getParameters();
        envParams.attackTime_mSec = 100.0;
        envParams.releaseTime_mSec = 100.0;
        envParams.clampToUnityMax = false;
        envelopeFollower.setParameters(envParams);

        for (int i = 0; i < MAX_PHASER_STAGES; i++)
        {
            apf[i].setParameters(biquadParams,true);
        }
        
    }    /* C-TOR */

    ~Phaser(void) {}    /* D-TOR */

public:
    /** reset members to initialized state */
    bool reset(double _sampleRate)
    {
        // --- reset LFO
        sampleRate = _sampleRate;
        lastEnv = 0.0;
        lfo.reset(_sampleRate);
        cutoffLFO.reset(_sampleRate);
        envelopeFollower.reset(_sampleRate);

        // --- reset APFs
        for (int i = 0; i < MAX_PHASER_STAGES; i++){
            apf[i].reset(_sampleRate);
            gamma[i] = 0.0;
        }

        return true;
    }

    /** process autio through phaser */
    /**
    \param xn input
    \return the processed sample
    */
    float processAudioSample(float xn)
    {
        // get LFO values from LFO generators for BPM and Freq modes
        SignalGenData lfoData = lfo.renderAudioOutput();
        SignalGenData cutoff_lfoData = cutoffLFO.renderAudioOutput();
        
        // get value from envelopeFollower for env mode
        float envValue = envelopeFollower.processAudioSample(xn);
        
        // value edited for phasing
        float phaseValue;

        // parameters to be used in function
        float cutoff = parameters.cutoff_Freq; //cutoff for apfs
        float depth = parameters.lfoDepth_Pct*0.99; //depth limited to 0.99
        float cutoff_lfoValue = cutoff_lfoData.normalOutput; //cutoff for wobble knob
        float cutoffMin = cutoff - (depth*0.3*cutoff); //wobble min
        float cutoffMax = cutoff + (depth*0.3*cutoff); //wobble max
        float modCutoff = doBipolarModulation(cutoff_lfoValue, cutoffMin, cutoffMax); //find wobble amount
        float apf_minF = modCutoff - (depth * modCutoff); //min value for phaser
        float apf_maxF = modCutoff + (depth * modCutoff); //max value for phaser
        
        biquadParams = apf[0].getParameters(); //get params from GUI
        
        if (parameters.mode == PhaserMode::envelope)
        {
            phaseValue = 10*envValue;
            biquadParams.fc = doUnipolarModulationFromMin(parameters.threshold*phaseValue, apf_minF, apf_maxF);
        }
        else
        {
        
            // --- create the bipolar modulator value
            phaseValue = lfoData.normalOutput;
            if (parameters.quadPhase)
                phaseValue = lfoData.quadPhaseOutput_pos;
            
            biquadParams.fc = doBipolarModulation(phaseValue, apf_minF, apf_maxF);
        }
        
        //calculate modulated values for each APF
        
        int num_apfs = parameters.num_apfs; //would equal 24 at max
        
        apf[0].setParameters(biquadParams,true); //since apfs are the same, only calculate coeffs for first one
        auto coeffs = apf[0].getCoefficients(); //get the new coeffs
        for(int i = 1; i < num_apfs; i++) //for every other apf...
        {
            apf[i].setParameters(biquadParams, false); //...set params without recalculating...
            apf[i].setCoefficients(coeffs); //...and update coeffs manually
        }

        // calculate gamma values - input gain into biquad structure
        
        gamma[0] = apf[num_apfs-1].getG_value();
        
        for (int i = 1; i < num_apfs; i++)
        {
            gamma[i] = apf[num_apfs-1-i].getG_value() * gamma[i-1];
        }

        // set the alpha0 value
        float K = parameters.intensity_Pct;
        float alpha0 = 1.0 / (1.0 + K*gamma[num_apfs-1]);

        // create combined feedback
        
        float Sn = apf[num_apfs-1].getS_value();
        for (int i = 0; i < num_apfs; i++)
        {
            Sn += gamma[i] * apf[num_apfs-1-i].getS_value();
        }
        
        float fb_var = K*Sn;
        float u,APF;
        
        //input is equal to gain alpha0 * (input + feedback)
        u = alpha0*(xn+fb_var); //u = alpha0*(xn + K*Sn);
        
        APF = apf[0].processAudioSample(u);
        for (int i = 1; i < MAX_PHASER_STAGES - 1; i++)
        {
           APF = apf[i].processAudioSample(APF);
        }
        
        // saturation control, tuned for preference
        APF = fuzzExp1WaveShaper(APF, parameters.saturation, 0.0+(parameters.saturation*expf(parameters.saturation)));
            
        // sum with -3dB coefficients
        float output = 0.707*xn + 0.707*APF;

        output = (1-parameters.mix)*xn + parameters.mix*output;

        return output;
    }

    /** get parameters: note use of custom structure for passing param data */
    /**
    \return PhaserParameters custom data structure
    */
    PhaserParameters getParameters() { return parameters; }

    /** set parameters: note use of custom structure for passing param data */
    /**
    \param PhaserParameters custom data structure
    */
    void setParameters(const PhaserParameters& params)
    {
        AudioDetectorParameters eparams = envelopeFollower.getParameters();
        // --- update LFO rate
        if (params.lfoRate_Hz != parameters.lfoRate_Hz || params.waveform != parameters.waveform || params.mode != parameters.mode || params.sub != parameters.sub || params.bpm != parameters.bpm)
        {
            OscillatorParameters lfoparams = lfo.getParameters();
            tempoSyncParameters tparams = tSync.getParameters();
            if (params.mode == PhaserMode::bpm)
            {
                tparams.sub = params.sub;
                tparams.bpm = params.bpm;
                tSync.setParameters(tparams);
                lfoparams.frequency_Hz = tSync.getNoteLengthInHz();
            }
            else if (params.mode == PhaserMode::envelope)
            {
                eparams.attackTime_mSec = params.attack;
                eparams.releaseTime_mSec = params.release;
                
                envelopeFollower.setParameters(eparams);
                lfoparams.frequency_Hz = 0.0;
            }
            else
                lfoparams.frequency_Hz = params.lfoRate_Hz;
            
            lfoparams.waveform = static_cast<generatorWaveform>(params.waveform);
            
            lfo.setParameters(lfoparams);
            
        }
        
        if (params.cutoff_lfoRate != parameters.cutoff_lfoRate)
        {
            auto cutoffLFOParams = cutoffLFO.getParameters();
            cutoffLFOParams.frequency_Hz = params.cutoff_lfoRate;
            cutoffLFO.setParameters(cutoffLFOParams);
        }
        
        if (params.attack != parameters.attack || params.release != parameters.release)
        {
            eparams.attackTime_mSec = params.attack;
            eparams.releaseTime_mSec = params.release;
            envelopeFollower.setParameters(eparams);
        }
        // --- save new
        parameters = params;
    }
protected:
    PhaserParameters parameters;///< the object parameters
    BiquadParameters biquadParams;
    Biquad apf[MAX_PHASER_STAGES];        ///< twelve APF objects
    float gamma[MAX_PHASER_STAGES];
    LFO lfo; ///< the one and only LFO
    LFO cutoffLFO;
    tempoSync tSync;
    AudioDetector envelopeFollower;
    double sampleRate = 0.0;
    float lastEnv = 0.0;
};
