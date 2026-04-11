#ifndef DELAY_H
#define DELAY_H

#include "Effect.h"
#include <stddef.h>

class Delay : public Effect
{
    private:
        float time;
        float mix;
        float feedback;

        float* buffer;
        size_t sampleCount;
        size_t timeSamps;
        size_t bufferLength;
    
    public:
        //Default constructor  
        Delay();
        //Constructor
        Delay(float _time, float _mix, float _feedback, int _sampleRate);
        //Destructor
        ~Delay() override;

        //Public member functions
        float process(float sample) override;

        void createBuffer(); 
};

#endif