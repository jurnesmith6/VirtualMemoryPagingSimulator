/**
* @author Jurne Smith
 * Red ID: 824190049
 * Prof. Shen
 * CS480 - Operating Systems
 * Assignment 3
 * 11/1/25
 */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include "log_helpers.h"
#include "PageTable.h"
#include "vaddr_tracereader.h"
#define EXITCODE 0
#define DEFAULTFRAMES 999999
#define DEFAULTPERIOD 10
#define MAXVPNBITS 28
#include "PageTracker.h"
#include <vector>

int main(int argc, char **argv) {

    int option;
    int numOfAccesses = -1;   //default
    int numOfFrames = DEFAULTFRAMES;
    int nfuPeriod = DEFAULTPERIOD;
    std::string mode = "summary";  // default



    while( (option = getopt(argc,argv,"n:f:b:l:")) != -1){

        switch(option) {
            case 'n':
                numOfAccesses = atoi(optarg);
                if(numOfAccesses < 1) {
                    std::cout << "Number of memory accesses must be a number and greater than 0.\n";
                    exit(EXITCODE);
                }
            break;
            case 'f':
                numOfFrames = atoi(optarg);
                if(numOfFrames < 1) {
                    std::cout << "Number of available frames must be a number and greater than 0.\n";
                    exit(EXITCODE);
                }
            break;
            case'b':
                nfuPeriod = atoi(optarg);
                if(nfuPeriod < 1) {
                    std::cout << "Bit string update interval must be a number and greater than 0.\n";
                    exit(EXITCODE);
                }
            break;
            case 'l':
                mode = optarg;
            break;
            default:
                std::cout<<"Please enter correct options\n";
                exit(EXITCODE);
        }
    }

    if(optind >= argc) {
        std::cout << "Missing mandatory arguments";
        exit(EXITCODE);
    }

    int argIndex = optind;  // first mandatory arg index

    p2AddrTr mtrace;
    unsigned int vAddr;

    FILE* tracef_h = fopen(argv[argIndex], "rb");

    if(!tracef_h) {
        std::cout << "Unable to open <<" << argv[argIndex] << ">>\n";
        exit(EXITCODE);
    }

    std::vector<int> ptLevelBits;

    argIndex++; // move to first level bits

    for(int i = argIndex; i < argc; i++) {
        int bits = atoi(argv[i]);

        if(bits < 1) {
            std::cout << "Level " << (i - argIndex) << " page table must be at least 1 bit\n";
            exit(EXITCODE);
        }
        ptLevelBits.push_back(bits);
    }

    int vpnBits = 0;

    for(int i : ptLevelBits) {
        vpnBits += i;
    }

    if(vpnBits > MAXVPNBITS) {
        std::cout << "Too many bits used in page tables\n";
        exit(EXITCODE);
    }

    PageTable pageTable(ptLevelBits, vpnBits);  // initialize multilevel page table with level info
    PageTracker tracker(nfuPeriod);

    int offsetBits = BIT32 - vpnBits;

    int numOfAddresses = 0;

    int maxPages = numOfFrames;

int numOfPageReplaces = 0;

    while(NextAddress(tracef_h,&mtrace) && numOfAddresses != numOfAccesses) {
        vAddr = mtrace.addr;

        numOfAddresses++;
        bool isHit = false;

        if(mode == "offset") {
            print_num_inHex(pageTable.extractOffset(vAddr));
        }


        tracker.updateBitstrings();

        Map* mapping = pageTable.searchMappedPfn(vAddr);

        unsigned int victimVa;
        int victimVpn = -1;  //default
        unsigned int victimPfn ;
        unsigned int victimBitstring;


        if(mapping) {
            pageTable.pageTableHits++;
            isHit = true;
        }
        else {

            if(tracker.loadedPages.size() == maxPages) {  // no more room/replacement

                unsigned int* victimInfo = tracker.findVictimVa();  //find victim info
                victimVa = victimInfo[0];
                victimPfn = victimInfo[1];
                victimBitstring = victimInfo[2];
                pageTable.insertMapForVpn2Pfn(victimVa, victimPfn, false); //invalidate victim page

                pageTable.insertMapForVpn2Pfn(vAddr, victimPfn, true ); // page new virtual page entry
                mapping = pageTable.searchMappedPfn(vAddr);

                tracker.loadedPages.at(mapping->pfn) = PageTracker::Frames(mapping->va, mapping->pfn, (1u << 15) ,tracker.clock);
                //tracker.loadedPages.at(mapping->pfn).va = mapping->va;
                //tracker.loadedPages.at(mapping->pfn).pfn = mapping->pfn;
                //tracker.loadedPages.at(mapping->pfn).bitString = mapping->pfn;
                //tracker.loadedPages.at(mapping->pfn).lastTimeAccessed = tracker.clock;


                numOfPageReplaces++;

                victimVpn = static_cast<int>(victimVa >> offsetBits);
            }

            else {  // room left/insert sequentially

                pageTable.insertMapForVpn2Pfn(vAddr,tracker.loadedPages.size(), true);
                mapping = pageTable.searchMappedPfn(vAddr);

                tracker.loadedPages.push_back(PageTracker::Frames(mapping->va, mapping->pfn,(1u << 15) ,tracker.clock));

            }
        }

        tracker.track(mapping->pfn);
        tracker.clock++;

        if(mode == "vpn2pfn_pr") {
            log_mapping((mapping->va >> offsetBits), mapping->pfn, victimVpn, victimBitstring, isHit);
        }


        if(mode == "va2pa") {
            uint32_t pa = (mapping->pfn << offsetBits) + pageTable.extractOffset(vAddr) ;
            log_va2pa(vAddr,pa);
        }

        if(mode == "vpns_pfn") {
            log_vpns_pfn(pageTable.numOfLevels, pageTable.levelVpns, mapping->pfn);
        }

    }

    if(mode == "summary") {
        log_summary(pageTable.pageSize, numOfPageReplaces,pageTable.pageTableHits, numOfAddresses,
        tracker.loadedPages.size(), pageTable.totalPgTableEntries);
    }

    if(mode == "bitmasks") {
        log_bitmasks(pageTable.numOfLevels, pageTable.levelMasks);
    }

    fclose(tracef_h);

    return 0;
}
