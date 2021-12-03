#include<bits/stdc++.h>
using namespace std;
#define PGSIZE 64
#define PGSHIFT 6
#define VPGCOUNT 128
#define PPGCOUNT 16
#define PTE_P 0x10

//PTX PTE
#define PTX(vaddr)		(vaddr >> PGSHIFT)
#define PGOFF(vaddr)    (vaddr & 0x3F)
#define PGADDR(vaddr)   (pg)


char Porcess[PGSIZE*VPGCOUNT];
char Page[PPGCOUNT][PGSIZE];
int pgtable[VPGCOUNT];

#define pgid int

struct PageInfo {
    int vptx; //1 -- 100 page table index
    pgid page;
    bool pref;
}pageinfo[PPGCOUNT];


struct LRU {
    list<pgid>l;
    map<pgid,list<pgid>::iterator>mp;
}Lru;

//初始化
void init_lru() {
    for(int i = 0;i < PPGCOUNT;i++) {
        pageinfo[i].page = i;
        pageinfo[i].pref = false;
        Lru.l.push_front(i);
        Lru.mp[i] = Lru.l.begin();
    }

}

//选出一页，去替换掉
pgid victim(int vptx) {
    PageInfo p = pageinfo[Lru.l.front()];
    Lru.l.pop_front();

    if(p.pref) {
        pgtable[p.vptx] &= (~PTE_P);   //取消引用
    }
    pgtable[vptx] = p.page|PTE_P;
    p.vptx = vptx;
    p.pref = true;
    Lru.l.push_back(p.page);
    Lru.mp[p.page] = Lru.l.end();
    Lru.mp[p.page]--;
    return p.page;
}

//访问某一页，更新LRU
void lru_visit(pgid pgnum) {
    PageInfo p = pageinfo[(*Lru.mp[pgnum])];
    Lru.l.erase(Lru.mp[pgnum]);
    Lru.l.push_back(p.page);
    Lru.mp[p.page] = Lru.l.end();
    Lru.mp[p.page]--;
}

void pgload(void *dst,void *src) {
    memcpy(dst,src,PGSIZE);    
}

bool pgfalut;
uint32_t pgflttime = 0;

void pg_falut(int vaddr) {
    pgfalut = true;
    int pgnum = victim(PTX(vaddr));
    pgload((void*)&Page[pgnum][0],(void*)&Porcess[PTX(vaddr)*PGSIZE]);
}

void visit(int vaddr) {
    if(!(pgtable[PTX(vaddr)] & PTE_P)) {
        pgflttime++;        
        pg_falut(vaddr);
    } else {
        //lru 提前 hashmap
        lru_visit(pgtable[PTX(vaddr)] & (~PTE_P));
        pgfalut = false;
    }
    int pgidx = pgtable[PTX(vaddr)];
    int val = Page[pgidx][PGOFF(vaddr)];
    printf("addr:0x%13x val:%10d",vaddr,val);
    if(pgfalut) {
        printf(" Page Fault\n");
    } else {
        printf("\n");
    }
    return;
}

int main(int argc,char *argv[]) {
    init_lru();
    if (argc != 2) {
        printf("Usage:%s visit-time",argv[0]);
        return 0;
    }
    int T = atoi(argv[1]);
    freopen("in","r",stdin);
    int v;
    while (T--)
    {
        // v = (rand()%(VPGCOUNT*PGSIZE)); 
        cin>>v; 
        visit(v);
    }
    printf("Page Fault Time:%u\n",pgflttime);
}
