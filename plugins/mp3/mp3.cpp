#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>

#include <taglib/tag.h>
#include <taglib/fileref.h>

#include "mp3.h"

extern "C"
int getType()
{
	return PLUGIN_MEDIA;
}

extern "C"
char* getName()
{
	return (char*)"mp3 media plugin";
}

extern "C"
bool checkFile(char* file)
{
	if (strcmp(&file[strlen(file) - 3], "mp3") != 0) {
		return false;
	}

	FILE* f;

	if (!(f = fopen(file, "rb"))) {
		return false;
	}

	fclose(f);

	MediaMp3* media = new MediaMp3(file);

	if (!media->getNextFrame()) {
		return false;
	}

	return true;
}

extern "C"
MediaMp3* openFile(char* file)
{
	return new MediaMp3(file);
}

int frequencies[3][4] =
{
   {22050, 24000, 16000, 50000},	/* MPEG 2.0 */
   {44100, 48000, 32000, 50000},	/* MPEG 1.0 */
   {11025, 12000, 8000, 50000}		/* MPEG 2.5 */
};

int bitrates[2][3][15] =
{
	{ /* MPEG 2.0 */
		{0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256},	/* layer 1 */
		{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},			/* layer 2 */
		{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160}			/* layer 3 */

	},
	{ /* MPEG 1.0 */
		{0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448},	/* layer 1 */
		{0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384},	/* layer 2 */
		{0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320}		/* layer 3 */
	}
};

char* MediaMp3::getPeriod(int length, int start)
{
	return NULL;
}

/*Metadata* MediaMp3::getMetadata()
{
	if (!this->metadata) {
		Metadata* meta = new Metadata();
		if (this->findMetadata(meta)) {
			this->setMetadata(meta);
		}
	}

	return this->metadata;
}*/

int MediaMp3::findId3v1()
{
	if (fseeko(this->file, (off_t)-128, SEEK_END) == -1) {
		return 0;
	}

	if (fgetc(this->file) == 'T') {
		if (fgetc(this->file) == 'A') {
			if (fgetc(this->file) == 'G') {
				return -128;
			}
		}
	}

	return 0;
}

int MediaMp3::findId3v2()
{
	unsigned long ret = 0;

	if (fseeko(this->file, 0, SEEK_SET) == -1) {
		return 0;
	}

	if (fgetc(this->file) == 'I') {
		if (fgetc(this->file) == 'D') {
			if (fgetc(this->file) == '3') {

				int i;

				if (fseeko(this->file, (off_t)3, SEEK_CUR) == -1) {
					return 0;
				}

				for (i=0; i < 4; i++) {
					ret = (ret << 7) | fgetc(this->file);
				}

				return (off_t)(ret);
			}
		}
	}

	return 0;
}

int MediaMp3::calculateTagSize(char* in_buffer)
{
	int size = 0;
	int i = 0;

	for (i = 0; i < 4; i++) {
		size += ((int)in_buffer[i]) * pow(255, (3 - i));
	}

	return size;
}

const char* binary_to_enc (char val)
{
	const char *retval;

	if (val == 0x00) {
		retval = "ISO8859-1";
	} else if (val == 0x01) {
		retval = "UTF-16";
	} else if (val == 0x02) {
		retval = "UTF-16BE";
	} else if (val == 0x03) {
		retval = "UTF-8";
	} else {
		throw "UNKNOWN id3v2.4 encoding!";
		retval = NULL;
	}
	return retval;
}

