#ifndef PROCESSAUDIO_H
#define PROCESSAUDIO_H

#include <string>
#include "libraries/AudioFile.h"
#include "Effect.h"
#include "Tremolo.h"

class ProcessAudio
{
private:
    AudioFile<double> audioFile;
    std::string filename;
    float sampleRate;
    int bitDepth;
    int numChannels;

public:
    // Default constructor
    ProcessAudio();
    // Constructor
    ProcessAudio(std::string filename);

    // Public member functions
    void loadFile(std::string filename);
    void writeFile(std::string filename);
    void processFile(Effect& effect);

    float getSampleRate();
    int getBitDepth();
    int getNumChannels();
    std::vector<std::vector<double>> getSamples();

};

#endif
