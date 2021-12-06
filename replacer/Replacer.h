#pragma once

#include<cstdio>
#include<list>
#include<map>
#include<iostream>
#include<cstring>
using namespace std;

#define PGSIZE 64
#define PGSHIFT 6
#define VPGCOUNT 128
#define PPGCOUNT 16
#define PTE_P 0x10 // log2PPGCOUNT
 
//PTX PTE
#define PTX(vaddr)		(vaddr >> PGSHIFT)
#define PGOFF(vaddr)    (vaddr & 0x3F)
#define PGADDR(vaddr)   (pg)
#define pgid int //物理页数

struct PageInfo {
    int vptx; //1 -- 100 page table index
    pgid page;
    bool pref;
};

extern char Porcess[PGSIZE*VPGCOUNT];
extern char Page[PPGCOUNT][PGSIZE];
extern int pgtable[VPGCOUNT];
extern PageInfo pageinfo[PPGCOUNT];

class Replacer {
    public:
    	Replacer() = default;
    	virtual pgid victim(int vptx) = 0; //逐出哪个物理页 换成vptx这一页
    	virtual void visit(pgid pgnum) = 0; //访问过哪个物理页
};


