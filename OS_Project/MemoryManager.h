#pragma once
#include <vector>
#include <list>
#include <unordered_map>
#include "Settings.h"
#include "ScoreBoard.h"
#include "PageReplacer.h"

using namespace std;

struct PageData {
    unsigned int frameBoxNumber;
    bool isValid = false;
    bool needsSaving = false;
};

class QuickCache {
    struct CacheBox { unsigned int pageNumber; unsigned int frameBox; bool filled = false; };
    vector<CacheBox> boxes;
    unsigned int maxBoxes;
public:
    QuickCache(unsigned int size) : maxBoxes(size) {
        if (size > 0) boxes.resize(size);
    }

    int checkCache(unsigned int pageNumber) {
        if (maxBoxes == 0) return -1;
        for (auto& box : boxes) {
            if (box.filled && box.pageNumber == pageNumber) return box.frameBox;
        }
        return -1;
    }

    void saveToCache(unsigned int pageNumber, unsigned int frameBox) {
        if (maxBoxes == 0) return;
        static int nextSlot = 0;
        boxes[nextSlot] = { pageNumber, frameBox, true };
        nextSlot = (nextSlot + 1) % maxBoxes;
    }

    void deleteFromCache(unsigned int pageNumber) {
        if (maxBoxes == 0) return;
        for (auto& box : boxes) {
            if (box.filled && box.pageNumber == pageNumber) {
                box.filled = false;
            }
        }
    }
};

class MemoryManager {
    Settings mySettings;
    ScoreBoard& myScoreboard;
    QuickCache myCache;
    unordered_map<unsigned int, PageData> ramTable;
    PageReplacer* myReplacer;
    list<unsigned int> emptyRamBoxes;

    void fixMissingPage(unsigned int pageNumber) {
        myScoreboard.misses++;
        myScoreboard.totalTimePassed += mySettings.hardDriveDelay;

        unsigned int chosenRamBox;

        if (!emptyRamBoxes.empty()) {
            chosenRamBox = emptyRamBoxes.front();
            emptyRamBoxes.pop_front();
        }
        else {
            unsigned int victimPage = myReplacer->kickOutVictim();

            if (ramTable[victimPage].needsSaving) {
                myScoreboard.hardDriveWrites++;
                myScoreboard.totalTimePassed += mySettings.hardDriveDelay;
            }

            chosenRamBox = ramTable[victimPage].frameBoxNumber;
            ramTable[victimPage].isValid = false;
            myCache.deleteFromCache(victimPage);
        }

        ramTable[pageNumber] = { chosenRamBox, true, false };
    }

public:
    MemoryManager(Settings settings, ScoreBoard& board, PageReplacer* replacer)
        : mySettings(settings), myScoreboard(board), myCache(settings.quickCacheSize), myReplacer(replacer)
    {
        for (unsigned int i = 0; i < mySettings.getTotalRamFrames(); ++i) {
            emptyRamBoxes.push_back(i);
        }
    }

    void readOrWrite(unsigned int memoryAddress, bool isWritingData) {
        myScoreboard.totalOperations++;

        unsigned int pageNumber = memoryAddress >> mySettings.getShiftAmount();

        myScoreboard.totalTimePassed += mySettings.quickCacheDelay;

        int ramBox = myCache.checkCache(pageNumber);

        if (ramBox != -1) {
            myScoreboard.quickCacheHits++;
        }
        else {
            myScoreboard.totalTimePassed += mySettings.ramDelay;

            if (ramTable.count(pageNumber) && ramTable[pageNumber].isValid) {
                myScoreboard.ramHits++;
                ramBox = ramTable[pageNumber].frameBoxNumber;
                myCache.saveToCache(pageNumber, ramBox);
            }
            else {
                fixMissingPage(pageNumber);
                ramBox = ramTable[pageNumber].frameBoxNumber;
                myCache.saveToCache(pageNumber, ramBox);
            }
        }

        if (isWritingData) {
            ramTable[pageNumber].needsSaving = true;
        }

        myReplacer->markAsUsed(pageNumber);
    }
};