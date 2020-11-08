// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <stdint.h>

#include "os.h"
// our tree will be 5 levels deep
int TREESIZE = 5;
int depth = 0;
uint64_t MASK = 0xFF800;

uint64_t* FindSecondToLastPointer(uint64_t PTEB, uint64_t Find) { //return the a virtual address to the pointer which points to the last mapped level for the page walk
    uint64_t mask = MASK;
    depth = 0;
    int FrameNumber ;
    uint64_t* head =(uint64_t *) phys_to_virt(PTEB);
    for (int i = 0; i < TREESIZE-1; i++) {
        depth = i;
        FrameNumber = (Find & mask) >> 48 - (9 * i);
        if (head[FrameNumber] == 0) { //this VA is missing some levels.
            return head;
        }
        mask >> 9;
        head = (uint64_t*)phys_to_virt(head[FrameNumber]);
    }
    return head; // this VA has all its levels, might not have a PA
   }
uint64_t getChunk(int depth, uint64_t vpn) {
    return (vpn & ((MASK>>9*depth))) >> 48 - 9 * depth;

}
void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn) {
    if (ppn != NO_MAPPING) {
    uint64_t* Address = FindSecondToLastPointer(pt, vpn);// get pointer to the pagetable
    Address = (uint64_t*)phys_to_virt((Address[getChunk(depth, vpn)])); // get pointer to physical page
    while (depth < TREESIZE-1) { // drill down to the second to last node
        (*Address) = alloc_page_frame(); //allocate and set a frame for the next level
        Address = FindSecondToLastPointer(pt, vpn);//get the pointer to the page that the next address is sitting in
        Address = (uint64_t*)phys_to_virt((Address[getChunk(depth, vpn)])); // get the actuall address
    }
    (*Address) = ppn;
    }
    else {
        uint64_t* Address = FindSecondToLastPointer(pt, vpn);
        if (depth == TREESIZE - 1) {
            Address = (uint64_t*)phys_to_virt((Address[getChunk(depth, vpn)])); // get the actuall address
            (*Address) = 0; // reset it
        }
    }
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn) {
    uint64_t* Address = FindSecondToLastPointer(pt, vpn);// get pointer to the pagetable
    Address = (uint64_t*)(Address[getChunk(depth, vpn)]); // get pointer to physical page
    if (!Address) return NO_MAPPING;
    return *Address;
}

int main()
{

    
}

