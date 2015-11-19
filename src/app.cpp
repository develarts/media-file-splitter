#include "app.h"

using namespace MediaFileSplitter;

App::App(appOptions options)
{
	this->options = options;

    char pluginPath[PATH_MAX] = {0};

    readlink("/proc/self/exe", pluginPath, sizeof(pluginPath));
    strcpy(strrchr(pluginPath, '/') + sizeof(char), "plugin/");

    struct dirent **filelist = {0};
    int fCount = -1;

    fCount = scandir(pluginPath, &filelist, 0, alphasort);

    char filePath[PATH_MAX] = {0};
    int i = 0;

    for (i = 0; i < fCount; i++) {

        if (!strstr(filelist[i]->d_name, ".so")) {
            continue;
        }

        memset(filePath, (int)'\0', sizeof(filePath));
        strcpy(filePath, pluginPath);
        strcat(filePath, filelist[i]->d_name);

        this->registerPlugin(Plugin::load(filePath));
    }
}

void App::start()
{
    DIR *dir;

    if ((dir = opendir(this->options.path)) != NULL) {

    	this->scanDir((char*)this->options.path);

    } else if (errno == ENOTDIR) {

    	this->doIndex((char*)this->options.path);

    } else {

        throw "No input specified";
    }
}

bool App::registerPlugin(Plugin* plugin)
{
	if (!plugin) {
		return false;
	}

	this->plugins.resize(this->plugins.size() + 1);
	this->plugins[this->plugins.size() - 1] = plugin;

	printf("plugin registered: %s (type: %i)\n", plugin->getName(), plugin->getType());
}

Plugin* App::getPlugin(char* file, int type)
{
	unsigned int i = 0;

	for (i = 0; i < this->plugins.size(); i++) {
		if (type != 0 && this->plugins[i]->getType() != type) {
			continue;
		}
		if (this->plugins[i]->checkFile(file)) {
			return this->plugins[i];
		}
	}

	return NULL;
}

Plugin* App::getPlugin(char* file)
{
	return this->getPlugin(file, 0);
}

bool App::doIndex(char* file)
{
	Plugin* plugin;

	if ((plugin = this->getPlugin(file, PLUGIN_INDEX)) == NULL) {
		return false;
	}

	printf("doIndex: %s\n", file);
	plugin->doFile(file);

	return true;
}

void App::scanDir(char* path)
{
	struct stat attribut;

	if (stat(path, &attribut) == -1) {

		printf("stat(%s) failed\n", path);
		return;

	} else if (!(attribut.st_mode & S_IFDIR)) {

		this->doIndex(path);
		return;
	}

	DIR* dir;
	struct dirent* entry;

	if (!(dir = opendir(path))) {
		return;
	}

	//printf("scanDir: %s\n", path);

	while ((entry = readdir(dir)) != NULL) {

		if (!strcmp((char*)entry->d_name, ".") || !strcmp((char*)entry->d_name, "..")) {
			continue;
		}

		char* file = (char*)malloc((strlen(path) + strlen(entry->d_name) + 2) * sizeof(char));
		memset(file, '\0', (strlen(path) + strlen(entry->d_name) + 2));
		strcpy(file, path);
		if (file[strlen(file) - 1] != '/') {
			strcat(file, "/");
		}
		strcat(file, (char*)entry->d_name);
		this->scanDir(file);
		free(file);
	}
	closedir(dir);
}
