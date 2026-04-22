#ifndef TREMOLO_H
#define TREMOLO_H

#include "../libraries/oscillators/rch_oscillators.h"
#include "Effect.h"
#include <string>//...

class Tremolo : public Effect
{
    private:
        float rate; //Tremolo rate, i.e. frequency of the digitally generated oscillator
        float depth; //Tremolo depth, i.e. amount of tremolo OR amplitude of the digitally generated oscillator

        //Tremolo constructor accepts a string for wave shape, then assigns the proper WaveShape to the object's waveShape member variable
        enum WaveShape
        {
            SINE,
            TRIANGLE,
            SQUARE
        };

        WaveShape waveShape;

        //Three oscillator types corresponding to each of the three waveshapes
        //Each oscillator object is directly tied to its waveshape, so we need all three available as member variables even if we don't end up using them all
        RCH::Oscillators::Templates::Sine oscSine; //Sine wave oscillator
        RCH::Oscillators::Templates::Triangle oscTri; //Triangle wave oscillator
        RCH::Oscillators::Templates::Square oscSqu; //Square wave oscillator

    public:
        //Default constructor
        Tremolo();
        //Constructor
        Tremolo(float _rate, float _depth, std::string shape, int _sampleRate);
        
        float process(float sample) override;
};

#endif
