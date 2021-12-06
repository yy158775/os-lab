#include"Replacer.h"
#include"LRUReplacer.h"

LRUReplacer::LRUReplacer(){
    for(int i = 0;i < PPGCOUNT;i++) {
        pageinfo[i].page = i;
        pageinfo[i].pref = false;
        l.push_front(i);
        mp[i] = l.begin();
    }
}

pgid LRUReplacer::victim(int vptx) {
    PageInfo &p = pageinfo[l.front()];
    l.pop_front();

    if(p.pref) {
        pgtable[p.vptx] &= (~PTE_P);   //取消引用
    }
    pgtable[vptx] = p.page|PTE_P;
    p.vptx = vptx;
    p.pref = true;
    l.push_back(p.page);
    mp[p.page] = l.end();
    mp[p.page]--;
    return p.page;
}

void LRUReplacer::visit(int pgnum) {
    PageInfo &p = pageinfo[(*mp[pgnum])];
    l.erase(mp[pgnum]);
    l.push_back(p.page);
    mp[p.page] = l.end();
    mp[p.page]--;
}
