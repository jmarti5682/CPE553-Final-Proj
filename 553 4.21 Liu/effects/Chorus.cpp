#include "Chorus.h"
#include <stddef.h>

//Default constructor
//@brief Creates and initializes a Chorus object with time = 10ms, mix = 0.5, feedback = 0.3, rate = 1.0 Hz, depth = 2ms and sampleRate = 48000 Hz
Chorus::Chorus()
{
    time = 0.01*48000;
    mix = 0.5;
    feedback = 0.3;
    rate = 1.0;
    depth = 0.002;
    sampleRate = 48000;
    
    createBuffer();
}
//Constructor
//@brief Creates and initializes a Chorus object with user defined parameters. Chorus acts as a delay-type effect where the delay time is modulated with a low frequency oscillator (LFO) signal overtime.
//@param _time Delay time (seconds), or the amount of time between the start of playback to the initial introduction of the delayed signal. For chorus, this will be modulated.
//@param _mix Chorus mix, or the amount of chorus signal to be overlayed on the original signal
//@param _feedback Chorus feedback, or the amount of chorus output to write back to the delay buffer
//@param _rate Chorus rate, or the frequency of the modulating LFO signal
//@param _depth Chorus depth (seconds), or the amplitude of the modulating LFO signal, i.e. the amount of samples by which the delay is modulated from its fixed value
//@param _sampleRate Sample rate associated with the chorus object. This should be extracted from the audio file itself using getSampleRate() from ProcessAudio class
Chorus::Chorus(float _time, float _mix, float _feedback, float _rate, float _depth, float _sampleRate)
{
    sampleRate = _sampleRate;
    time = _time * sampleRate; //convert from delay time in seconds to samples
    mix = _mix;
    feedback = _feedback;
    rate = _rate;
    depth = _depth * sampleRate; //convert from mod depth in seconds to samples

    oscSine.setFrequency(rate);
    oscSine.setAmplitude(depth);
    oscSine.setSampleRate(sampleRate);

    createBuffer();
}

//Public member functions
//@brief Takes an audio sample as input and outputs a sample processed by the chorus effect
//@param sample Input audio sample
//@return Audio sampled processed by chorus effect, i.e. input signal + chorus signal
float Chorus::process(float sample)
{
    modTimeSamps = time + oscSine.tick();

    // read delayed sample from buffer at readIndex (with wrap)
    float readIndex = writeIndex - modTimeSamps;
    while (readIndex < 0)
        readIndex += bufferLength;
    while (readIndex >= bufferLength)
        readIndex -= bufferLength;

    float wet = interpolate(readIndex);

    writeBuffer(sample, wet);

    float out = sample * (1 - mix) + mix * wet;

    return out;
}

//@brief Takes a non-integer read index as input and returns the linearly interpolated sample value. Modulation process yields non integer read indeces for the delay buffer, so we essentially estimate the sample value at this non integer index
//@param readIndex The raw read index value obtained after modulation
//@return Interpolated sample value
float Chorus::interpolate(float readIndex)
{
    int i0 = static_cast<int>(readIndex);
    int i1 = (i0 + 1) % bufferLength;
    float diff = readIndex - i0;

    float y0 = buffer[i0];
    float y1 = buffer[i1];

    float wet = y0 + diff * (y1-y0);

    return wet;
}

