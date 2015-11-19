#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plugin.h"
#include "plugin/indexPlugin.h"
#include "plugin/mediaPlugin.h"

Plugin::Plugin(void* libHandle)
{
    this->libHandle = libHandle;
}

Plugin* Plugin::load(char* file)
{
    void *libHandle;

    if (((libHandle = dlopen(file, RTLD_LAZY)) == NULL)) {
        printf("dlopen error: %s\n", dlerror());
        return NULL;
    }

    int (*fGetTypePtr)();

    if ((fGetTypePtr = (int(*)())dlsym(libHandle, "getType")) == NULL) {
        printf("dlsym(getType) error: %s\n", dlerror());
        return NULL;
    }

    Plugin* plugin;

    if ((*fGetTypePtr)() == PLUGIN_MEDIA) {
        plugin = new MediaPlugin(libHandle);
    } else {
        plugin = new IndexPlugin(libHandle);
    }

    char* (*fGetNamePtr)();

    if ((fGetNamePtr = (char*(*)())dlsym(libHandle, "getName")) != NULL) {
        plugin->setName((*fGetNamePtr)());
    } else {
        plugin->setName((char*)"Unknown");
    }

    return plugin;
}

char* Plugin::getName()
{
    return this->name;
}

bool Plugin::checkFile(char* file)
{
    bool (*fCheckFilePtr)(char*);

    if ((fCheckFilePtr = (bool(*)(char*))dlsym(libHandle, "checkFile")) == NULL) {
        throw dlerror();
    }

    return (*fCheckFilePtr)(file);
}

void Plugin::setName(char* name)
{
    this->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(this->name, name);
}

Plugin::~Plugin()
{
    if (this->libHandle) {
        dlclose(this->libHandle);
    }
    if (this->name) {
        free(this->name);
    }
}
