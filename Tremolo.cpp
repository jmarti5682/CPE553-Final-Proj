#include "Tremolo.h"
#include <iostream>

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
    switch (waveShape)
    {
        case SINE:
            return sample * oscSine.tick();

        case TRIANGLE:
            return sample * oscTri.tick();

        case SQUARE:
            return sample * oscSqu.tick();

        default:
            return sample;
    }
}