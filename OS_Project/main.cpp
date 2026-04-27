#include "Settings.h"
#include "FileReader.h"
#include <iostream>
#include <unordered_map>
#include <deque>

using namespace std;

int main() {
    cout << "--- Testing Step 2: FileReader ---\n\n";

    Settings mySettings = FileReader::loadSettings("config.txt");

    cout << "[Loaded Settings]\n";
    cout << "RAM Size: " << mySettings.ramMemorySize << "\n";
    cout << "Page Size: " << mySettings.sizeOfOnePage << "\n";
    cout << "TLB Size: " << mySettings.quickCacheSize << "\n";
    cout << "TLB Delay: " << mySettings.quickCacheDelay << "\n";
    cout << "RAM Delay: " << mySettings.ramDelay << "\n";
    cout << "Disk Delay: " << mySettings.hardDriveDelay << "\n\n";

    cout << "[Testing Future Timeline (OPT Prep)]\n";

    unsigned int shift = mySettings.getShiftAmount();
    unordered_map<unsigned int, deque<int>> futureMap = FileReader::lookIntoFuture("trace.txt", shift);

    for (auto const& pair : futureMap) {
        cout << "Page Number " << pair.first << " is used at steps: ";
        for (int step : pair.second) {
            cout << step << " ";
        }
        cout << "\n";
    }

    return 0;
}