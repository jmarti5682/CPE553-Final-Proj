#include <iostream>
#include "libraries/oscillators/rch_oscillators.h"
#include "ProcessAudio.h"
#include "Tremolo.h"
#include "Visualizer.h"
using namespace std;

int main()
{
    ProcessAudio test("testaudio_clean.wav");

    visualize(test, "Pre-Effects"); // Before effects

    test.processFile("tremolo");

    visualize(test, "Post-Effects"); // After effects

    test.writeFile("output.wav");

    float sr = test.getSampleRate();
    cout << sr;
}
