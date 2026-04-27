#pragma once
#include "Settings.h"
#include <string>
#include <unordered_map>
#include <deque>

using namespace std;

class FileReader {
public:
    static Settings loadSettings(const string& filename);
    static unordered_map<unsigned int, deque<int>> lookIntoFuture(string filename, unsigned int shiftAmount);
};
