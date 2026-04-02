#include <iostream>
#include "libraries/oscillators/rch_oscillators.h"
#include "ProcessAudio.h"
#include "Tremolo.h"
using namespace std;

int main()
{
    ProcessAudio test("testaudio_clean.wav");

    Tremolo tremolo;

    test.processFile(tremolo);

    test.writeFile("output.wav");

    float sr = test.getSampleRate();

    cout << sr;
}