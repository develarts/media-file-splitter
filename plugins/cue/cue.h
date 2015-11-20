#include "../../src/plugin.h"
#include "../../src/index.h"

class IndexCue : public Index
{
    protected:
        int getNextIndex();

    public:
        IndexCue(char* location) : Index(location) {};
        Metadata* getNext();
        bool getFile(char* output);
        virtual ~IndexCue();
};
