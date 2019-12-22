#include "func_archivator.h"
#include <stdio.h>
#include <stdlib.h>

Archive* readArchive(char* fileName) {
    FILE* file = fopen(fileName, "rb");
    if (file == NULL)
        return NULL;
    Archive* arch = malloc(sizeof(Archive));

    fread(&arch->filesCount, sizeof(arch->filesCount), 1, file);
    arch->files = malloc(arch->filesCount * sizeof(ArcFile*));
    for (int i = 0; i < arch->filesCount; i++) {
        arch->files[i] = malloc(sizeof(ArcFile));
        fread(&arch->files[i]->fileNameSize, sizeof(unsigned short), 1, file);
        arch->files[i]->fileName = malloc(arch->files[i]->fileNameSize * sizeof(char));
        fread(arch->files[i]->fileName, sizeof(char), arch->files[i]->fileNameSize, file);
        fread(&arch->files[i]->flags, sizeof(char), 1, file);
        fread(&arch->files[i]->dataSize, sizeof(unsigned), 1, file);
        arch->files[i]->data = malloc(arch->files[i]->dataSize * sizeof(char));
        fread(arch->files[i]->data, sizeof(char), arch->files[i]->dataSize, file);
    }
    fclose(file);
    return arch;
}

void writeArchive(char* fileName, Archive* arch) {
    FILE* file = fopen(fileName, "wb");
    fwrite(&arch->filesCount, sizeof(arch->filesCount), 1, file);
    for (int i = 0; i < arch->filesCount; i++) {
        fwrite(&arch->files[i]->fileNameSize, sizeof(arch->files[i]->fileNameSize), 1, file);
        fwrite(arch->files[i]->fileName, sizeof(char), arch->files[i]->fileNameSize, file);
        fwrite(&arch->files[i]->flags, sizeof(arch->files[i]->flags), 1, file);
        fwrite(&arch->files[i]->dataSize, sizeof(arch->files[i]->dataSize), 1, file);
        fwrite(arch->files[i]->data, sizeof(char), arch->files[i]->dataSize, file);
    }
    fclose(file);
}

Archive* packArchive(unsigned filesCount, char** fileNames) {
    Archive* arch = malloc(sizeof(Archive));
    arch->filesCount = filesCount;
    arch->files = malloc(filesCount * sizeof(ArcFile));
    for (int i = 0; i < filesCount; i++) {
        arch->files[i] = readFile(fileNames[i]);
        if (arch->files[i] == NULL) {
            free(arch->files);
            free(arch);
            return NULL;
        }
    }
    return arch;
}

void extractArchive(Archive* arch) {
    for (int i = 0; i < arch->filesCount; i++)
        writeFile(arch->files[i]);
}