
/**
* @author Jurne Smith
 * Red ID: 824190049
 * Prof. Shen
 * CS480 - Operating Systems
 * Assignment 3
 * 11/1/25
 */
#include "PageTable.h"
#include <bitset>
#include <ostream>


PageTable::PageTable(std::vector<int> &ptLevelBits, int totalVpnBits) {


    this->ptLevelBits = ptLevelBits;
    this->numOfLevels = static_cast<int>(ptLevelBits.size());
    this->totalVpnBits = totalVpnBits;
    totalOffsetBits = BIT32 - totalVpnBits;
    pageSize = 1 << totalOffsetBits;
    pageTableHits = 0;
    levelVpns = new uint32_t[numOfLevels];

    int rootEntries = 1 << ptLevelBits.at(0);  // 2^level 1 bits


    root = new PageNode(rootEntries,(numOfLevels == 1));   //initialize root node, if there is only one level is the only pageTable
    totalPgTableEntries = rootEntries;

    levelShifts = new unsigned int[numOfLevels];
    levelMasks = new unsigned int[numOfLevels];


    for (int i = 0; i < numOfLevels; i++) {
        int currLevel = i;
        levelShifts[i] = findShift(ptLevelBits, currLevel);
        levelMasks[i] = findMask(ptLevelBits.at(i), levelShifts[i]);
    }


}

PageTable::~PageTable() {
    delete[] levelShifts;
    delete[] levelMasks;
    delete root;
    delete[] levelVpns;
}

Map* PageTable::searchMappedPfn(unsigned int virtAddress) {

    PageNode* currNode = root;

    for (int i = 0; i < numOfLevels; i++) {  // for each level

        int currLevel = i;
        unsigned int currVpn = extractVPNFromVirtualAddress(virtAddress, currLevel);
        levelVpns[i] = currVpn;


        if(!currNode->isLeaf) {
            if(currNode->nextLevel[currVpn] != nullptr) {
                currNode = currNode->nextLevel[currVpn];  //intermediate pageTable node, advance to next level
            }
        }
        else {
            if(currNode->mappings[currVpn] != nullptr) {
                 if(currNode->mappings[currVpn]->isValid) { // only if the mapping is valid
                     return currNode->mappings[currVpn]; //leaf pageTable, return mapping info
                 }
            }
        }

    }

    return nullptr;


}

void PageTable::insertMapForVpn2Pfn(unsigned int virtualAddress, unsigned int frame, bool isValid) {

    PageNode* currNode = root;
    for (int i = 0; i < numOfLevels; i++) {
        int currLevel = i;
        bool nextIsLeaf = (currLevel == numOfLevels - 2);
        unsigned int currVpn = extractVPNFromVirtualAddress(virtualAddress, currLevel);
        levelVpns[i] = currVpn;


        if(!currNode->isLeaf) {
            if(currNode->nextLevel[currVpn] != nullptr) {
                currNode = currNode->nextLevel[currVpn];
            }
            else {

                unsigned int tableEntries = 1u << ptLevelBits.at(i+1);  // 2^nextLevel bits
                currNode->nextLevel[currVpn] = new PageNode(tableEntries, nextIsLeaf);   //create a new page table at path
                totalPgTableEntries += tableEntries;
                currNode = currNode->nextLevel[currVpn];  // follow path
            }
        }
        else {

            if(isValid) {   // if we are inserting a mapping
                currNode->mappings[currVpn] = new Map(frame, virtualAddress,  true); //insert a new mapping

            }
            else {   // if we are invalidating a mapping
                currNode->mappings[currVpn] = new Map(frame, virtualAddress,  false);
                //currNode->mappings[currVpn]->isValid = false;

            }
        }

    }

}

unsigned int PageTable::extractOffset(unsigned int virtualAddress) {
    return (virtualAddress & findMask(totalOffsetBits, 0));

}
unsigned int PageTable::extractVPNFromVirtualAddress(unsigned int virtualAddress, int currLevel) {
    return (virtualAddress & levelMasks[currLevel]) >> levelShifts[currLevel];
}

unsigned int PageTable::findShift(std::vector<int> &ptLevelBits, int level) {

    int shift = 32;  //start from end bits

    for(int i = 0; i <= level; ++i) {
        shift -= ptLevelBits.at(i);
    }
    return shift;
}

unsigned int PageTable::findMask(int bits, unsigned int shift) {

    unsigned int mask = ((1 << bits) - 1) << shift;

    return mask;

}



