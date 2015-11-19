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
		Media(char *location);
		~Media();
		virtual Metadata* getMetadata();
		virtual void setMetadata(Metadata* metadata);
		virtual char* getPeriod(int length, int start) = 0;
		virtual char* getLocation();

        /*int getLength();
        int getPosition();
        int setPosition(int seconds);

        void append(char* buffer);*/
};
