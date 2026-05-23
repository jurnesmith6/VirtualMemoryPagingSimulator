/**
* @author Jurne Smith
 * Red ID: 824190049
 * Prof. Shen
 * CS480 - Operating Systems
 * Assignment 3
 * 11/1/25
 */

#ifndef PAGETRACKER_H
#define PAGETRACKER_H
#include <vector>

/**
* PageTracker keeps track of the frames and page replacement algorithm
*/
class PageTracker {

public:

    /**
    * @struct Frames represents the individual page frames in physical memory
    */
    struct Frames {
        unsigned int va;
        unsigned int pfn;
        unsigned int bitString;
        int lastTimeAccessed;

        Frames(unsigned int va,unsigned int pfn,unsigned int bitString,int lastTimeAccessed): va(va), pfn(pfn), bitString(bitString), lastTimeAccessed(lastTimeAccessed) {}
    };

    std::vector<Frames> loadedPages;
    int numPages;
    int nfuPeriod;
    int clock;
    std::vector<unsigned int> recentlyAccessed;

    explicit PageTracker(int nfuPeriod);
    /**
    * track() tracks recently accessed frames and updates lastTimeAccessed for each frame when accesses
    */
    void track(unsigned int pfn);
    /**
    * findVictimVa() finds the frame that will be replaced when a page replacement is neded
    */
    unsigned int* findVictimVa();
    /**
    * updateBitstrings() updates each frames bitstring value every nfu period
    */
    void updateBitstrings();



};



#endif //PAGETRACKER_H
