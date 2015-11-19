#pragma once
#include <dlfcn.h>
#include "index.h"

#define PLUGIN_MEDIA 1
#define PLUGIN_INDEX 2

class Plugin
{
    protected:
        char* name;
        void* libHandle;

    public:
        Plugin(void* libHandle);
        static Plugin* load(char* file);

        virtual int getType() = 0;
        char* getName();
        void setName(char *);

        bool checkFile(char* file);
        virtual bool doFile(char*) = 0;
        virtual bool doIndex(Index*) = 0;

        ~Plugin();
};
