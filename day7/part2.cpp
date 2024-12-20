#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>
#include <cmath>
#include <cstdint>
#include <atomic>
#include <thread> 

uint64_t concatNums(uint64_t firstNum, uint64_t secondNum) {
    uint64_t numDigits = std::log10(secondNum) + 1;
    return firstNum * static_cast<uint64_t>(std::pow(10, numDigits)) + secondNum;
}

bool testIfPossible(const std::vector<uint64_t>& numbers, uint64_t result, uint64_t index, uint64_t current) {
    if (index == numbers.size()) 
        return current == result;

    if (testIfPossible(numbers, result, index + 1, current + numbers[index]))
        return true;

    if (testIfPossible(numbers, result, index + 1, current * numbers[index]))
        return true;

    if (testIfPossible(numbers, result, index + 1, concatNums(current, numbers[index])))
        return true;
    
    return false;
}

uint64_t getCalibrationResult(const std::vector<std::vector<uint64_t>>& numbers) {
    std::atomic<uint64_t> result(0);
    
    auto processChunk = [&](size_t start, size_t end) {
        uint64_t localResult = 0;
        for (size_t i = start; i < end; i++) {
            const auto &numLine = numbers[i];
            uint64_t res = numLine[0];  
            uint64_t current = numLine[1];
            
            if (testIfPossible(numLine, res, 2, current)) 
                localResult += res;
        }
        result += localResult;
    };
    
    size_t numThreads = std::thread::hardware_concurrency();
    size_t chunkSize = numbers.size() / numThreads;
    std::vector<std::thread> threads;

    for (size_t i = 0; i < numThreads; i++) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? numbers.size() : start + chunkSize;
        threads.push_back(std::thread(processChunk, start, end));
    }

    for (auto& t : threads) {
        t.join();
    }

    return result.load();
}
int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    std::vector<std::vector<uint64_t>> numbers;
    std::string line;

    std::vector<std::string> lines;
    while(std::getline(file, line)) {
        lines.push_back(line);
    }
    
    for (auto line: lines) {
        if (line.empty())
            continue;

        std::vector<uint64_t> numLine;

        int indexFirst = line.find(":");
        if (indexFirst == std::string::npos) {
            continue;
        }
        uint64_t number = std::stoull(line.substr(0, indexFirst));
        numLine.push_back(number);

        std::istringstream ss(line.substr(indexFirst + 1));
        uint64_t num;
        while (ss >> num) {
            numLine.push_back(num);
        }
        numbers.push_back(numLine);
    }
    uint64_t totalResult = getCalibrationResult(numbers);
    std::println("Total Calibration Result: {}", totalResult);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}
