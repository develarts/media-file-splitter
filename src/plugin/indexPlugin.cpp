#include <dlfcn.h>
#include <cctype>
#include "../app.h"
#include "indexPlugin.h"
#include "mediaPlugin.h"

Index* IndexPlugin::openFile(char* file)
{
    if (!this->libHandle) {
        return NULL;
    }

    Index* (*fptr)(char*);
    if ((fptr = (Index*(*)(char*))dlsym(this->libHandle, "openFile")) == NULL) {
        throw dlerror();
    }

    return (*fptr)(file);
}

bool IndexPlugin::doFile(char* file)
{
    Index* index;
    Plugin* plugin;

    if (!(index = this->openFile(file))) {
        return false;
    }

    char* mediaFile = (char*)malloc(255 * sizeof(char));

    if (!index->getFile(mediaFile)) {
        return false;
    }
    
    char* mediaFilepath = (char*)malloc((strlen(index->getLocation()) + strlen(mediaFile) + 1) * sizeof(char));
    strcpy(mediaFilepath, index->getLocation());

    /*int i;
    for (i = 0; i < strlen(mediaFile); ++i)
	mediaFile[i] = tolower(mediaFile[i]);*/
    //strcpy(strrchr(mediaFilepath, '/') + sizeof(char), file);

    strcpy(strrchr(mediaFilepath, '/') + sizeof(char), mediaFile);

    struct stat attribut;

    if (stat(mediaFilepath, &attribut) == -1) {

        //char* location = index->getLocation();

        mediaFilepath = (char*)malloc((strlen(file) + 1) * sizeof(char));
		strcpy(mediaFilepath, file);
		//strcpy(strrchr(mediaFilepath, '/') + sizeof(char), mediaFile);
		strcpy(strrchr(mediaFilepath, '.') + sizeof(char), strrchr(mediaFile, '.') + sizeof(char));
	
		//strcpy(strrchr(mediaFilepath, '/') + sizeof(char), mediaFile);
		//index->setFile(mediaFilepath);
    }

    if (!(plugin = this->getPlugin(mediaFilepath, PLUGIN_MEDIA))) {

        printf("no plugin for media: %s\n", mediaFilepath);

        free(mediaFilepath);
        free(mediaFile);

        return false;
    }

    free(mediaFilepath);
    free(mediaFile);

    plugin->doIndex(index);

    delete index;

    return true;
}

bool IndexPlugin::doIndex(Index* index)
{
    return false;
}

int IndexPlugin::getType()
{
    return PLUGIN_INDEX;
}
