#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cmath>
#include <cctype>
#include <utility>
#include <print>


int findxmas(const std::vector<std::string> &lines) {
    int xmasCounter = 0;
    for (int l = 0; l < lines.size(); l++) {
        for(int c = 0; c < lines[l].size(); c++) {
            if (lines.size() - l >= 3) {
                if (c <= lines[l].size() - 3) {
                    std::string lineDiagonalRight = std::string(1, lines[l][c]) + lines[l+1][c+1] + lines[l+2][c+2];
                    std::string lineDiagonalLeft = std::string(1, lines[l][c+2]) + lines[l+1][c+1] + lines[l+2][c];
                    if ((lineDiagonalRight == "MAS" || lineDiagonalRight == "SAM") && (lineDiagonalLeft == "MAS" || lineDiagonalLeft == "SAM"))
                        xmasCounter++;

                }
            }
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
        
    std::println("xmas count: {0}", res);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}

