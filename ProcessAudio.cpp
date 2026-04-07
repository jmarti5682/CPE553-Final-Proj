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
    std::string filePath = "data/" + filename;

    audioFile.load(filePath);

    sampleRate = audioFile.getSampleRate();
    bitDepth = audioFile.getBitDepth();
    numChannels = audioFile.getNumChannels();
}

void ProcessAudio::loadFile(std::string filename)
{
    this->filename = filename;
    std::string filePath = "data/" + filename;

    audioFile.load(filePath);

    sampleRate = audioFile.getSampleRate();
    bitDepth = audioFile.getBitDepth();
    numChannels = audioFile.getNumChannels();
}

void ProcessAudio::writeFile(std::string filename)
{
    std::string filePath = "data/" + filename;
    audioFile.save(filePath);
    std::cout << "Wrote " << filename << " to \"data\" directory." << std::endl;
}

void ProcessAudio::processFile(Effect& effect)
{
    for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
    {
        double processedSample = effect.process(audioFile.samples[0][i]);
        audioFile.samples[0][i] = processedSample;
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

std::vector<std::vector<double>> ProcessAudio::getSamples()
{
    return audioFile.samples;
}
