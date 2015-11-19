#include "../plugin.h"
#include "../index.h"

class IndexPlugin : virtual public Plugin
{
    public:
        IndexPlugin(void* libHandle) : Plugin(libHandle) {};
        Index* openFile(char* file);
        bool doFile(char* file);
        bool doIndex(Index* index);
        int getType();
        char* getName();
};