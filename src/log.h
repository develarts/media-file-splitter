#pragma once

#include <stdio.h>
#include <stdarg.h>

namespace MediaFileSplitter {

#define MSGSIZE_MAX 3072

class Log
{
public:

	Log();

	void setSilent(bool state);

	void show(const char* msg);

	static void notify(const char* format, ...)
	{
		char msg[MSGSIZE_MAX];

		va_list va;
		va_start(va, format);

		vsnprintf(msg, MSGSIZE_MAX, format, va);

		va_end(va);

		instance->show(msg);
	};

private:

	static Log* instance;

	bool silent;

};

} // namespace MediaFileSplitter
