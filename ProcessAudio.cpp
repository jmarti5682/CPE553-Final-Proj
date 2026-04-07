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
}

void ProcessAudio::processFile(std::string effectType)
{
    if (effectType == "tremolo")
    {
        float rate, depth;
        std::string shape;

        std::cout << "Enter tremolo rate:" << std::endl;
        std::cin >> rate;

        std::cout << "Enter tremolo depth:" << std::endl;
        std::cin >> depth;

        std::cout << "Enter desired wave shape (sine, triangle, or square):" << std::endl;
        std::cin >> shape;

        Tremolo tremolo(rate, depth, shape, getSampleRate());

        for (int channel = 0; channel < audioFile.getNumChannels(); channel++)
        {
            for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
            {
                double processedSample = tremolo.process(audioFile.samples[channel][i]);
                audioFile.samples[channel][i] = processedSample;
            }
        }
    }
    else
    {
        std::cout << "Effect type not recognized. Audio not processed" << std::endl;
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
