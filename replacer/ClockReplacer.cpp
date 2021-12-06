#include "ClockReplacer.h"

ClockReplacer::ClockReplacer() {
    index = 0;
    for(int i = 0;i < PPGCOUNT;i++) {
        ref[i] = false;
        pageinfo[i].page = i;
        pageinfo[i].pref = false;
    }
}

pgid ClockReplacer::victim(int vptx) {
    int ans; 
    bool find = false;
    for(int i = 0;i < PPGCOUNT;i++) {
        if(!ref[index]) {
            find = true;
            break;
        }
        index = (++index)%(PPGCOUNT);
    }
    if(!find) {
        //set false
        for(int i = 0;i < PPGCOUNT;i++) {
            ref[index] = false;
            index = (++index)%(PPGCOUNT);
        }
    }
    ref[index] = true;
    index = (++index)%(PPGCOUNT);
    PageInfo &p = pageinfo[index];
    if(p.pref) {
        pgtable[p.vptx] &= (~PTE_P);
    }
    pgtable[vptx] = p.page|PTE_P;
    p.vptx = vptx;
    p.pref = true;
    return p.page;
}   

void ClockReplacer::visit(pgid pgnum) {
    ref[pgnum] = true;
}