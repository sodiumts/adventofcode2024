#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <cmath>


bool safe_numbers(std::vector<int> &numbers) {
    bool lastGoingDown = false;
    for (int i = 0; i < numbers.size() - 1; i++) {
        int difference = numbers[i] - numbers[i + 1];
        int goingDown = difference < 0; 

        if (i != 0) {
            if (lastGoingDown != goingDown) {
                if (i == numbers.size() - 2) {
                    numbers.pop_back();
                } else {
                    numbers.erase(numbers.begin() + i);
                }
                return false;
            }
            lastGoingDown = goingDown;
        } else {
            lastGoingDown = goingDown;
        }

        if (std::abs(difference) > 3 || std::abs(difference) < 1) {
            if (i == numbers.size() - 2) {
                numbers.pop_back();
            } else {
                numbers.erase(numbers.begin() + i);
            }
            return false;
        }
    }


    return true;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file");
    }
    
    std::vector<std::string> lines;
    std::string line;
    while(std::getline(file, line)) {
        lines.push_back(line);
    }
    
    int safeCounter = 0;

    for (const std::string &l : lines) {
        std::istringstream stream(l);
        std::vector<std::string> split(
           (std::istream_iterator<std::string>(stream)),
           std::istream_iterator<std::string>()
        );

        std::vector<int> nums;
        nums.reserve(split.size());

        std::transform(split.begin(), split.end(), std::back_inserter(nums), 
                        [](const std::string &token) { return std::stoi(token); }
                       );
        
        if (safe_numbers(nums)) {
            for (int num: nums) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
            safeCounter++;
        } else {
           if(safe_numbers(nums)) {
               for (int num: nums) {
                   std::cout << num << " ";
               }
               std::cout << std::endl;
               safeCounter++;
           }
        }
    }
     
        
    std::cout << "Safe Reports: " << safeCounter << std::endl;
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " nanoseconds = " << duration.count() / 1e6 << " milliseconds "<< std::endl;
    file.close();
    return 0;
}

