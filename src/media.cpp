#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "media.h"

Media::Media(char* location)
{
	if (!(this->file = fopen(location, "rb"))) {
		this->file = fopen(location, "wb");
	}
	this->location = (char*)malloc((strlen(location) + 1) * sizeof(char));
	strcpy(this->location, location);
	this->metadata = NULL;
}

Metadata* Media::getMetadata()
{
	return this->metadata;
}

void Media::setMetadata(Metadata* meta)
{
	this->metadata = meta;
}

char* Media::getLocation()
{
	return this->location;
}

Media::~Media()
{
    if (this->file != NULL) {
        fclose(this->file);
    }
    if (this->metadata) {
        delete this->metadata;
    }
    if (this->location) {
        free(this->location);
    }
}
