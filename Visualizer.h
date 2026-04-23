/*
    Name: Jose Martinez-Ponce
    Date: 04/02/2026
    Purpose: Header File for visualizer
*/

#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <ncurses.h>
#include <vector>
#include <cmath>
#include <string>
#include <thread>
#include <chrono>
#include <fftw3.h>
#include "ProcessAudio.h"

// ==========================
//  FFT  (via FFTW3)
//  Returns magnitude per
//  frequency bin for a chunk
//  of samples.
// =========================

std::vector<double> getMagnitudes(const std::vector<double> &samples, int fftSize);
void visualize(ProcessAudio &processor, std::string label);

// =========================
//  VISUALIZER
// =========================

class Visualizer
{
public:
    static constexpr int FFT_SIZE = 2048;

    // Draw a single frame from a chunk of samples
    void drawFrame(const std::vector<double> &chunk, std::string label);
};

#endif
