#include "log.h"

using namespace MediaFileSplitter;

Log* Log::instance;

Log::Log()
{
	if (instance) {
		delete instance;
	}

	silent = false;
	instance = this;
}

void Log::setSilent(bool state)
{
	silent = state;
}

void Log::show(const char* msg)
{
	if (this->silent) {
		return;
	}
	fprintf(stdout, "%s\n", msg);
}
