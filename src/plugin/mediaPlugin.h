#pragma once
#include "../plugin.h"
#include "../media.h"
#include "../index.h"

class MediaPlugin : virtual public Plugin
{
    public:
        MediaPlugin(void* libHandle) : Plugin(libHandle) {};
        Media* openFile(char* file);
        bool doFile(char* file);
        bool doIndex(Index* index);
        int getType();
        char* getName();
};