/*
    Name: Jose Martinez-Ponce
    Date: 04/02/2026
    Purpose: Basic frequency bar visualizer using ncurses.
             Uses FFTW3 library for the FFT computation. Adding in Hann-Windowed FFT
*/

#include "Visualizer.h"

// static constants to visualize color in visualizer
static constexpr int CLR_TITLE = 1; // Color title
static constexpr int CLR_LOW = 2;   // Color Low = green - bottom
static constexpr int CLR_MID = 3;   // Color Mid = yellow - middle
static constexpr int CLR_HIGH = 4;  // Color High = red - top
static constexpr int CLR_PEAK = 5;  // Color Peak = white on red - peak

static void initColors()
{
    start_color();
    use_default_colors();
    init_pair(CLR_TITLE, COLOR_BLACK, COLOR_CYAN);
    init_pair(CLR_LOW, COLOR_GREEN, -1);
    init_pair(CLR_MID, COLOR_YELLOW, -1);
    init_pair(CLR_HIGH, COLOR_RED, -1);
    init_pair(CLR_PEAK, COLOR_WHITE, COLOR_RED);
}

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
        // Hann Window
        double w = 0.5 * (1.0 - std::cos(2.0 * M_PI * i / (fftSize - 1)));
        if (i < take)
        {
            in[i] = samples[i] * w;
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

void Visualizer::drawFrame(const std::vector<double> &chunk, std::string label)
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int barRows = rows - 4; // row 0 = title, last row = freqLabels
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
    attron(COLOR_PAIR(CLR_TITLE) | A_BOLD);
    mvaddstr(0, 0, title.c_str());
    attroff(COLOR_PAIR(CLR_TITLE | A_BOLD));

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
            bool isPeak = filled && (row == barRows - barHeight);

            if (filled)
            {
                double relativePos = 1.0 - (double)row / barRows; // 1.0 = Bottom, 0.0 = Top
                int pair;
                if (isPeak)
                {
                    pair = CLR_PEAK;
                }
                else if (relativePos < 0.45)
                {
                    pair = CLR_LOW;
                }
                else if (relativePos < 0.75)
                {
                    pair = CLR_MID;
                }
                else
                {
                    pair = CLR_HIGH;
                }

                attron(COLOR_PAIR(pair));
                mvaddch(screenRow, screenCol, ACS_BLOCK);
                mvaddch(screenRow, screenCol + 1, ' ');
                attroff(COLOR_PAIR(pair));
            }
            else
            {
                mvaddch(screenRow, screenCol, ' ');
                mvaddch(screenRow, screenCol + 1, ' ');
            }
        }
    }

    // Bottom Row - Frequency Labels
    std::vector<std::pair<int, std::string>> freqLabels = {
        {20, "20"},
        {50, "50"},
        {100, "100"},
        {200, "200"},
        {500, "500"},
        {1000, "1k"},
        {2000, "2k"},
        {5000, "5k"},
        {10000, "10k"},
        {20000, "20k"}};
    std::string tickRow(cols, ' ');
    std::string labelRow(cols, ' ');
    int lastEnd = -1;
    // std::string bottomRow(cols, ' ');
    for (auto &[freq, label] : freqLabels)
    {
        int bar = (int)((double)freq / (sampleRate / 2.0) * numBars);
        int col = bar * 2;
        int startCol = col - (int)label.size() / 2; // centering

        if (startCol < 0)
        {
            startCol = 0;
        }

        if (col < cols)
        {
            tickRow[col] = '|';
        }

        // if no overlap from previous
        if (startCol > lastEnd && startCol + (int)label.size() < cols)
        {
            for (int i = 0; i < (int)label.size(); i++)
            {
                labelRow[startCol + i] = label[i];
            }
            lastEnd = startCol + (int)label.size();
        }
    }
    mvaddstr(rows - 2, 0, tickRow.c_str());
    mvaddstr(rows - 1, 0, labelRow.c_str());

    refresh();
}

// =========================
//  Called from main()
//  Loops through the audio
//  chunk by chunk and animates
//  the bars.
// =========================
void visualize(ProcessAudio &processor, std::string label, std::string filename)
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
    initColors();

    // MACOS COMMAND - ONLY WORKS ON MAC
    std::string cmd = "afplay data/" + filename + " &";
    system(cmd.c_str());

    Visualizer viz;
    viz.sampleRate = sampleRate;
    int offset = 0;

    auto startTime = std::chrono::steady_clock::now();

    while (offset + Visualizer::FFT_SIZE < (int)leftChannel.size())
    {
        std::vector<double> chunk(leftChannel.begin() + offset, leftChannel.begin() + offset + Visualizer::FFT_SIZE);

        viz.drawFrame(chunk, label);

        if (getch() == 'q')
        {
            break;
        }

        // There is a slight delay with the visualization and playback
        // Need to get track of the delay
        offset += hop;
        double expectedMs = (double)offset / sampleRate * 1000.0;
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
        int sleepMs = (int)(expectedMs - elapsed);
        if (sleepMs > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
        }
    }

    // MACOS COMMAND
    system("pkill afplay");

    endwin();
}
