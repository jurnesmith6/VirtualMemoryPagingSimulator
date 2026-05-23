/**
* @author Jurne Smith
 * Red ID: 824190049
 * Prof. Shen
 * CS480 - Operating Systems
 * Assignment 3
 * 11/1/25
 */
#include "PageTracker.h"

#include <iomanip>
#include <iostream>
#include <algorithm>




PageTracker::PageTracker(int nfuPeriod): clock(0), numPages(0), nfuPeriod(nfuPeriod) {

}


void PageTracker::track(unsigned int pfn) {


   loadedPages.at(pfn).lastTimeAccessed = clock;

    auto iter = std::find(recentlyAccessed.begin(), recentlyAccessed.end(), pfn);

    if(iter == recentlyAccessed.end()) {  // if we have not already inserted into the list
        recentlyAccessed.push_back(pfn);
    }


}

void PageTracker::updateBitstrings() {

    if(clock % nfuPeriod  == 0) {

        // for all frames, normal aging update
        for(int i = 0; i < loadedPages.size(); i++) {
            loadedPages.at(i).bitString = loadedPages.at(i).bitString >> 1u;
        }

        //for all recently accessed frames, special update

        for(int i = 0; i < recentlyAccessed.size(); i++) {
            unsigned int recentIndx = recentlyAccessed.at(i);
            loadedPages.at(recentIndx).bitString += (1u << 15);
        }

        recentlyAccessed.clear(); //refresh recently accessed memory

    }

}

unsigned int* PageTracker::findVictimVa() {

    int smallest = 0;

    for(int i = 1; i < loadedPages.size(); i++) {

        if((loadedPages.at(i).bitString) < (loadedPages.at(smallest).bitString)) {
           smallest = i;
        }
    }

    int victim = smallest;
    for(int i = 0; i < loadedPages.size(); i++) {

        if(loadedPages.at(victim).bitString == loadedPages.at(i).bitString) {  // if equal to the entry with smallest bitstring

            if((loadedPages.at(victim).lastTimeAccessed) > (loadedPages.at(i).lastTimeAccessed)) {  // compare tied pages lastAccessTime
                victim = i;
            }
        }
    }

    unsigned int* victimInfo = new unsigned int[3]{loadedPages.at(victim).va, loadedPages.at(victim).pfn, loadedPages.at(victim).bitString};

    return victimInfo;


}

