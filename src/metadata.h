#pragma once

class Metadata
{
    private:
        char* title;
        char* artist;
        char* album;
        char* genre;
        char* comment;
        char* file;
        char* lengthAsText;
        int year;
        int start;
        int length;
    public:
	char* getTitle();
	void setTitle(char* title);
        char* getArtist();
        void setArtist(char* artist);
        char* getAlbum();
        void setAlbum(char* album);
        int getYear();
        void setYear(int year);
        char* getGenre();
        void setGenre(char* genre);
        char* getComment();
        void setComment(char* comment);
        int getStart();
        void setStart(int index);
        int getLength();
        void setLength(int length);
        char* getLengthAsText();
        char* getFile();
        void setFile(char* file);
        Metadata();
        ~Metadata();
    void stelen(const wchar_t* title);
};
