# Audio Editor and Visualizer
> CPE553: Intro to C++
> 
> Jose Martinez-Ponce, Benjamin Kunze, Peizhi Liu

## IN ORDER TO MAXIMIZE FUNCTIONALITY, THIS PROGRAM HAS BEEN DESIGNED FOR USE WITH MACOS. IN ITS CURRENT FORM, THE PROGRAM DOES NOT COMPILE ON WINDOWS.

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

**Peizhi Liu -  Command Line Interface ('main.cpp')**

- Allows users to load an effect, define effect parameters, then save output audio with effect applied. 
-Integrated three effects stacks into the program's user interface.Implemented a mechanism to automatically reset the effects stacks when switching tracks.
---

## How to Run:

### Dependencies
- 'ncurses' - for terminal rendering
- 'fftw3' - for frequency analysis
- 'AudioFile' - header-only library for reading in/writing audio
- 'rch_oscillators' - basic oscillator library

**Install dependencies on macOS:**

```bash
brew install ncurses fftw
```

### Recommended: Using Makefile

The Makefile automates the compilation process with proper flags and dependencies. The Makefile does NOT install dependencies for you, so please make sure to complete the prior steps for installing dependencies. 

**Compile using Makefile:**
```bash
make
```

This will generate an executable named `audio_editor` that contains all the source files and linked libraries.

**Run the program:**
```bash
./audio_editor
```

**Clean up build artifacts:**
```bash
make clean
```

This removes all object files and the executable, allowing for a fresh rebuild.

---

### Legacy: Manual Compilation (Failsafe)

If the Makefile approach doesn't work, you can compile manually using the commands below.

**Compile (basic):**
```bash
g++ -std=c++17 -o audio_editor main.cpp ProcessAudio.cpp Visualizer.cpp effects/Effect.cpp effects/Tremolo.cpp effects/Delay.cpp effects/Chorus.cpp -lncurses -lfftw3 -lm
```

**Compile (with explicit paths for macOS Homebrew):**
```bash
g++ -std=c++17 -I/opt/homebrew/opt/fftw/include -L/opt/homebrew/opt/fftw/lib -o audio_editor main.cpp ProcessAudio.cpp Visualizer.cpp effects/Effect.cpp effects/Tremolo.cpp effects/Delay.cpp effects/Chorus.cpp -lncurses -lfftw3 -lm
```

**Run:**
```bash
./editor
```

---

### Audio Files

This program includes drum, guitar, and synthesizer sample audio files. Users can manually add their own .wav files to the "data" directory to process their own audio. Make sure any .wav files added do not contain any metadata that may be associated with the software they were exported from (like tempo, automation, markers, etc. that may be saved if bounced from a DAW).

