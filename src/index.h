#pragma once
#include <stdio.h>

#include "metadata.h"

class Index
{
    protected:
        FILE *file;
        char *location;
        int cursor;

    public:
        Index(char *location);
        char* getLocation();
        virtual Metadata* getNext() {};
        virtual bool getFile(char* output) = 0;
        ~Index();
};

