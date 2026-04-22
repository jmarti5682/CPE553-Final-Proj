#include "Tremolo.h"
#include <iostream>

//Default constructor
//@brief Creates and initializes a tremolo object with rate = 1.0, depth = 1.0, sample rate = 48000, with sinusoidal waveshape
Tremolo::Tremolo()
{
    rate = 1.0;
    depth = 1.0;
    waveShape = SINE;
    sampleRate = 48000;

    //Set oscillator parameters
    oscSine.setFrequency(rate);
    oscSine.setAmplitude(depth);
    oscSine.setSampleRate(sampleRate);
}   

//Constructor
//@brief Creates and initializes a tremolo object with user defined parameters. Tremolo effect modulates the volume of an input waveform with an oscillator-generated waveform
//@param _rate Tremolo rate, or the rate of amplitude modulation.
//@param _depth Tremolo depth, or the amount of amplitude modulation
//@param shape Wave shape of the modulating waveform, either sine, triangle, or square wave
//@param _sampleRate Sample rate associated with the tremolo object. This should be extracted from the audio file itself using getSampleRate() from ProcessAudio class
Tremolo::Tremolo(float _rate, float _depth, std::string shape, int _sampleRate)
{
    rate = _rate;
    depth = _depth;
    sampleRate = _sampleRate;

    //Assign value to waveShape based on the string input for shape
    if (shape == "sine")
        waveShape = SINE;
    else if (shape == "triangle")
        waveShape = TRIANGLE;
    else if (shape == "square")
        waveShape = SQUARE;
    else
    {
        std::cout << "Waveshape not recognized. Defaulting to sine." << std::endl;
        waveShape = SINE;
    }

    //Each oscillator object is tied to its waveshape, so we assign depth/rate/SR values to each even though we only end up using one
    oscSine.setFrequency(rate);
    oscSine.setAmplitude(depth);
    oscSine.setSampleRate(sampleRate);

    oscTri.setFrequency(rate);
    oscTri.setAmplitude(depth);
    oscTri.setSampleRate(sampleRate);

    oscSqu.setFrequency(rate);
    oscSqu.setAmplitude(depth);
    oscSqu.setSampleRate(sampleRate);
}

//@brief Takes an audio sample as input and outputs a sample processed by the tremolo effect
//@param sample Input audio sample
//@return Audio sampled processed by tremolo effect
float Tremolo::process(float sample) 
{
    float lfo;
    switch (waveShape)
    {
        case SINE:
            lfo = (oscSine.tick() + 1.0f) * 0.5f;
            return sample * lfo;
            break;

        case TRIANGLE:
            lfo = (oscTri.tick() + 1.0f) * 0.5f;
            return sample * lfo;
            break;

        case SQUARE:
            lfo = (oscSqu.tick() + 1.0f) * 0.5f;
            return sample * lfo;
            break;

        default:
            return sample;
            break;
    }
}