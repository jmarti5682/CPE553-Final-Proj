/*
    Name: Jose Martinez-Ponce
    Date: 04/02/2026
    Purpose: Basic frequency bar visualizer using ncurses.
             Uses FFTW3 library for the FFT computation.
*/

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

std::vector<double> getMagnitudes(const std::vector<double> &samples, int fftSize)
{
    double *in = fftw_alloc_real(fftSize);
    fftw_complex *out = fftw_alloc_complex(fftSize / 2 + 1);
    fftw_plan plan = fftw_plan_dft_r2c_1d(fftSize, in, out, FFTW_ESTIMATE);

    int take;
    if ((int)samples.size() < fftSize)
    {
        take = (int)samples.size();
    }
    else
    {
        take = fftSize;
    }

    for (int i = 0; i < fftSize; i++)
    {
        if (i < take)
        {
            in[i] = samples[i];
        }
        else
        {
            in[i] = 0.0;
        }
    }

    fftw_execute(plan);

    int halfSize = fftSize / 2;
    std::vector<double> mag(halfSize);

    for (int i = 0; i < halfSize; i++)
    {
        double r = out[i][0];  // real
        double im = out[i][1]; // imaginary
        mag[i] = sqrt(r * r + im * im) / (fftSize / 2.0);
    }

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return mag;
}

// =========================
//  VISUALIZER
// =========================

class Visualizer
{
public:
    static constexpr int FFT_SIZE = 2048;

    // Draw a single frame from a chunk of samples
    void drawFrame(const std::vector<double> &chunk, std::string label)
    {
        int rows, cols;
        getmaxyx(stdscr, rows, cols);

        int barRows = rows - 2; // row 0 = title, last row = empty
        int numBars = cols / 2;

        if (barRows < 1 || numBars < 1)
            return;

        std::vector<double> mag = getMagnitudes(chunk, FFT_SIZE);
        int numBins = (int)mag.size();

        // Map bins to bars linearly
        std::vector<double> barMag(numBars, 0.0);
        for (int b = 0; b < numBars; b++)
        {
            int bin0 = b * numBins / numBars;
            int bin1 = (b + 1) * numBins / numBars;
            if (bin1 <= bin0)
            {
                bin1 = bin0 + 1;
            }
            double sum = 0.0;
            for (int i = bin0; i < bin1 && i < numBins; i++)
            {
                sum += mag[i];
            }
            barMag[b] = sum / (bin1 - bin0);
        }

        // Find max magnitude to normalize bar heights
        double maxMag = 0.001;
        for (double m : barMag)
        {
            if (m > maxMag)
            {
                maxMag = m;
            }
        }
        // Title bar
        std::string title = " CPE553 Visualizer  [" + label + "]  (press 'q' to exit) ";
        while ((int)title.size() < cols)
        {
            title += ' ';
        }
        title.resize(cols);
        mvaddstr(0, 0, title.c_str());

        // Draw bars
        for (int b = 0; b < numBars; b++)
        {
            int barHeight = (int)((barMag[b] / maxMag) * barRows);

            for (int row = 0; row < barRows; row++)
            {
                int screenRow = row + 1;
                int screenCol = b * 2;

                // Fill from the bottom up
                bool filled = (row >= barRows - barHeight);
                mvaddch(screenRow, screenCol, filled ? '#' : ' ');
                mvaddch(screenRow, screenCol + 1, ' ');
            }
        }

        refresh();
    }
};

// =========================
//  Called from main()
//  Loops through the audio
//  chunk by chunk and animates
//  the bars.
// =========================
void visualize(ProcessAudio &processor, std::string label)
{
    auto samples = processor.getSamples();
    int sampleRate = (int)processor.getSampleRate();

    if (samples.empty() || samples[0].empty())
    {
        return;
    }

    std::vector<double> &leftChannel = samples[0];
    int hop = sampleRate * 40 / 1000;

    // functions for ncurses
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);

    Visualizer viz;
    int offset = 0;

    while (offset + Visualizer::FFT_SIZE < (int)leftChannel.size())
    {
        std::vector<double> chunk(leftChannel.begin() + offset, leftChannel.begin() + offset + Visualizer::FFT_SIZE);
        viz.drawFrame(chunk, label);

        if (getch() == 'q')
        {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        offset += hop;
    }

    endwin();
}
