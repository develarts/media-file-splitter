#include <dlfcn.h>
#include <sys/stat.h>
#include "string.h"
#include "stdlib.h"
#include "mediaPlugin.h"

Media* MediaPlugin::openFile(char* file)
{
    if (!this->libHandle) {
        return NULL;
    }

    Media* (*fptr)(char*);
    if ((fptr = (Media*(*)(char*))dlsym(this->libHandle, "openFile")) == NULL) {
        throw dlerror();
    }

    return (*fptr)(file);
}

bool MediaPlugin::doFile(char* file)
{
    return false;
}

bool MediaPlugin::doIndex(Index* index)
{
    Metadata* meta;
    Metadata* mediaMeta;
    Media* media = NULL;
    struct stat attribut;

    while ((meta = index->getNext()) != NULL) {

        if (!media) {

            char* location = (char*)malloc((strlen(index->getLocation()) + strlen(meta->getFile()) + 1) * sizeof(char));
            strcpy(location, index->getLocation());
            strcpy(strrchr(location, '/') + sizeof(char), meta->getFile());

	    if (stat(location, &attribut) == -1) {
		printf("Media file not exist: %s\n", location);
		return false;
	    }
	    
            media = this->openFile(location);
            free(location);

            if ((mediaMeta = media->getMetadata())) {
                printf("%s: %s\n", mediaMeta->getArtist(), (char*)mediaMeta->getTitle());
            }
        }

        if (!meta->getLength()) {
            meta->setLength(mediaMeta->getLength() - meta->getStart());
        }

        printf("[%i, %s] %s: %s\n", meta->getStart(), meta->getLengthAsText(), meta->getArtist(), (char*)meta->getTitle());
        delete meta;
    }

    printf("length: %s\n", mediaMeta->getLengthAsText());

    if (media) {
        delete media;
    }

    return true;
}

int MediaPlugin::getType()
{
    return PLUGIN_MEDIA;
}