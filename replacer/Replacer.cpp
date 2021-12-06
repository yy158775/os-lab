#include"Replacer.h"
#include"LRUReplacer.h"
#include"ClockReplacer.h"
using namespace std;

char Porcess[PGSIZE*VPGCOUNT];
char Page[PPGCOUNT][PGSIZE];
int pgtable[VPGCOUNT];

PageInfo pageinfo[PPGCOUNT];

void pgload(void *dst,void *src) {
    memcpy(dst,src,PGSIZE);    
}

Replacer *replacer;

bool pgfalut;
uint32_t pgflttime = 0;

void pg_falut(int vaddr) {
    pgfalut = true;
    int pgnum = replacer->victim(PTX(vaddr));
    pgload((void*)&Page[pgnum][0],(void*)&Porcess[PTX(vaddr)*PGSIZE]);
}

void visit(int vaddr) {
    if(!(pgtable[PTX(vaddr)] & PTE_P)) {
        pgflttime++;        
        pg_falut(vaddr);
    } else {
        //lru 提前 hashmap
        replacer->visit(pgtable[PTX(vaddr)] & (~PTE_P));
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
    if (argc != 3) {
        printf("Usage:%s $visit-time $choice\n",argv[0]);
        printf("choice:\n");
        printf("1:LRU\n");
        printf("2:CLOCK");
        return 0;
    }
    int T = atoi(argv[1]);
    int choice = atoi(argv[2]);
    
    if(choice == 1) {
        replacer =  new LRUReplacer();
    } else if(choice == 2) {
        replacer =  new ClockReplacer();
    }

    freopen("in.txt","r",stdin);
    int v;
    while (T--)
    {
        // v = (rand()%(VPGCOUNT*PGSIZE)); 
        cin>>v; 
        visit(v);
    }
    printf("Page Fault Time:%u\n",pgflttime);
}
