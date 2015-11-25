#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#include "log.h"
#include "plugin.h"

namespace MediaFileSplitter {

using namespace std;

struct appOptions
{
	const char* path;
	const char* backup;
	bool clean;
	bool silent;
};

class App
{
private:
	vector<Plugin*> plugins;
	appOptions options;

public:
	App(appOptions options);

	void initLogger();

	void start();
	void scanDir(char* path);
	bool process(char* file);

	bool registerPlugin(Plugin* plugin);
	Plugin* getPlugin(char* file, int type);
	Plugin* getPlugin(char* file);
};

} // namespace MediaFileSplitter
