#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_archivator.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Error: not enough arguments");
        return -1;
    }
    char** fileNames = malloc((argc - 4) * sizeof(char*));
    int curFileName = 0;

    char* arcName = NULL;

    char createFlag = 0;
    char extractFlag = 0;
    char listFlag = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp("--file", argv[i]) == 0) {
            arcName = argv[i + 1];
            i++;
            continue;
        }
        else if (strcmp("--create", argv[i]) == 0) {
            createFlag = 1;
        }
        else if (strcmp("--extract", argv[i]) == 0) {
            extractFlag = 1;
        }
        else if (strcmp("--list", argv[i]) == 0) {
            listFlag = 1;
        }
        else {
            fileNames[curFileName++] = argv[i];
        }
    }

    if (arcName == NULL) {
        printf("Error: not enough args");
        return -1;
    }
    if (createFlag)
        writeArchive(arcName, packArchive(curFileName, fileNames));
    else if (extractFlag)
        extractArchive(readArchive(arcName));
    else if (listFlag) {
        Archive* arch = readArchive(arcName);
        for (int i = 0; i < arch->filesCount; i++) {
            printf("Name: %s; size: %d\n", arch->files[i]->fileName, arch->files[i]->dataSize);
        }
    }

}