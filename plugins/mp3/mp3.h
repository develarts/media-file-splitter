#include "../../src/plugin.h"
#include "../../src/media.h"

class Mp3Frame
{
    protected:
        int bitrate;
        int version;
        int layer;
        int frequency;
        float length;
        char* data;

    public:
        Mp3Frame(char* data, FILE* file);
        float getLength();
        int getVersion();
        int getFrequency();
        int getBitrate();
        ~Mp3Frame();
};

class MediaMp3 : public Media
{
    private:
        int findId3v1();
        int findId3v2();
        bool findMetadata(Metadata* meta);

    protected:
        Metadata* metaData;

    public:
        MediaMp3(char* location) : Media(location) {};
        ~MediaMp3();
        char* getPeriod(int length, int start);
        Metadata* getMetadata();
        int calculateTagSize(char* in_buffer);
        Mp3Frame* getNextFrame();
};