bool MediaMp3::findMetadata(Metadata* meta)
{
	if (fseeko(this->file, 0, SEEK_SET) == -1) {
		return false;
	}

	TagLib::FileRef f(this->getLocation());

	TagLib::String artist = f.tag()->artist();
	char* cArtist = (char*)malloc(sizeof(char) * (artist.length() + 1));
	memset(cArtist, (int)'\0', (artist.length() + 1));
	wcstombs(cArtist, artist.toCWString(), artist.length());
	meta->setArtist(cArtist);
	free(cArtist);

	TagLib::String title = f.tag()->title();
	char* cTitle = (char*)malloc(sizeof(char) * (title.length() + 1));
	memset(cTitle, (int)'\0', (title.length() + 1));
	wcstombs(cTitle, title.toCWString(), title.length());
	meta->setTitle(cTitle);
	free(cTitle);

	Mp3Frame* frame;
	float length = 0.0;
	int frames = 0;

	while ((frame = this->getNextFrame())) {
		length += frame->getLength();
		delete frame;
		frames++;
	}

	meta->setStart(0);
	meta->setLength((int)(length * 100.0));

	return true;

	/*off_t offset = 0;

	if ((offset = this->findId3v2()) != 0) {

		if (fseeko(this->file, 0, SEEK_SET) != -1) {

			int readed;
			int headerSize = 0;
			char *buffer = (char*)malloc(offset * sizeof(char));

			if ((readed = fread(buffer, 1, 9, this->file)) > 0) {
				headerSize = this->calculateTagSize(&buffer[6]);
			}

			fseeko(this->file, 0, SEEK_SET);

			if ((readed = fread(buffer, 1, offset, this->file)) > 0) {

				int pos = 10;
				int size = 0;

				while (pos < offset) {

					if (!strlen(&buffer[pos]) || strlen(&buffer[pos]) > 4) {
						break;
					}

					if ((size = this->calculateTagSize(&buffer[pos + 4])) <= 0) {
						pos++;
						continue;
					}

					char* field = (char*)malloc(size * sizeof(char));
					memset(field, '\0', size);

					if (strcmp(&buffer[pos], "TIT2") == 0) {
						strncpy(field, &buffer[pos + 11], size - 1);
						printf("Encoding: %s; %s\n", binary_to_enc(buffer[pos + 10]), field);
			printf("konwersja %d :: %s -> %s\n", wcslen((wchar_t*)&buffer[pos + 11]), (wchar_t*)&buffer[pos + 11], field);
						meta->setTitle(field);
			
					} else if (strcmp(&buffer[pos], "TPE1") == 0) {
						strncpy(field, &buffer[pos + 11], size - 1);
						meta->setArtist(field);
					} else if (strcmp(&buffer[pos], "TALB") == 0) {
						strncpy(field, &buffer[pos + 11], size - 1);
						meta->setAlbum(field);
					} else if (strcmp(&buffer[pos], "TDRC") == 0 || strcmp(&buffer[pos], "TYER") == 0 ) {
						strncpy(field, &buffer[pos + 11], size - 1);
						meta->setYear(atoi(field));
					} else if (strcmp(&buffer[pos], "TCON") == 0) {
						strncpy(field, &buffer[pos + 11], size - 1);
						meta->setGenre(field);
					} else if (strcmp(&buffer[pos], "COMM") == 0) {
						int i = pos + 14;
						field = (char*)malloc((headerSize - (pos + 15)) * sizeof(char));
						memset(field, '\0', size);
						while (i++ < headerSize && buffer[i] != '\0') {
							if (!buffer[i + 4]) {
								if ((size = this->calculateTagSize(&buffer[i + 6])) > 0) {
									if (i + 6 + size >= headerSize || !buffer[i + 6 + size + 4]) {
										break;
									}
								}
							}
							strncat(field, &buffer[i], 1);
						}
						meta->setComment(field);
						pos = i;
						size = -10;
					} else {
						//strncpy(field, &buffer[pos + 11], size - 1);
						//printf("%s: (%i) %s\n", &buffer[pos], size, field);
					}

					free(field);

					pos += size + 10;
				}
			}

			free(buffer);
		}

		return true;
	}

	if ((offset = this->findId3v1()) != 0) {

		if (fseeko(this->file, offset, SEEK_END) != -1) {

			int readed;
			char *buffer;
			char* field;

			buffer = (char*)malloc(sizeof(char) * 128);
			memset(buffer, (int)'\0', sizeof(char) * 128);

			if ((readed = fread(buffer, 1, 128, this->file)) > 0) {

				field = (char*)malloc(31 * sizeof(char));
				memset(field, (int)'\0', 31 * sizeof(char));
				strncpy(field, &buffer[3], 30);
				meta->setTitle(field);
				free(field);

				field = (char*)malloc(31 * sizeof(char));
				memset(field, (int)'\0', 31 * sizeof(char));
				strncpy(field, &buffer[33], 30);
				meta->setArtist(field);
				free(field);

				field = (char*)malloc(31 * sizeof(char));
				memset(field, (int)'\0', 31 * sizeof(char));
				strncpy(field, &buffer[63], 30);
				meta->setAlbum(field);
				free(field);

				field = (char*)malloc(5 * sizeof(char));
				memset(field, (int)'\0', 5 * sizeof(char));
				strncpy(field, &buffer[93], 4);
				meta->setYear(2000);
				free(field);

				field = (char*)malloc(31 * sizeof(char));
				memset(field, (int)'\0', 31 * sizeof(char));
				strncpy(field, &buffer[97], 30);
				meta->setComment(field);
				free(field);
			}

			free(buffer);
		}

		return true;
	}*/

	return false;
}

Mp3Frame* MediaMp3::getNextFrame()
{
	int readed;
	char *buffer = (char*)malloc(sizeof(char) * 4);

	while ((readed = fread(buffer, 1, 3, this->file))) {

		if (readed < 3) {
			break;
		}

		// check frame sync
		if ((buffer[0] & 0xFF) != 0xFF || (buffer[1] & 0xE0) != 0xE0) {
			continue;
		}

		// check frequency validity
		if (((buffer[2] >> 2) & 0x3) == 0x3) {
			continue;
		}

		// check bitrate validity
		if (((buffer[2] >> 4) & 0x0F) == 0xF) {
			continue;
		}

		// check layer validity
		if (((buffer[1] >> 1) & 3) != 1) {
			continue;
		}

		Mp3Frame* frame = new Mp3Frame(buffer, this->file);
		free(buffer);

		return frame;
	}

	free(buffer);

	return NULL;
}

Mp3Frame::Mp3Frame(char* data, FILE* file)
{
	this->layer = (data[1] >> 1) & 3;

	if (data[1] & 0x10) {
		this->version = ((data[1] >> 3) & 1);
	} else {
		this->version = 2;
	}

	this->frequency = frequencies[this->version][(data[2] >> 2) & 0x3];
	this->bitrate = bitrates[this->version & 1][3 - this->layer][(data[2] >> 4) & 0x0F];
	this->length = ((this->version == 1 ? 1152.0 : 576.0) / (float)this->frequency);

	int dataLength = floor((144 * (this->bitrate * 1000)) / this->frequency) + ((data[2] >> 1) & 0x1);

	this->data = (char*)malloc(sizeof(char) * dataLength - 3);
	fread(this->data, 1, dataLength - 3, file);
}

int Mp3Frame::getVersion()
{
	return this->version;
}

int Mp3Frame::getFrequency()
{
	return this->frequency;
}

int Mp3Frame::getBitrate()
{
	return this->bitrate;
}

float Mp3Frame::getLength()
{
	return this->length;
}

Mp3Frame::~Mp3Frame()
{
	if (this->data) {
		free(this->data);
	}
}
