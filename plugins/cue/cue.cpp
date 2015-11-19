#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "cue.h"

extern "C"
int getType()
{
    return PLUGIN_INDEX;
}

extern "C"
char* getName()
{
    return (char*)"cue index plugin";
}

extern "C"
bool checkFile(char* file)
{
	for (unsigned int i = 0; i < strlen(file); i++) {
		file[i] = tolower(file[i]);
	}

    if ((strcmp(&file[strlen(file) - 3], "Cue") != 0)) {
        return false;
    }

    FILE* pFile;

    if (!(pFile = fopen(file, "rb"))) {
        return false;
    }

    char* buffer = (char*)malloc(255 * sizeof(char));

    if (!(fread(buffer, 1, 254, pFile))) {
        free(buffer);
        fclose(pFile);
        return false;
    }

    if (!(strstr(buffer, "FILE"))) {
        free(buffer);
        fclose(pFile);
        return false;
    }

    free(buffer);
    fclose(pFile);

    return true;
}

extern "C"
IndexCue* openFile(char* file)
{
    return new IndexCue(file);
}

bool IndexCue::getFile(char* output)
{
    char* c = (char*)malloc(2 * sizeof(char));
    char* buffer = (char*)malloc(128 * sizeof(char));
    memset(buffer, (int)'\0', sizeof(buffer));
    bool isField = false;
    char* value;
    int pos = 0;

    fseek(this->file, 0, SEEK_SET);

    while (fread(c, 1, 1, this->file)) {

        if (c[0] != '\n') {
            strncat(buffer, c, 1);
            continue;
        }

        value = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(value, "");

        for (pos = 1; pos < strlen(buffer); pos++) {
            if (buffer[pos] == '"') {
                if (!isField) {
                    isField = true;
                    continue;
                } else {
                    break;
                }
            }
            if (isField) {
                strncat(value, &buffer[pos], 1);
            }
        }

        if (strstr(buffer, "FILE") != false) {

            memset(output, '\0', sizeof(output));
            strcpy(output, value);

            free(value);
            free(c);
            free(buffer);

            return true;
        }

        isField = false;
        strcpy(buffer, "");
        free(value);
    }

    free(c);
    free(buffer);

    return false;
}

int IndexCue::getNextIndex()
{
    char* c = (char*)malloc(2 * sizeof(char));
    char* buffer = (char*)malloc(128 * sizeof(char));
    memset(buffer, (int)'\0', sizeof(buffer));

    while (fread(c, 1, 1, this->file)) {

        if (c[0] != '\n') {
            strncat(buffer, c, 1);
            continue;
        }

        if (strstr(buffer, "INDEX") != false) {

            int index;
            char* tmp = (char*)malloc(sizeof(char) * 3);
            strncpy(tmp, &buffer[13], 2);
            index = atoi(tmp) * 60 * 100;
            strncpy(tmp, &buffer[16], 2);
            index += atoi(tmp) * 100;
            strncpy(tmp, &buffer[19], 2);
            index += atoi(tmp);

            free(c);
            free(tmp);

            return index;
        }

        strcpy(buffer, "");
    }

    free(c);
    free(buffer);

    return -1;
}

Metadata* IndexCue::getNext()
{
    Metadata* meta = new Metadata;

    char* c = (char*)malloc(2 * sizeof(char));
    char* buffer = (char*)malloc(128 * sizeof(char));
    memset(buffer, (int)'\0', sizeof(buffer));

    char* value;

    bool isHeader = true;
    bool isField = false;
    int track = 0;
    int pos = 0;
    int index;

    fseek(this->file, 0, SEEK_SET);

    while (fread(c, 1, 1, this->file)) {

        if (c[0] != '\n') {
            strncat(buffer, c, 1);
            continue;
        }

        value = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(value, "");

        for (pos = 1; pos < strlen(buffer); pos++) {
            if (buffer[pos] == '"') {
                if (!isField) {
                    isField = true;
                    continue;
                } else {
                    break;
                }
            }
            if (isField) {
                strncat(value, &buffer[pos], 1);
            }
        }

        if (strstr(buffer, "FILE ") != false) {
            meta->setFile(value);
            strcpy(buffer, "");
            isHeader = false;
            free(value);
            continue;
        }

        if (strstr(buffer, "TRACK") != false) {
            track++;
            if (this->cursor < track) {
                this->cursor++;
                if ((index = this->getNextIndex()) > 0) {
                    meta->setLength(index - meta->getStart());
                } else {
                    meta->setLength(0);
                }
                free(value);
                return meta;
            } else {
                meta->setLength(0);
            }
        }

        isField = false;

        if (isHeader || this->cursor == track) {

            if (!isHeader && strstr(buffer, "PERFORMER") != false) {
                meta->setArtist(value);
            }

            if (strstr(buffer, "COMMENT") != false) {
                meta->setComment(value);
            }

            if (strstr(buffer, "DATE") != false) {
                meta->setYear(atoi(&buffer[9]));
            }

            if (strstr(buffer, "INDEX") != false) {
                char* tmp = (char*)malloc(sizeof(char) * 3);
                strncpy(tmp, &buffer[13], 2);
                index = atoi(tmp) * 60 * 100;
                strncpy(tmp, &buffer[16], 2);
                index += atoi(tmp) * 100;
                strncpy(tmp, &buffer[19], 2);
                index += atoi(tmp);
                free(tmp);
                meta->setStart(index);
            }

            if (strstr(buffer, "GENRE") != false) {
                meta->setGenre(&buffer[10]);
            }

            if (strstr(buffer, "TITLE") != false) {
                if (isHeader) {
                    meta->setAlbum(value);
                } else {
                    meta->setTitle(value);
                }
            }

        }

        strcpy(buffer, "");
        free(value);
    }

    if (this->cursor == track) {

        this->cursor++;

        free(c);
        free(buffer);

        return meta;
    }

    free(c);
    free(buffer);

    return NULL;
}
