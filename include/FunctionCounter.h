#ifndef _FunctionCounter_h_
#define _FunctionCounter_h_

#include <ElfFileInst.h>

class FunctionCounter : public ElfFileInst {
private:

public:
    FunctionCounter(ElfFile* elf);
    ~FunctionCounter();

    void declare();
    void instrument();
};


#endif /* _FunctionCounter_h_ */
