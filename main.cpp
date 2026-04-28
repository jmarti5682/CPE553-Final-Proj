#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>

#include "ProcessAudio.h"
#include "effects/Effect.h"
#include "effects/Tremolo.h"
#include "effects/Delay.h"
#include "effects/Chorus.h"
#include "Visualizer.h" // 依然保持注释，除非队友已经写好了这个模块

using namespace std;

// 1. 将效果器栈伪装成单一 Effect (组合模式)
class EffectStack : public Effect
{
private:
    vector<unique_ptr<Effect>> stack;

public:
    void addEffect(unique_ptr<Effect> effect)
    {
        stack.push_back(std::move(effect));
    }

    float process(float sample) override
    {
        float currentSample = sample;
        for (auto &effect : stack)
        {
            currentSample = effect->process(currentSample);
        }
        return currentSample;
    }

    bool isEmpty() const
    {
        return stack.empty();
    }
};

// 2. 防呆输入清洗库
int getValidInt(const string &prompt)
{
    int choice;
    while (true)
    {
        cout << prompt;
        if (cin >> choice)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
        else
        {
            cout << "[Error] Invalid input. Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

float getValidFloat(const string &prompt)
{
    float val;
    while (true)
    {
        cout << prompt;
        if (cin >> val)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        else
        {
            cout << "[Error] Invalid input. Please enter a valid decimal number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string getValidShape(const string &prompt)
{
    string shape;
    while (true)
    {
        cout << prompt;
        cin >> shape;
        for (char &c : shape)
            c = tolower(c);
        if (shape == "sine" || shape == "triangle" || shape == "square")
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return shape;
        }
        cout << "[Error] Invalid shape. Choose from 'sine', 'triangle', 'square'.\n";
    }
}

// 3. 核心 CLI 循环
int main()
{
    cout << "=== Audio Effects Engine CLI ===\n";
    ProcessAudio engine("synth_test.wav");
    float sampleRate = engine.getSampleRate();
    string currentLabel = "Synth (synth_test.wav)";
    string currentFile = "synth_test.wav";

    if (sampleRate <= 0)
    {
        cout << "[Fatal Error] Failed to load audio or sample rate is invalid.\n";
        return 1;
    }

    EffectStack myStack;
    bool keepRunning = true;

    while (keepRunning)
    {
        cout << "\n--- Main Menu ---\n"
             << "1. Select Audio File\n"
             << "2. Add Tremolo Effect\n"
             << "3. Add Delay Effect\n"
             << "4. Add Chorus Effect\n"
             << "5. Process Audio & Save\n"
             << "6. Visualize Audio & Play\n"
             << "7. Exit\n";

        int choice = getValidInt("Enter your choice (1-7): ");

        switch (choice)
        {
        case 1:
        {
            cout << "\n--- Scanning 'data' directory ---\n";
            vector<string> wavFiles;
            
            // 1. 物理防爆：探测文件夹是否存在
            if (!std::filesystem::exists("data") || !std::filesystem::is_directory("data")) {
                cout << "[Fatal Error] 'data' directory not found! Please create a folder named 'data' and put .wav files inside.\n";
                break;
            }

            // 2. 动态扫描：只提取 .wav 结尾的文件，并且只保留纯文件名
            for (const auto& entry : std::filesystem::directory_iterator("data")) {
                if (entry.path().extension() == ".wav") {
                    // 致命修改：使用 .filename() 剥离路径，只保留 "PeizhiLiu-test.wav"
                    wavFiles.push_back(entry.path().filename().string()); 
                }
            }

            // 3. 业务防呆：文件夹里没有音频怎么办？
            if (wavFiles.empty()) {
                cout << "[Error] No .wav files found in the 'data' directory.\n";
                break;
            }

            // 4. 动态生成菜单
            for (size_t i = 0; i < wavFiles.size(); ++i) {
                cout << i + 1 << ". " << wavFiles[i] << "\n";
            }

            // 5. 安全捕获用户输入
            int fileChoice = getValidInt("Select a file by number (1-" + to_string(wavFiles.size()) + "): ");
            
            if (fileChoice >= 1 && fileChoice <= static_cast<int>(wavFiles.size())) {
                string selectedPath = wavFiles[fileChoice - 1];
                
                // 加载选中的文件
                engine.loadFile(selectedPath);
                currentFile = selectedPath;
                currentLabel = "Custom (" + selectedPath + ")";
                
                cout << "-> Successfully loaded '" << selectedPath << "' for processing.\n";
                
                // 重要防呆：如果用户换了新歌，必须清空之前的效果器栈，否则特效会跨歌曲叠加！
                myStack = EffectStack(); 
                cout << "-> [Notice] Effects stack has been reset for the new audio file.\n";
            } else {
                cout << "[Error] Invalid selection. Out of range.\n";
            }
            break;
        }
        case 2:
        {
            cout << "\n[Configuring Tremolo]\n";
            float rate = getValidFloat("Enter LFO Rate (Hz, e.g., 5.0): ");
            float depth = getValidFloat("Enter Depth (0.0 to 1.0): ");
            string shape = getValidShape("Enter LFO Shape (sine/triangle/square): ");

            myStack.addEffect(make_unique<Tremolo>(rate, depth, shape, sampleRate));
            cout << "-> Tremolo added to the effects stack.\n";
            break;
        }
        case 3:
        {
            cout << "\n[Configuring Delay]\n";
            float time = getValidFloat("Enter Delay Time (in seconds, e.g., 0.5): ");
            float mix = getValidFloat("Enter Mix level (0.0 to 1.0, 0.5 is half-half): ");
            float feedback = getValidFloat("Enter Feedback amount (0.0 to 1.0): ");

            myStack.addEffect(make_unique<Delay>(time, mix, feedback, static_cast<int>(sampleRate)));
            cout << "-> Delay added to the effects stack.\n";
            break;
        }
        case 4:
        {
            cout << "\n[Configuring Chorus]\n";
            float time = getValidFloat("Enter Base Delay Time (in seconds, e.g., 0.015): ");
            float mix = getValidFloat("Enter Mix level (0.0 to 1.0): ");
            float feedback = getValidFloat("Enter Feedback amount (0.0 to 1.0): ");
            float rate = getValidFloat("Enter LFO Rate (Hz, e.g., 1.5): ");
            float depth = getValidFloat("Enter LFO Depth (e.g., 0.002): ");

            myStack.addEffect(make_unique<Chorus>(time, mix, feedback, rate, depth, sampleRate));
            cout << "-> Chorus added to the effects stack.\n";
            break;
        }
    
        case 5:
        {
            if (myStack.isEmpty())
            {
                cout << "[Warning] Your effects stack is empty! Add an effect first.\n";
                break;
            }
            cout << "\nProcessing audio... Please wait.\n";

            engine.processFile(myStack);

            string outName;
            cout << "Enter the name for your output file (e.g., my_awesome_track.wav): ";
            cin >> outName;

            //如果用户忘记打 .wav，系统自动加上
            if (outName.length() < 4 || outName.substr(outName.length() - 4) != ".wav") {
                outName += ".wav";
            }

            engine.writeFile(outName);
            cout << "[SYSTEM] Success! Saved to '" << outName << "'.\n";
            cout << "[SYSTEM] (Visualization module is currently disabled for Windows OS compatibility)\n";
            
            keepRunning = false; // 导出后退出程序
            break;
        }
        case 6:
        {
            visualize(engine, currentLabel, currentFile);
            break;
        }
        case 7:
        {
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
