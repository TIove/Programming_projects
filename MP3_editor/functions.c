#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int reverseBytes(unsigned n) {
    return ((n >> 24) & 0x000000ff) |
           ((n >> 8) & 0x0000ff00) |
           ((n << 8) & 0x00ff0000) |
           ((n << 24) & 0xff000000);
}

char isCorrectFrame(FrameHeader* frameHeader) {
    if (frameHeader->frameID[0] == 0 &&
        frameHeader->frameID[1] == 0 &&
        frameHeader->frameID[2] == 0 &&
        frameHeader->frameID[3] == 0)
        return 0;
    if (frameHeader->sizeReversed == 0)
        return 0;
    return 1;
}

MP3File* readMP3(char* fileName) {
    FILE* file = fopen(fileName, "rb");
    if (file == NULL)
        return NULL;

    MP3File* mp3 = (MP3File*) malloc(sizeof(MP3File));
    mp3->fileName = fileName;
    mp3->tagHeader = (ID3v2Header*) malloc(sizeof(MP3File));
    fread(mp3->tagHeader, sizeof(ID3v2Header), 1, file);

    if ((mp3->tagHeader->flags >> 6) % 2) {
        mp3->extendedTagHeader = (ID3v2ExtendedHeader*) malloc(sizeof(ID3v2ExtendedHeader));
        fread(mp3->extendedTagHeader, sizeof(ID3v2ExtendedHeader), 1, file);
    }
    else {
        mp3->extendedTagHeader = NULL;
    }

    unsigned tagSize = reverseBytes(mp3->tagHeader->sizeReversed);

    mp3->frameCount = 0;
    mp3->frames = NULL;
    while (ftell(file) < tagSize + sizeof(ID3v2Header)) {
        Frame* curFrame = malloc(sizeof(Frame));
        curFrame->frameHeader = malloc(sizeof(FrameHeader));
        fread(curFrame->frameHeader, sizeof(FrameHeader), 1, file);
        if (!isCorrectFrame(curFrame->frameHeader))
            break;
        size_t frameDataSize = reverseBytes(curFrame->frameHeader->sizeReversed);
        curFrame->frameData = malloc(frameDataSize * sizeof(char));
        fread(curFrame->frameData, sizeof(char), frameDataSize, file);
        mp3->frames = realloc(mp3->frames, ++mp3->frameCount * sizeof(Frame));
        mp3->frames[mp3->frameCount - 1] = curFrame;
    }
    mp3->dataSize = tagSize + sizeof(ID3v2Header) - ftell(file);
    mp3->data = malloc(mp3->dataSize * sizeof(char));
    fread(mp3->data, sizeof(char), mp3->dataSize, file);
    fclose(file);
    return mp3;
}

char updateMP3(MP3File* mp3) {
    FILE* file = fopen(mp3->fileName, "r+b");
    if (file == NULL)
        return 0x1F;
    fwrite(mp3->tagHeader, sizeof(ID3v2Header), 1, file);
    if (mp3->extendedTagHeader != NULL)
        fwrite(mp3->extendedTagHeader, sizeof(ID3v2ExtendedHeader), 1, file);
    for (int i = 0; i < mp3->frameCount; i++) {
        fwrite(mp3->frames[i]->frameHeader, sizeof(FrameHeader), 1, file);
        fwrite(mp3->frames[i]->frameData, sizeof(char), reverseBytes(mp3->frames[i]->frameHeader->sizeReversed), file);
    }
    fwrite(mp3->data, sizeof(char), mp3->dataSize, file);
    fclose(file);
}