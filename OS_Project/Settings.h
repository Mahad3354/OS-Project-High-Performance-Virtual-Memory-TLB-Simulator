#pragma once
#include <cmath>
#include <string>

using namespace std;

struct Settings {
    int ramMemorySize;
    int sizeOfOnePage;
    int quickCacheSize;

    double quickCacheDelay;
    double ramDelay;
    double hardDriveDelay;

    int getShiftAmount() const {
        return log2(sizeOfOnePage);
    }

    int getOffsetMask() const {
        return sizeOfOnePage - 1;
    }

    int getTotalRamFrames() const {
        return ramMemorySize / sizeOfOnePage;
    }
};
