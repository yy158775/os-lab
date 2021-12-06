#pragma once
#include"Replacer.h"

class ClockReplacer : public Replacer {
    private:
        int index;
        // pgid q[PPGCOUNT]; //物理页 对应 的虚拟页
        bool ref[PPGCOUNT]; //true can't replace
    public:
        ClockReplacer();
        pgid victim(int vptx) override;
        void visit(pgid pgnum) override;
};

