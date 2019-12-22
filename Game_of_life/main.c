#include <stdio.h>
#include <stdlib.h>
#include "string.h"
int main(int argc, char* argv[]) {
    int count_of_gen = -1;
    int step_of_gen = 1;
    char *file_first_gen = ' ';
    char *file_name;
    for (int i = 2; i < argc; ++i) {
        if ((int)argv[i-1][2] == 105) {
            file_first_gen = argv[i];
        } 
        else if ((int)argv[i-1][2] == 111) {
            file_name = argv[i];
        }
        else if ((int)argv[i-1][2] == 109){
            step_of_gen = atoi(argv[i]);
        } 
        else if ((int)argv[i-1][2] == 100){
            count_of_gen = atoi(argv[i]);
        }
    }
    
    FILE* fp = fopen(file_first_gen, "rb");

    unsigned char info[138] = {};
    fread(info, sizeof(unsigned char), 138, fp);
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    int cnt = 0;
    int size = width * height * 3;
    char bf[10000] = "";
    unsigned char arr[size];
    fread(arr, sizeof(unsigned char),size,fp);
    unsigned char new_arr[height+2][width+2]; 
    
    for (int j = 1; j < height + 1; j++) {
        for (int k = 1; k < width + 1; ++k) {
            if (arr[cnt] < 129) 
                arr[cnt] = 0;
            if (arr[cnt] > 128)
                arr[cnt] = 255; 
            new_arr[j][k] = arr[cnt]; 
            cnt+=3;
        } 
    }
    
    if (count_of_gen == -1) {
        count_of_gen = 100;
    }
    unsigned char new_arr_2[height + 2][width + 2]; 
    
    for (int index_of_gen = 1; index_of_gen < count_of_gen + 1; ++index_of_gen) {
        new_arr[0][0] = new_arr[height][width];
        new_arr[0][width+1] = new_arr[height][1];
        new_arr[height+1][0] = new_arr[1][width];
        new_arr[height+1][width+1] = new_arr[1][1];
        for (int m = 1; m < height+1; ++m) {
            new_arr[0][m] = new_arr[height][m];
            new_arr[height+1][m] = new_arr[1][m];
            new_arr[m][0] = new_arr[m][width];
            new_arr[m][width+1] = new_arr[m][1];
        }
        for (int i = 1; i < height+1; ++i) {
            for (int j = 1; j < width+1; ++j) { 
                if (new_arr[i][j] == 0){
                    if (new_arr[i-1][j]+ new_arr[i-1][j-1] + new_arr[i][j-1] + new_arr[i+1][j-1]+ new_arr[i-1][j+1]+ new_arr[i+1][j]+ new_arr[i][j+1] + new_arr[i+1][j+1] >= 255*5
                        && new_arr[i-1][j]+ new_arr[i-1][j-1] + new_arr[i][j-1] + new_arr[i+1][j-1]+ new_arr[i-1][j+1]+ new_arr[i+1][j]+ new_arr[i][j+1] + new_arr[i+1][j+1] <= 255*6 ){
                        new_arr_2[i][j] = 0; } else {
                        new_arr_2[i][j] = 255; 
                    }
                } else{
                    if(new_arr[i-1][j]+ new_arr[i-1][j-1] + new_arr[i][j-1] + new_arr[i+1][j-1]+ new_arr[i-1][j+1]+
                       new_arr[i+1][j]+ new_arr[i][j+1] + new_arr[i+1][j+1] == 255*5) {
                        new_arr_2[i][j] = 0; } else {
                        new_arr_2[i][j] = 255;
                    } 
                }
            } 
        }
        for (int k = 0; k < height+2; ++k) { 
            for (int i = 0; i < width+2; ++i) {
                new_arr[k][i] = new_arr_2[k][i]; 
            }
        }
        unsigned char array[width*height];
        for (int k = 1; k < height+1; ++k) {
            for (int i = 1; i < width+1; ++i) { array[height*k+i] = new_arr_2[k][i];
            }
        }
        for (int l = 0; l < width*height ; ++l) { 
            arr[l*3] = array[l];
            arr[l*3+1] = array[l];
            arr[l*3+2] = array[l];
        }
        if (index_of_gen % step_of_gen == 0) {
            char file_result[10000] = {};
            strcat(file_result,file_name);
            char fl2[10000] = "/";
            strcat(fl2,bf);
            int i = 2;
            strcat(bf,"1");
            strcat(fl2, "11.bmp");
            strcat(file_result, fl2);
            for (int j = size - width * 30; j < size; ++j) {
                arr[j] = 255;
            }
            for (int k = 0; k < height * 30; ++k) {
                arr[k] = 255;
            }
            FILE *new_f = fopen(file_result, "wb");
            fwrite(info, sizeof(char), 138, new_f);
            fwrite(arr ,sizeof(char), size, new_f);
            fclose(new_f);
        }
    }
    fclose(fp);
    return 0;
}