#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>

const int dirsx[] = {-1, 1, 0, 0};
const int dirsy[] = {0, 0, -1, 1};

bool inBounds(int x, int y, int rows, int cols) {
    return x >= 0 && y >= 0 && x < rows && y < cols;
}

int findDistinctPaths(int x, int y, int currentValue, const std::vector<std::vector<int>>& grid, std::vector<std::vector<bool>>& visited) {
    int rows = grid.size();
    int cols = grid[0].size();
    int pathCount = 0;

    if (grid[x][y] == 9) {
        return 1;
    }

    visited[x][y] = true;

    for (int d = 0; d < 4; d++) {
        int nextx = x + dirsx[d];
        int nexty = y + dirsy[d];

        if (inBounds(nextx, nexty, rows, cols) && !visited[nextx][nexty] && grid[nextx][nexty] == currentValue + 1) {
            pathCount += findDistinctPaths(nextx, nexty, grid[nextx][nexty], grid, visited);
        }
    }

    visited[x][y] = false;

    return pathCount;
}
 
std::string getGradientColor(int value) {
    int red = 0, green = 0, blue = 0;

    if (value <= 4) {
        red = static_cast<int>(255 * value / 4.0); 
        green = 255;                              
    } else {
        red = 255;                               
        green = static_cast<int>(255 * (9 - value) / 5.0); 
    }

    return "\033[38;2;" + std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m";
}

uint64_t trailheadScore(const std::vector<std::vector<int>> &map) {
    uint64_t score = 0;
    std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map[0].size(), false));
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == 0) {
                score += findDistinctPaths(i, j, 0, map, visited); 
            }
        }
    }
    
    return score;
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
    
    std::vector<std::vector<int>> map; 
    std::string line;
    int lineCounter = 0;
    while(getline(file, line)) {
        std::vector<int> row;
        for (char c: line) {
            row.push_back(c - '0');
        }
        map.push_back(row);
    }

    for(auto line: map) {
        for (auto c: line) {
            std::print("{}{}\033[0m",getGradientColor(c), c);
        }
        std::println();
    }

    std::println("Score: {}", trailheadScore(map)); 
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}
