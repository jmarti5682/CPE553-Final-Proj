CXX      = g++
CXXFLAGS = -std=c++17 -Wall -I/opt/homebrew/include -Ilibraries
LDFLAGS  = -L/opt/homebrew/lib -lfftw3 -lncurses

TARGET = audio_editor

SRCS = main.cpp \
       ProcessAudio.cpp \
       Visualizer.cpp \
       effects/Effect.cpp \
       effects/Delay.cpp \
       effects/Tremolo.cpp \
       effects/Chorus.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
