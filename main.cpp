#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>

#include "ProcessAudio.h"
#include "effects/Effect.h"
#include "effects/Tremolo.h"
//#include "Visualizer.h" 

using namespace std;

// 1. 将效果器栈伪装成单一 Effect
class EffectStack : public Effect {
private:
    vector<unique_ptr<Effect>> stack;

public:
    void addEffect(unique_ptr<Effect> effect) {
        stack.push_back(std::move(effect));
    }

    float process(float sample) override {
        float currentSample = sample;
        for (auto& effect : stack) {
            currentSample = effect->process(currentSample);
        }
        return currentSample;
    }

    bool isEmpty() const {
        return stack.empty();
    }
};

// 2. 防呆输入清洗库 
int getValidInt(const string& prompt) {
    int choice;
    while (true) {
        cout << prompt;
        if (cin >> choice) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            return choice;
        } else {
            cout << "[Error] Invalid input. Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

float getValidFloat(const string& prompt) {
    float val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        } else {
            cout << "[Error] Invalid input. Please enter a valid decimal number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string getValidShape(const string& prompt) {
    string shape;
    while (true) {
        cout << prompt;
        cin >> shape;
        for (char &c : shape) c = tolower(c);
        if (shape == "sine" || shape == "triangle" || shape == "square") {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return shape;
        }
        cout << "[Error] Invalid shape. Choose from 'sine', 'triangle', 'square'.\n";
    }
}

// 3. 核心 CLI 循环
int main() {
    cout << "=== Audio Effects Engine CLI ===\n";
    
    ProcessAudio engine("testaudio_clean.wav");
    float sampleRate = engine.getSampleRate();
    
    if (sampleRate <= 0) {
        cout << "[Fatal Error] Failed to load audio or sample rate is invalid.\n";
        return 1;
    }

    EffectStack myStack;
    bool keepRunning = true;

    while (keepRunning) {
        cout << "\n--- Main Menu ---\n"
             << "1. Add Tremolo Effect\n"
             << "2. Process Audio & Save\n"
             << "3. Exit\n";
             
        int choice = getValidInt("Enter your choice (1-3): ");

        switch (choice) {
            case 1: {
                cout << "\n[Configuring Tremolo]\n";
                float rate = getValidFloat("Enter LFO Rate (Hz, e.g., 5.0): ");
                float depth = getValidFloat("Enter Depth (0.0 to 1.0): ");
                string shape = getValidShape("Enter LFO Shape (sine/triangle/square): ");
                
                myStack.addEffect(make_unique<Tremolo>(rate, depth, shape, sampleRate));
                cout << "-> Tremolo added to the effects stack.\n";
                break;
            }
            case 2: {
                if (myStack.isEmpty()) {
                    cout << "[Warning] Your effects stack is empty! Add an effect first.\n";
                    break;
                }
                cout << "\nProcessing audio... Please wait.\n";
                
                engine.processFile(myStack); 
                
                engine.writeFile("output.wav");
                cout << "-> Success! Saved to 'output.wav'.\n";
                keepRunning = false; 
                break;
            }
            case 3: {
                cout << "Exiting...\n";
                keepRunning = false;
                break;
            }
            default:
                cout << "[Error] Please choose a valid menu option.\n";
        }
    }

    return 0;
}