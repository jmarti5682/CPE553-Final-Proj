#ifndef EFFECT_H
#define EFFECT_H

class Effect
{
    public:
        virtual float process(float sample);
        virtual ~Effect() = default;

        int sampleRate;
        int getSampleRate();
};

#endif