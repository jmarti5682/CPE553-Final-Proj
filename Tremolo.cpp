#include "Tremolo.h"
#include <iostream>

//Default constructor
Tremolo::Tremolo()
{
    rate = 1.0;
    depth = 1.0;
    waveShape = SINE;
    sampleRate = 48000.0;

    oscSine.setFrequency(rate);
    oscSine.setAmplitude(depth);
    oscSine.setSampleRate(sampleRate);
}   

//Constructor
Tremolo::Tremolo(float _rate, float _depth, std::string shape, float _sampleRate)
{
    rate = _rate;
    depth = _depth;
    sampleRate = _sampleRate;

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

float Tremolo::process(float sample) 
{
    float lfo;
    switch (waveShape)
    {
        case SINE:
            lfo = (oscSine.tick() + 1.0f) * 0.5f;  // maps -1..1 to 0..1
            return sample * lfo;
            break;

        case TRIANGLE:
            return sample * oscTri.tick();

        case SQUARE:
            return sample * oscSqu.tick();

        default:
            return sample;
    }
}