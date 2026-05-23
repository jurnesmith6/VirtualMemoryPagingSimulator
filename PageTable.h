/**
* @author Jurne Smith
 * Red ID: 824190049
 * Prof. Shen
 * CS480 - Operating Systems
 * Assignment 3
 * 11/1/25
 */

#ifndef PAGETABLE_H
#define PAGETABLE_H
#include <vector>
#include "PageNode.h"
#define BIT32 32

/**
* @class PageTable represents the structure for the N level pageTable
*/

class PageTable {

public:

    int maxFrames;
    int pageSize;
    int numOfFramesAllocated;
    int numOfPageReplacements;
    int pageTableHits;
    int numOfAddresses;
    unsigned int totalPgTableEntries;
    std::vector<int> ptLevelBits;
    int totalVpnBits;
    int totalOffsetBits;
    int numOfLevels;
    PageNode* root;
    unsigned int* levelMasks;
    unsigned int* levelShifts;
    uint32_t* levelVpns;
    unsigned int vpn;
    unsigned int offSet;

    PageTable(std::vector<int> &ptLevelBits, int totalVpnBits);
    ~PageTable();

    /**
    * extractVPNFromVirtualAddress() extracts the vpn of a current level
    */
    unsigned int extractVPNFromVirtualAddress(unsigned int virtualAddress, int level);

    /**
    * findShift() calculates the necessary shift needed to extract a vpn at a specific level
    */
    unsigned int findShift(std::vector<int> &ptLevelBits, int level);

    /**
    * findMask() calculates the necessary mask needed to extract a vpn at a specific level
    */
    unsigned int findMask(int bits, unsigned int shift);
    /**
    * extractOffset() calculates the offset of a virtual address
    */
    unsigned int extractOffset(unsigned int virtualAddress);
    /**
    * searchMappedPfn() searches the page table for a specific page mapping
    * returns nullpt if no mapping is found, returns Map* containing page info if there is a valid find
    */
    Map* searchMappedPfn(unsigned int virtualAddress);
    /**
    * insertMapForVpn2Pfn() adds a page to the page table or invalidates a page currently in the table due to a page replacement
    */
    void insertMapForVpn2Pfn(unsigned int virtualAddress, unsigned int frame, bool isValid);



};



#endif //PAGETABLE_H
