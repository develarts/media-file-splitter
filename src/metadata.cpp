#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "metadata.h"
typedef unsigned char uchar;

Metadata::Metadata()
{
    this->album = '\0';
    this->artist = '\0';
    this->title = '\0';
    this->comment = '\0';
    this->file = '\0';
    this->genre = '\0';
    this->lengthAsText = '\0';
}

char* Metadata::getAlbum()
{
    return this->getAlbum();
}

void Metadata::setAlbum(char* album)
{
    this->album = (char*)malloc(sizeof(char) * (strlen(album) + 1));
    memset(this->album, (int)'\0', (strlen(album) + 1));
    strcpy(this->album, album);
}

char* Metadata::getArtist()
{
    return this->artist;
}

void Metadata::setArtist(char* artist)
{
    this->artist = (char*)malloc(sizeof(char) * (strlen(artist) + 1));
    memset(this->artist, (int)'\0', (strlen(artist) + 1));
    strcpy(this->artist, artist);
}

char* Metadata::getTitle()
{
    return this->title;
}

void Metadata::setTitle(char* title)
{
    //this->title = title;
    
    //printf("konwersja [%d - %d] :: %s -> %s\n", len, sizeof(title), this->title, title);
    this->title = (char*)malloc(sizeof(char) * (strlen(title) + 1));
    memset(this->title, (int)'\0', (strlen(title) + 1));
    strcpy(this->title, title);
    
    return;
    
//    int len = strlen(title);
    
    //this->title = (wchar_t*)malloc(sizeof(wchar_t) * (len + 1));
    //memset(this->title, (int)'\0', (len + 1));
    
    /*int i = 0;

    for (i = 0; i < len; i++) {
	wcscat(this->title, (wchar_t*)uchar(title[i]));
    }*/
    
//    mbstowcs(this->title, title, 200);
}

char* Metadata::getComment()
{
    return this->comment;
}

void Metadata::setComment(char* comment)
{
    this->comment = (char*)malloc(sizeof(char) * (strlen(comment) + 1));
    memset(this->comment, (int)'\0', (strlen(comment) + 1));
    strcpy(this->comment, comment);
}

int Metadata::getStart()
{
    return this->start;
}

void Metadata::setStart(int start)
{
    this->start = start;
}

int Metadata::getLength()
{
    return this->length;
}

void Metadata::setLength(int length)
{
    this->length = length;
}

char* Metadata::getLengthAsText()
{
    if (!this->lengthAsText) {

        int iLength = this->getLength();
        this->lengthAsText = (char*)malloc(sizeof(char) * 10);
        memset(this->lengthAsText, '\0', 10);

        sprintf(this->lengthAsText, "%02i:%02i:%02i", (int)(iLength / (100 * 60)), (int)((iLength % (100 * 60)) / 100), (iLength % (100 * 60)) % 100);
    }

    return this->lengthAsText;
}

int Metadata::getYear()
{
    return this->year;
}

void Metadata::setYear(int year)
{
    this->year = year;
}

char* Metadata::getGenre()
{
    return this->genre;
}

void Metadata::setGenre(char* genre)
{
    this->genre = (char*)malloc(sizeof(char) * (strlen(genre) + 1));
    memset(this->genre, (int)'\0', (strlen(genre) + 1));
    strcpy(this->genre, genre);
}

char* Metadata::getFile()
{
    return this->file;
}

void Metadata::setFile(char* file)
{
    this->file = (char*)malloc(sizeof(char) * (strlen(file) + 1));
    memset(this->file, (int)'\0', (strlen(file) + 1));
    strcpy(this->file, file);
}

Metadata::~Metadata()
{
    if (this->album) {
        free(this->album);
    }
    if (this->artist) {
        free(this->artist);
    }
    if (this->title) {
        free(this->title);
    }
    if (this->comment) {
        free(this->comment);
    }
    if (this->genre) {
        free(this->genre);
    }
    if (this->file) {
        free(this->file);
    }
    if (this->lengthAsText) {
        free(this->lengthAsText);
    }
}