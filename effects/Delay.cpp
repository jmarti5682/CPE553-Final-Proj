#include "Delay.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <stddef.h>

//Default constructor
//@brief Creates and initializes a Delay object with time = 1.0s, mix = 0.5, feedback = 0.25, and sampleRate = 48000 Hz
Delay::Delay()
{
    time = 48000;
    mix = 0.5;
    feedback = 0.25;
    sampleRate = 48000;
    writeIndex = 0;

    createBuffer();
}

//Constructor
//@brief Creates and initializes a Delay object with user defined parameters. Delay acts as an echo-type effect, overlaying a time-delayed version of the input signal on top of itself
//@param _time Delay time (seconds), or the amount of time between the start of playback to the initial introduction of the delayed signal
//@param _mix Delay mix, or the amount of delayed signal to be overlayed on the original signal
//@param _feedback Delay feedback, or the amount of delay output to write back to the delay buffer
//@param _sampleRate Sample rate associated with the delay object. This should be extracted from the audio file itself using getSampleRate() from ProcessAudio class
Delay::Delay(float _time, float _mix, float _feedback, int _sampleRate)
{
    sampleRate = _sampleRate;
    time = _time * sampleRate; //convert from delay time in seconds to samples
    mix = _mix;
    feedback = _feedback;
    writeIndex = 0;

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
    size_t readIndex = (writeIndex + bufferLength - time) % bufferLength;

    //read delayed sample from buffer, at the current read index
    float wet = buffer[readIndex];

    //write current input sample AND current delayed sample (scaled by feedback) to buffer 
    writeBuffer(sample, wet);

    //out is a blend of dry and wet signals
    float out = (1 - mix) * sample + mix * wet;

    return out;
}

//@brief Writes the current input sample and the current delayed sample (scaled by feedback) to the delay buffer, then increments writeIndex
//@param sample Current input sample
//@param wet Current delayed sample, i.e. the sample which has most recently been read from the delay buffer
//@return void
void Delay::writeBuffer(float sample, float wet)
{
    // write current sample + feedback into buffer at write position
    buffer[writeIndex] = sample + (wet * feedback);

    // advance write pointer with wrap
    writeIndex = (writeIndex + 1) % bufferLength;
}

//@brief Helper function called by the constructor to initialize the delay buffer with user defined parameters. Zeroes out the buffer to prevent reading garbage data
//@return void
void Delay::createBuffer()
{
    //instantiate circular buffer with size equal to delay samples
    bufferLength = std::max<size_t>(1, 2 * time); //circular buffer length is double the delay length
    buffer = new float[bufferLength]; //instantiate buffer* as an array
    std::fill_n(buffer, bufferLength, 0.0f); //zero out the buffer to prevent reading garbage data
}