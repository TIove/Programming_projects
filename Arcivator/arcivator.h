#ifndef PROGA15_ARCIVATOR_H
#define PROGA15_ARCIVATOR_H

typedef struct {
    unsigned short fileNameSize;
    char* fileName;
    char flags;
    unsigned dataSize;
    char* data;
} ArcFile;

ArcFile* readFile(char* fileName);
void writeFile(ArcFile* arcFile);

#endif //PROGA15_ARCIVATOR_H