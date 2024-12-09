#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>
#include <queue>


std::queue<int> getFreeSpaces(const std::vector<int> &diskMap) {
    std::queue<int> freeSpaces;
    bool isFreeSpace = false;
    int bigIndex = 0;
    for (int i = 0; i < diskMap.size(); i++) {
        if(!isFreeSpace) {
            bigIndex += diskMap[i];
            isFreeSpace = true;
        } else {
            for (int j = 0; j < diskMap[i]; j++) {
                freeSpaces.push(bigIndex + j);
            }
            bigIndex += diskMap[i];
            isFreeSpace = false;
        }
    }
    return freeSpaces;
}

bool hasGap(const std::vector<int> &disk) {
    bool hadOne = false;
    for (int i = 0; i < disk.size(); i++) {
        if(disk[i] == -1) {
            hadOne = true;
        }
        if(hadOne && disk[i] != -1) {
            return true;
        }
    }
    return false;

}

std::queue<int> getFreeSpaces2(const std::vector<int> &disk) {
    std::queue<int> freeSpaces;

    for (int i = 0; i < disk.size(); i++) {
        if(disk[i] == -1) {
            freeSpaces.push(i);
        }   
    }
    return freeSpaces;
}

uint64_t calculateChecksum(const std::vector<int> &diskMap) {
    auto freeSpaces = getFreeSpaces(diskMap);
    
    std::vector<int> diskNums;
    bool isFile = true;
    int fileIDCounter = 0;
    for (int i = 0; i < diskMap.size(); i++) {
       if(isFile) {
            for (int j = 0; j < diskMap[i]; j++) {
                diskNums.push_back(fileIDCounter);
            }
            fileIDCounter++;
            isFile = false;
       } else {
            for (int j = 0; j < diskMap[i]; j++) {
                diskNums.push_back(-1);
            }
            isFile = true;
       }
    }
    std::vector<int> freeSpace;
    for(int i = diskNums.size() - 1; i >= 0; i--) {
        if(diskNums[i] != -1 && freeSpaces.size() > 0) {
            diskNums[freeSpaces.front()] = diskNums[i];
            freeSpaces.pop();
            diskNums[i] = -1;
            if(hasGap(diskNums)) {
                freeSpaces = getFreeSpaces2(diskNums);
            } else {
                break;
            }
        } 
    }
    uint64_t checksum = 0;
    for (int i = 0; i < diskNums.size(); i++) {
        if(diskNums[i] == -1) {
            break;
        }
        checksum += i * diskNums[i];
    }
    return checksum;
}
int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
    
    
    std::vector<int> diskNum;
    std::string line;
    while(getline(file, line)) {
       for (char c: line) {
            diskNum.push_back(c - '0');
        } 
    }

    std::println("Checksum: {}", calculateChecksum(diskNum)); 
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}
