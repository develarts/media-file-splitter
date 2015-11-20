#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "index.h"

Index::Index(char* location)
{
    if (!(this->file = fopen(location, "rb"))) {
        this->file = fopen(location, "wb");
    }
    this->location = (char*)malloc((strlen(location) + 1) * sizeof(char));
    strcpy(this->location, location);

    this->cursor = 1;
}

char* Index::getLocation()
{
    return this->location;
}

Index::~Index()
{
    if (this->file != NULL) {
        fclose(this->file);
    }
    if (this->location != NULL) {
        free(this->location);
    }
}
