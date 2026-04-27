#include "Settings.h"
#include "ScoreBoard.h"
#include <iostream>

using namespace std;

int main() {
    Settings mySettings;

    mySettings.ramMemorySize = 262144;
    mySettings.sizeOfOnePage = 4096;
    mySettings.quickCacheSize = 16;
    mySettings.quickCacheDelay = 1.0;
    mySettings.ramDelay = 100.0;
    mySettings.hardDriveDelay = 10000000.0;

    cout << "RAM Size: " << mySettings.ramMemorySize << "\n";
    cout << "Page Size: " << mySettings.sizeOfOnePage << "\n";
    cout << "Calculated Shift: " << mySettings.getShiftAmount() << "\n";
    cout << "Calculated Frames: " << mySettings.getTotalRamFrames() << "\n\n";

    ScoreBoard myScoreboard;

    myScoreboard.totalOperations = 1000;
    myScoreboard.quickCacheHits = 850;
    myScoreboard.ramHits = 100;
    myScoreboard.misses = 50;
    myScoreboard.hardDriveWrites = 12;
    myScoreboard.totalTimePassed = 500000000;

    myScoreboard.showFinalScore();

    return 0;
}