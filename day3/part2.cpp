#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cmath>
#include <cctype>


bool isNum(const std::string &num) {
    for (char c: num) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

int sumResult(const std::vector<std::string> &lines) {
    int result = 0;
    for (const auto &line: lines) {
        std::size_t pos = 0;
        std::size_t doPos = 0;
        std::size_t dontPos = 0;
        bool mulEnabled = true;
        
        std::vector<int> doPositions;
        while ((doPos = line.find("do()", doPos)) != std::string::npos) {
            doPositions.push_back(doPos);
            doPos += 4;
        }

        std::vector<int> dontPositions;
        while ((dontPos = line.find("don't()", dontPos)) != std::string::npos) {
            dontPositions.push_back(dontPos);
            dontPos += 7;
        }

        std::cout << "doPos: ";
        for (int p: doPositions) {
            std::cout << p << ", ";
        }
        std::cout << std::endl;
        
        std::cout << "dontPos: ";
        for (int p: dontPositions) {
            std::cout << p << ", ";
        }
        std::cout << std::endl;

        while ((pos = line.find("mul(", pos)) != std::string::npos) {
            
            std::size_t startP = pos + 4;
            std::size_t endP = line.find(')', startP);

            std::string content = line.substr(startP, endP - startP);
            std::size_t commaP = content.find(',');
            
            if (commaP == std::string::npos) {
                //std::cout << "missing comma" << std::endl;
                pos += 4;
                continue;
            }
            std::string firstNum = content.substr(0, commaP);
            std::string secondNum = content.substr(commaP + 1);

            if (isNum(firstNum) && isNum(secondNum)) {
                int first = std::stoi(firstNum);
                int second = std::stoi(secondNum);
                
                result += first * second;
            } else {
                //std::cout << "String not num: " << firstNum << "    " << secondNum << std::endl;
            }
            pos += 4;
        }
    }
    return result;
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
    
    int res = sumResult(lines);

        
    std::cout << "mul() result: " << res << std::endl;
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " nanoseconds = " << duration.count() / 1e6 << " milliseconds "<< std::endl;
    file.close();
    return 0;
}

