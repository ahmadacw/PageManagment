// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <stdint.h>

#include "os.h"
// our tree will be 5 levels deep
int TREESIZE = 5;
int depth = 0;


uint64_t* VirtToPhys(uint64_t PTEB, uint64_t Find, int flag) { //flag ==1 find me a physical address.
    int mask = 0xFF800;
    int FrameNumber ;
    uint64_t* head =(uint64_t *) phys_to_virt(PTEB);
    for (int i = 0; i < TREESIZE; i++) {
        depth = i;
        FrameNumber = (Find && mask) >> 48 - (9 * i);
        if (head[FrameNumber] == 0) { //no mapping
            if (mask) return NULL; 
            return &head[FrameNumber];
        }
        mask >> 9;
    }
    return &(head[FrameNumber]); // this VA has a mapping
   }
uint64_t alloc_page_frame() {
    return 1;
}

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn) {
    if (ppn != NO_MAPPING) {
    uint64_t* Add = VirtToPhys(pt, vpn,1);
    if (*Add != 0) {
        printf("Address already mapped");
        return;
    }
    while (depth < 4) {
        (*Add) = alloc_page_frame();
        uint64_t* Add = VirtToPhys(pt, vpn, 1);
    }
    uint64_t* Add = VirtToPhys(pt, vpn, 1);
    (*Add) = ppn;
    }
    else {
        uint64_t* Add = VirtToPhys(pt, vpn, 1);
        (*Add) = 0;
    }
}

int main()
{

    
}

