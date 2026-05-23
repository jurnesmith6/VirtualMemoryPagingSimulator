/**
* @author Jurne Smith
 * Red ID: 824190049
 * Prof. Shen
 * CS480 - Operating Systems
 * Assignment 3
 * 11/1/25
 */

#include "PageNode.h"


PageNode::PageNode(unsigned int numEntries, bool isLeaf): numEntries(numEntries), isLeaf(isLeaf) {
    if(!isLeaf) {
        nextLevel = new PageNode*[numEntries];
        mappings = nullptr;
        for(int i = 0; i < numEntries; i++) {
            nextLevel[i] = nullptr;
        }
    }
    else {
        mappings = new Map*[numEntries];
        for(int i = 0; i < numEntries; i++) {
            mappings[i] = nullptr;
        }
        nextLevel = nullptr;

    }
}

PageNode::~PageNode() {

    if(isLeaf) {
        if(mappings) {
            for(int i = 0; i < numEntries; i++) {
                delete mappings[i];
            }
            delete [] mappings;
        }
    }
    else {
        if(nextLevel) {
            for(int i = 0; i < numEntries; i++) {
                delete nextLevel[i];
            }
            delete [] nextLevel;
        }

    }

}



