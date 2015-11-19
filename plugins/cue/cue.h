#include "../../src/plugin.h"
#include "../../src/index.h"

class IndexCue : public Index
{
    protected:
        Metadata* meta;
        int getNextIndex();

    public:
        IndexCue(char* location) : Index(location) {};
        Metadata* getNext();
        bool getFile(char* output);
        ~IndexCue();
};
