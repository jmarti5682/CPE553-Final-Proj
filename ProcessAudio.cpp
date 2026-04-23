#include "ProcessAudio.h"
#include <iostream>

//Default constructor
//@brief Creates and initializes an audio processor with filename = "Unknown", sample rate = 0, bit depth = 0, num channels = 0
ProcessAudio::ProcessAudio() : filename("Unknown"), sampleRate(0.0), bitDepth(0), numChannels(0)
{
    std::cout << "ProcessAudio object declared without filename associated. Please load a .wav file using loadFile()." << std::endl;
}
//Constructor
//@brief Creates and initializes an audio processor with user defined filename. Constructor automatically extracts sample rate, bit depth, and num channels from file and assigns information to relevant member variables
//@param filename Input audio file name
ProcessAudio::ProcessAudio(std::string filename)
{
    this->filename = filename;
    std::string filePath = "data/" + filename;

    audioFile.load(filePath);

    sampleRate = audioFile.getSampleRate();
    bitDepth = audioFile.getBitDepth();
    numChannels = audioFile.getNumChannels();
}

//@brief Loads an audio file, extracts sample rate, bit depth, and num channels from the file and assigns information to relevant member variables
//@param filename Input audio file name
//@return void
void ProcessAudio::loadFile(std::string filename)
{
    this->filename = filename;
    std::string filePath = "data/" + filename;

    audioFile.load(filePath);

    sampleRate = audioFile.getSampleRate();
    bitDepth = audioFile.getBitDepth();
    numChannels = audioFile.getNumChannels();
}

//@brief Saves an output audio file to the "data" directory
//@param filename Output audio file name (new file will be created)
//@return void
void ProcessAudio::writeFile(std::string filename)
{
    std::string filePath = "data/" + filename;
    audioFile.save(filePath);
    std::cout << "Wrote " << filename << " to \"data\" directory." << std::endl;
}

//@brief Processes an input audio file with a user specified effect
//@param effect Effect with which to process input audio
//@return void
void ProcessAudio::processFile(Effect& effect)
{   
    sumToMono();
    for(int i = 0; i < audioFile.getNumChannels(); i++) //this is not necessary after summing to mono, but I'm leaving it in case I add stereo support
    {
        for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
        {
            double processedSample = effect.process(audioFile.samples[0][i]);
            audioFile.samples[0][i] = processedSample;
        }
    }
}

//@brief Converts an audio file from stereo to mono by summing the left and right channel samples with normalized amplitude
//@return void
void ProcessAudio::sumToMono()
{
    if (audioFile.isStereo())
    {
        int numSamples = audioFile.getNumSamplesPerChannel();

        std::vector<double> mono(numSamples);

        for (int i = 0; i < numSamples; ++i)
        {
            mono[i] = 0.5 * (audioFile.samples[0][i] + audioFile.samples[1][i]);
        }

        AudioFile<double>::AudioBuffer buffer;
        buffer.resize(1);
        buffer[0] = mono;

        audioFile.setAudioBuffer(buffer);
    }
    else
        std::cout << "File is already mono." << std::endl;
}

//Getters
int ProcessAudio::getSampleRate()
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
