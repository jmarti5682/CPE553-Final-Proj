#include "Effect.h"

//@brief Takes an audio sample as input and outputs a processed version of the sample. The version belonging to Effect (unoverloaded) simply returns the input sample unprocessed.
//@param sample Input audio sample
//@return Processed audio sample (in this case, no processing is actually done)
float Effect::process(float sample)
{
    return sample;
}

//@brief Gets sample rate associated with an Effect object
//@return Sample rate associated with the Effec
float Effect::getSampleRate()
{
    return sampleRate;
}
