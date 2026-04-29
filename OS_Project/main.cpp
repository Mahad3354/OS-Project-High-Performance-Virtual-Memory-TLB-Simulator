#include <iostream>
#include <unordered_map>
#include <deque>
#include "PageReplacer.h"

using namespace std;

int main() {
    cout << "--- Testing FIFO ---\n";
    SimpleFIFO fifo;
    fifo.markAsUsed(1);
    fifo.markAsUsed(2);
    fifo.markAsUsed(3);

    cout << "Expected Victim: 1\n";
    cout << "Actual Victim: " << fifo.kickOutVictim() << "\n\n";

    cout << "--- Testing LRU ---\n";
    SimpleLRU lru;
    lru.markAsUsed(1);
    lru.markAsUsed(2);
    lru.markAsUsed(3);
    lru.markAsUsed(1);

    cout << "Expected Victim: 2\n";
    cout << "Actual Victim: " << lru.kickOutVictim() << "\n\n";

    cout << "--- Testing OPT ---\n";
    unordered_map<unsigned int, deque<int>> futureTimeline;
    futureTimeline[1] = { 10 };
    futureTimeline[2] = { 5 };
    futureTimeline[3] = {};

    FutureOPT opt(futureTimeline);
    opt.markAsUsed(1);
    opt.markAsUsed(2);
    opt.markAsUsed(3);

    cout << "Expected Victim: 3\n";
    cout << "Actual Victim: " << opt.kickOutVictim() << "\n";

    return 0;
}