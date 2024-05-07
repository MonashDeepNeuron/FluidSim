#pragma once

#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

class timer {
private:
    int low;
    int high;
    std::ofstream file;
    std::pair<int, int> ave;
    high_resolution_clock::time_point start_time; // Added this line
public:
    timer(auto name){
        // Create folder for output - helps with gitignore.
        std::string folder = "./output";
        if (!std::filesystem::exists(folder)) {
            std::filesystem::create_directory(folder);
        }
        // Add file to output folder.
        std::string filepath = folder + '/' + std::string(name) + ".txt";
        file.open(filepath, std::ios_base::out);

        ave = {0, 0};
        low = std::numeric_limits<int>::max();
        high = 0;
    }

    void start(){
        start_time= high_resolution_clock::now();
    }
    void stop(){
        auto stop = high_resolution_clock::now();
        auto d = std::chrono::duration_cast<std::chrono::microseconds>(stop - start_time).count();
        int duration = static_cast<int>(d);
        file << "Time taken by function: " << duration << " microseconds\n";
        if (duration > high){
            high = duration;
        }
        else if (duration < low){
            low = duration;
        }
        ave.first += 1;
        ave.second += duration;
    }
    void finish(){
        file << "High: "<< high << '\n';
        file << "Low: " << low << '\n';
        file << "Average: " << static_cast<float>((ave.second/(ave.first))) << '\n';
        file.close();
    }

};