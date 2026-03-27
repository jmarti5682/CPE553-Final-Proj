#ifndef TREMOLO_H
#define TREMOLO_H

#include <string>
#include "libraries/oscillators/rch_oscillators.h"

class Tremolo
{
private:
    float rate;
    float depth;
    float sampleRate;

    enum WaveShape
    {
        SINE,
        TRIANGLE,
        SQUARE
    };

    WaveShape waveShape;

    RCH::Oscillators::Templates::Sine oscSine;
    RCH::Oscillators::Templates::Triangle oscTri;
    RCH::Oscillators::Templates::Square oscSqu;

public:
    Tremolo(float _rate, float _depth, std::string shape, float _sampleRate);
    float process(float sample);
};

#endif