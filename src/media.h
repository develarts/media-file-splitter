#pragma once
#include <stdio.h>

#include "metadata.h"

class Media
{
	protected:
		FILE* file;
		char* location;
		Metadata* metadata;

	public:
		Media(char* location);
		~Media();
		Metadata* getMetadata();
		void setMetadata(Metadata* metadata);
		char* getLocation();
		virtual char* getPeriod(int length, int start) = 0;
};
