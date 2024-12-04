#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>


int findxmas(const std::vector<std::string> &lines) {
    int xmasCounter = 0;
    for (int l = 1; l < lines.size() - 1; l++) {
        std::size_t indexOfA = 0;
        while((indexOfA = lines[l].find('A', indexOfA)) != std::string::npos) {
            std::string lineDown = std::string(1,lines[l-1][indexOfA-1]) + lines[l][indexOfA] + lines[l+1][indexOfA+1];
            std::string lineUp = std::string(1,lines[l-1][indexOfA+1]) + lines[l][indexOfA] + lines[l+1][indexOfA-1];
            if ((lineDown == "MAS" || lineDown == "SAM") && (lineUp == "MAS" || lineUp == "SAM"))
                xmasCounter++;
            
            indexOfA++;
        }
    }
    return xmasCounter;
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
    
    int res = findxmas(lines);
        
    std::println("Mas  count: {0}", res);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}

