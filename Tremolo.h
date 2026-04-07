#ifndef TREMOLO_H
#define TREMOLO_H

#include "libraries/oscillators/rch_oscillators.h"
#include "Effect.h"

class Tremolo : public Effect
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
        //Default constructor
        Tremolo();
        //Constructor
        Tremolo(float _rate, float _depth, std::string shape, float _sampleRate);
        float process(float sample) override;
};

#endif
