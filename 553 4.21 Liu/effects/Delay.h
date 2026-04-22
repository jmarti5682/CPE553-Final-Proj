#ifndef DELAY_H
#define DELAY_H

#include "Effect.h"
#include <stddef.h>

class Delay : public Effect
{
    protected:
        size_t time; //time measured in SAMPLES, not seconds; when constructor is called, time argument (in seconds) will be converted to samples
        float mix;
        float feedback;
        size_t writeIndex;
        size_t bufferLength;
        float* buffer;
        
    public:
        //Default constructor  
        Delay();
        //Constructor
        Delay(float _time, float _mix, float _feedback, int _sampleRate);
        //Destructor
        ~Delay() override;

        //Public member functions
        float process(float sample) override;

        void writeBuffer(float sample, float wet);

        void createBuffer(); 
};

#endif