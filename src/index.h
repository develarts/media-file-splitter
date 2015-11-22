#pragma once
#include <stdio.h>

#include "metadata.h"

class Index
{
    protected:
        FILE* file;
        char* location;
        int cursor;

    public:
        Index(char* location);
        ~Index();
        char* getLocation();
        virtual Metadata* getNext() = 0;
        virtual bool getFile(char* output) = 0;
};
