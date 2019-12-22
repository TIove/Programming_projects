#ifndef PROGA15_FUNC_ARCHIVATOR_H
#define PROGA15_FUNC_ARCHIVATOR_H
#include "arcivator.h"

typedef struct {
    unsigned filesCount;
    ArcFile** files;
} Archive;

Archive* readArchive(char* fileName);
void writeArchive(char* fileName, Archive* arch);
Archive* packArchive(unsigned filesCount, char** fileNames);
void extractArchive(Archive* arch);
#endif //PROGA15_FUNC_ARCHIVATOR_H