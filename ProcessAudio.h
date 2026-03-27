#ifndef PROCESSAUDIO_H
#define PROCESSAUDIO_H

#include <string>
#include "libraries/AudioFile.h"
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
    // Constructor
    ProcessAudio(std::string _filename);

    // Public member functions
    void loadFile(std::string filename);
    void writeFile(std::string filename);
    void processFile(std::string effectType);

    float getSampleRate();
    int getBitDepth();
    int getNumChannels();
};

#endif