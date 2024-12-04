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
            // we only need to check down and diagonals up until the 3rd line from the end
            if (lines.size() - l > 3) {
                // Only check rays that go down and diagonal to the right
                if (c < 3) {
                    std::string lineDown = std::string(1,lines[l][c]) + lines[l+1][c] + lines[l+2][c] + lines[l+3][c];
                    std::string lineDiagonalRight = std::string(1, lines[l][c]) + lines[l+1][c+1] + lines[l+2][c+2] + lines[l+3][c+3];
                    if (lineDown == "XMAS" || lineDown == "SAMX")
                        xmasCounter++;
                    
                    if (lineDiagonalRight == "XMAS" || lineDiagonalRight == "SAMX")
                        xmasCounter++;
                }
                // Only check rays that go down and diagonal to the left
                if (c > lines[l].size() - 4) {
                    std::string lineDown = std::string(1,lines[l][c]) + lines[l+1][c] + lines[l+2][c] + lines[l+3][c];
                    std::string lineDiagonalLeft = std::string(1, lines[l][c]) + lines[l+1][c-1] + lines[l+2][c-2] + lines[l+3][c-3];
                    if (lineDown == "XMAS" || lineDown == "SAMX")
                        xmasCounter++;

                    if (lineDiagonalLeft == "XMAS" || lineDiagonalLeft == "SAMX")
                        xmasCounter++;
                }

                // In this case check rays that go down, diagonal to the right and left
                if (c <= lines[l].size() - 4 && c >= 3) {
                    std::string lineDown = std::string(1,lines[l][c]) + lines[l+1][c] + lines[l+2][c] + lines[l+3][c];
                    std::string lineDiagonalRight = std::string(1, lines[l][c]) + lines[l+1][c+1] + lines[l+2][c+2] + lines[l+3][c+3];
                    std::string lineDiagonalLeft = std::string(1, lines[l][c]) + lines[l+1][c-1] + lines[l+2][c-2] + lines[l+3][c-3];
                    if (lineDown == "XMAS" || lineDown == "SAMX")
                        xmasCounter++;

                    if (lineDiagonalRight == "XMAS" || lineDiagonalRight == "SAMX")
                        xmasCounter++;

                    if (lineDiagonalLeft == "XMAS" || lineDiagonalLeft == "SAMX")
                        xmasCounter++;

                }
            }
            // check if horizontally the text says xmas or samx up until 4 chars before the edge
            if (c < lines[l].size() - 3) {
                std::string ex = lines[l].substr(c, 4);
                if (ex == "XMAS" || ex == "SAMX")
                    xmasCounter++;
                
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

