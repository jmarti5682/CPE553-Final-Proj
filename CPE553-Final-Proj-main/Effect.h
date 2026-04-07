#ifndef EFFECT_H
#define EFFECT_H

class Effect
{
    private:
        

    public:
        virtual float process(float sample);
        virtual ~Effect() = default;

        float sampleRate;
        float getSampleRate();
};

#endif