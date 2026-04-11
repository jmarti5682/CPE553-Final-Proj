#include "Delay.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <stddef.h>

//Default constructor
//@brief Creates and initializes a Delay object with time = 1.0, mix = 0.5, feedback = 0.25, and sampleRate = 48000
Delay::Delay()
{
    time = 1.0;
    mix = 0.5;
    feedback = 0.25;
    sampleRate = 48000;
    sampleCount = 0;

    createBuffer();
}

//Constructor
//@brief Creates and initializes a Delay object with user defined parameters. Delay acts as an echo-type effect, overlaying a time-delayed version of the input signal on top of itself
//@param _time Delay time, or the amount of time between the start of playback to the initial introduction of the delayed signal
//@param _mix Delay mix, or the amount of delayed signal to be overlayed on the original signal
//@param _feedback Delay feedback, or the amount of delay output to write back to the delay buffer
//@param _sampleRate Sample rate associated with the delay object. This should be extracted from the audio file itself using getSampleRate() from ProcessAudio class
Delay::Delay(float _time, float _mix, float _feedback, int _sampleRate)
{
    time = _time;
    mix = _mix;
    feedback = _feedback;
    sampleRate = _sampleRate;
    sampleCount = 0;

    createBuffer();
}

//Destructor
//@brief Deallocates the delay buffer
Delay::~Delay()
{
    delete[] buffer;
}

//@brief Takes an audio sample as input and outputs a sample processed by the delay effect
//@param sample Input audio sample
//@return Audio sampled processed by delay effect, i.e. input signal + delayed copy
float Delay::process(float sample)
{
    // read delayed sample from buffer at readIndex (with wrap)
    size_t readIndex = (sampleCount + bufferLength - timeSamps) % bufferLength;

    float delayed = buffer[readIndex];

    // output is dry + mix * wet
    float out = sample + (mix * delayed);

    // write current sample + feedback into buffer at write position
    buffer[sampleCount] = sample + (delayed * feedback);

    // advance write pointer with wrap
    sampleCount = (sampleCount + 1) % bufferLength;

    return out;
}

//@brief Helper function called by the constructor to initialize the delay buffer with user defined parameters. Zeroes out the buffer to prevent reading garbage data
void Delay::createBuffer()
{
    // Calculate # of samples corresponding to delay time in seconds, instantiate circular buffer with size equal to delay samples
    timeSamps = static_cast<int>(std::round(time * sampleRate)); //delay time in SAMPLES
    bufferLength = std::max<size_t>(1, 2 * timeSamps); //circular buffer length is double the delay length
    buffer = new float[bufferLength]; //instantiate buffer* as an array
    std::fill_n(buffer, bufferLength, 0.0f); //zero out the buffer to prevent reading garbage data
}