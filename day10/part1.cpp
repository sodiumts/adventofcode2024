#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>
#include <queue>

const int dirsx[] = {-1, 1, 0, 0};
const int dirsy[] = {0, 0, -1, 1};

bool inBounds(int x, int y, int rows, int cols) {
    return x >= 0 && y >= 0 && x < rows && y < cols;
}

int countPaths(int startX, int startY, const std::vector<std::vector<int>>& map) {
    int rows = map.size();
    int cols = map[0].size();
    int pathCount = 0;

    std::queue<std::tuple<int, int, int>> q;
    q.push({startX, startY, 0});

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    visited[startX][startY] = true;

    while (!q.empty()) {
        auto [x, y, currentValue] = q.front(); // I love tuple unpacking
        q.pop();

        if (map[x][y] == 9) {
            pathCount++;
            continue; 
        }
        for (int d = 0; d < 4; d++) {
            int nextx = x + dirsx[d];
            int nexty = y + dirsy[d];

            if (inBounds(nextx, nexty, rows, cols) && !visited[nextx][nexty] && map[nextx][nexty] == currentValue + 1) {
                visited[nextx][nexty] = true;
                q.push({nextx, nexty, map[nextx][nexty]});
            }
        }
    }

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
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == 0) {
                score += countPaths(i, j, map); 
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
