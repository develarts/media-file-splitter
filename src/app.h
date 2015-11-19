#pragma once
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

#include "plugin.h"

using namespace std;

namespace MediaFileSplitter {

	struct appOptions
	{
		const char *path;
		const char *backup;
		bool clean;
		bool silent;
	};

	class App
	{
		private:
			vector <Plugin*> plugins;
			appOptions options;

		public:

			App(appOptions options);
			void start();
			bool registerPlugin(Plugin* plugin);
			Plugin* getPlugin(char* file, int type);
			Plugin* getPlugin(char* file);
			bool doIndex(char* file);
			void scanDir(char* path);
	};

}
