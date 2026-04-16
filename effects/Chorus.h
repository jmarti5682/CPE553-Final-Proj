#include "Delay.h"
#include "../libraries/oscillators/rch_oscillators.h"
#include <stddef.h>

class Chorus : public Delay
{
    private:
        float rate;
        float depth;
        float modTimeSamps;
        RCH::Oscillators::Templates::Sine oscSine; //Sine wave oscillator
        
    public:
    //Default constructor
        Chorus();
    //Constructor
        Chorus(float _time, float _mix, float _feedback, float _rate, float _depth, float _sampleRate);

    //Public member functions
        float process(float sample) override;

        float interpolate(float readIndex);
};