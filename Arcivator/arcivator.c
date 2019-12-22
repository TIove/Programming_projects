#include "arcivator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ArcFile* readFile(char* fileName) {
    FILE* file = fopen(fileName, "rb");
    if (file == NULL)
        return NULL;
    ArcFile* arcFile = malloc(sizeof(ArcFile));
    arcFile->fileNameSize = strlen(fileName) + 1;
    arcFile->fileName = malloc(arcFile->fileNameSize * sizeof(char));
    strcpy(arcFile->fileName, fileName);
    arcFile->fileName[arcFile->fileNameSize - 1] = '\0';
    fseek(file, 0, SEEK_END);
    arcFile->dataSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    arcFile->data = malloc(arcFile->dataSize * sizeof(char));
    fread(arcFile->data, sizeof(char), arcFile->dataSize, file);
    fclose(file);
    return arcFile;
}

void writeFile(ArcFile* arcFile) {
    FILE* file = fopen(arcFile->fileName, "wb");
    if (file == NULL)
        return;
    fwrite(arcFile->data, sizeof(char), arcFile->dataSize, file);
    fclose(file);
}