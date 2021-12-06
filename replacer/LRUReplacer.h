#pragma once
#include"Replacer.h"

class LRUReplacer : public Replacer {
private:
    list<pgid>l;
    map<pgid,list<pgid>::iterator>mp;
public:
    LRUReplacer();
    pgid victim(int vptx) override;
    void visit(pgid pgnum) override;
};

