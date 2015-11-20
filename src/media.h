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
		virtual ~Media();
		Metadata* getMetadata();
		void setMetadata(Metadata* metadata);
		virtual char* getPeriod(int length, int start) = 0;
		char* getLocation();

        /*int getLength();
        int getPosition();
        int setPosition(int seconds);

        void append(char* buffer);*/
};
