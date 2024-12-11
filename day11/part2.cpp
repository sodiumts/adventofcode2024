#include <cstdint>
#include <fstream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>
#include <unordered_map>
#include <utility>


uint64_t countDigits(uint64_t num) {
    uint64_t digits = 0;
    while (num > 0) {
        num /= 10;
        digits++;
    }
    return digits;
}

std::pair<uint64_t, uint64_t> splitNumber(uint64_t num, uint64_t numOfDigits) {
    uint64_t divisor = 1;
    for (uint64_t i = 0; i < numOfDigits / 2; i++) {
        divisor *= 10;
    }
    return {num / divisor, num % divisor};
}

uint64_t getStoneCount(std::unordered_map<uint64_t, uint64_t>& counter, int times) {
    uint64_t totalCount = 0;

    for (int i = 0; i < times; i++) {
        std::unordered_map<uint64_t, uint64_t> newCounter;
        for (auto &[key, value]: counter) {
            if (value == 0) {
                continue;
            }

            if (key == 0) {
                newCounter[1] += value; 
                continue;
            }
            if (key == 1) {
                newCounter[2024] += value;
                continue;
            }

            uint64_t digits = countDigits(key);
            if(digits % 2 == 0) {
                auto [firstHalf, secondHalf] = splitNumber(key, digits);
                newCounter[firstHalf] += value;
                newCounter[secondHalf] += value;
                continue;
            }

            newCounter[key * 2024] += value;
        }
        counter = newCounter;
    }



    for (auto &[key, value] : counter) {
        totalCount += value;
    }
    return totalCount;
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
   
    std::unordered_map<uint64_t, uint64_t> counter;
    std::string line;
    while(getline(file, line)) {
        int idxSpace = line.find(' ');
        counter[std::stoull(line.substr(0, idxSpace))]++;
        std::istringstream ss(line.substr(idxSpace + 1));
        uint64_t num;
        while(ss >> num) {
            counter[num]++;
        }
    }

    std::println("Number Of Stones: {}", getStoneCount(counter, 75)); 
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}
