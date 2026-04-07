#include "ProcessAudio.h"
#include <iostream>

// Default constructor
ProcessAudio::ProcessAudio() : filename("Unknown"), sampleRate(0.0), bitDepth(0), numChannels(0)
{
    std::cout << "ProcessAudio object declared without filename associated. Please load a .wav file using loadFile()." << std::endl;
}

// Constructor
ProcessAudio::ProcessAudio(std::string filename)
{
    this->filename = filename;
    audioFile.load(filename);

    sampleRate = audioFile.getSampleRate();
    bitDepth = audioFile.getBitDepth();
    numChannels = audioFile.getNumChannels();
}

void ProcessAudio::loadFile(std::string filename)
{
    this->filename = filename;
    
    audioFile.load(filename);

    sampleRate = audioFile.getSampleRate();
    bitDepth = audioFile.getBitDepth();
    numChannels = audioFile.getNumChannels();
}

void ProcessAudio::writeFile(std::string filename)
{
    audioFile.save(filename);
}

void ProcessAudio::processFile(Effect& effect)
{
    int numChannels = audioFile.getNumChannels();
    int numSamples = audioFile.getNumSamplesPerChannel();

    for (int channel = 0; channel < numChannels; channel++)
    {
        for (int i = 0; i < numSamples; i++)
        {
            double processedSample = effect.process(audioFile.samples[channel][i]);
            audioFile.samples[channel][i] = processedSample;
        }
    }
}

float ProcessAudio::getSampleRate()
{
    return audioFile.getSampleRate();
}

int ProcessAudio::getBitDepth()
{
    return audioFile.getBitDepth();
}

int ProcessAudio::getNumChannels()
{
    return audioFile.getNumChannels();
}