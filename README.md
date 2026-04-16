# Audio Editor and Visualizer
> CPE555: Intro to C++
> 
> Jose Martinez-Ponce, Benjamin Kunze, Peizhi Liu


## Program Purpose

This program take in a '.wav' audio file, where it displays an animated frequency visualizer in the terminal, applies an audio effect, and then displays the visualizer again so you can see how the effect changed the sound. 

The visualizer shows which frequencies are present in the audio, bass frequencies on the left, treble on the right. 

## Milestone 1

**Jose Martinez-Ponce - Visualizer ('Visualizer.h')**

- Built a terminal-based frequency bar visualizer using ncurses
- Reads audio samples from 'ProcessAudio' via 'getSamples()' and runs an FFT (Fast Fourier Transform) to convert raw audio into frequency data

- Draws animated bars representing loudness at each frequency

- Added 'getSamples()' function to 'ProcessAudio' so the visualizer can access raw audio data

**Benjamin Kunze - Audio Processing ('ProcessAudio.h / .cpp' , 'Tremolo.h / .cpp')**

- Loads and write '.wav' files using the 'AudioFile' library
- Implemented tremolo effect with adjustable rate, depth, and wave shape (sine, triangle, square)

---

## How to Run:

### Dependencies
- 'ncurses' - for terminal rendering
- 'fftw3' - for frequency analysis
- 'AudioFile' - header-only library

**Install dependencies on macOS:**

```bash
brew install ncurses fftw
```

### Compile
```bash
g++ -std=c++17 -o editor main.cpp ProcessAudio.cpp effects/Effect.cpp effects/Tremolo.cpp -lncurses -lfftw3 -lm
```
You may need to use the following command instead, to ensure proper includes/linking:
```bash
g++ -std=c++17 -I/opt/homebrew/opt/fftw/include -L/opt/homebrew/opt/fftw/lib -o editor main.cpp ProcessAudio.cpp effects/Effect.cpp effects/Tremolo.cpp effects/Delay.cpp effects/Chorus.cpp -lncurses -lfftw3 -lm
```

### Run
```bash
./editor
```

Make sure your '.wav' file is located inside of the '/data' folder. The program currently loads the 'testaudio_clean.wav' by defualt.


