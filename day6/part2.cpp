#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <unordered_set>
#include <functional>
#include <tuple>

struct tuple_hash {
    template <class T1, class T2, class T3>
    std::size_t operator()(const std::tuple<T1, T2, T3>& tuple) const {
        auto h1 = std::hash<T1>{}(std::get<0>(tuple));
        auto h2 = std::hash<T2>{}(std::get<1>(tuple));
        auto h3 = std::hash<T3>{}(std::get<2>(tuple));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
struct Guard {
    int x;
    int y;
    int rotation; // 0, 1, 2, 3
};
bool testBlock(std::vector<std::string> &map, struct Guard &guard){
    std::unordered_set<std::tuple<int, int, int>, tuple_hash> visitedPositions;
    while(true) {
        std::pair<int, int> nextPosition;
        std::tuple<int, int, int> currentPosition = std::make_tuple(guard.x, guard.y, guard.rotation);
        if(visitedPositions.find(currentPosition) != visitedPositions.end()) {
            return true;
        }
        visitedPositions.insert(currentPosition);
        switch(guard.rotation) {
            case 0:
                nextPosition.first = guard.x;
                nextPosition.second = guard.y - 1;
            break;
            case 1:
                nextPosition.first = guard.x + 1;
                nextPosition.second = guard.y;
            break;
            case 2:
                nextPosition.first = guard.x;
                nextPosition.second = guard.y + 1;
            break;
            case 3:
                nextPosition.first = guard.x - 1;
                nextPosition.second = guard.y;
            break;
        }
        if(nextPosition.first < 0 || nextPosition.first >= map[0].size() || nextPosition.second < 0 || nextPosition.second >= map.size()) {
            break;
        }

        char nextChar = map[nextPosition.second][nextPosition.first];
        if(nextChar == '.') {
            guard.x = nextPosition.first;
            guard.y = nextPosition.second;
        }  else {
            guard.rotation = (guard.rotation + 1) % 4;
        }
    }
    return false;
}


int mapOutPath(std::vector<std::string> &map, struct Guard &guard) {
    int count = 0;
    for (int i = 0; i < map.size(); i++) {
        for(int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == '.') {
                std::vector<std::string> mapCopy(map);
                mapCopy[i][j] = '#';
                struct Guard guardCopy = guard;
                if(testBlock(mapCopy, guardCopy)) {
                    count++;
                }
            }
        }
    }


    return count;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file");
    }
    
    std::vector<std::string> map;
    std::string line;
    struct Guard guard = { 0, 0, 0 }; 
    int i = 0;
    while(std::getline(file, line)) {
        map.push_back(line);        
        size_t guardIndex = 0;
        if((guardIndex = line.find('^', guardIndex)) != std::string::npos) {
            guard.x = guardIndex;
            guard.y = i;
            map[guard.y][guard.x] = '.';
        }
        i++;
    }
    
        
    std::println("VisitedPositions: {0}", mapOutPath(map, guard));

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}

