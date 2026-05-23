/**
* @author Jurne Smith
 * Red ID: 824190049
 * Prof. Shen
 * CS480 - Operating Systems
 * Assignment 3
 * 11/1/25
 */

#ifndef PAGENODE_H
#define PAGENODE_H
#include <cstdint>

/**
* struct map represents the page mappings between virtual mem and ram
*/

struct Map {

    uint32_t pfn;
    uint32_t va;
    bool isValid;


    Map(uint32_t pfn, uint32_t va, bool isValid) : pfn(pfn), va(va), isValid(isValid){}

};
/**
* @class PageNode represents the page table entries in the n level page Table.
*/
class PageNode {

public:
    unsigned int numEntries;
    PageNode** nextLevel;
    Map** mappings;
    bool isLeaf;

    PageNode(unsigned int numIntries = 0, bool isLeaf = 0);
    ~PageNode();






};




#endif //PAGENODE_H
