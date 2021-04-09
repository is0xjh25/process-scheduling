/*
COMP30023 Project One (main.c)
Created by Yun-Chi Hsiao (student ID: 1074004) 
Mar 20th, 2021
*/

#include "utilities.h"
#include "scheduler.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************/
int main(int argc, char *argv[]) {
    
    // default setting
    FILE *file = NULL;
    char *file_url = NULL;
    int num_processor = 0;
    int advanced_mode = FALSE;

    read_input(argc, argv, &file_url, &num_processor, &advanced_mode);

    file = open_file(file_url);

    if (advanced_mode) {
        advanced_processor_scheduler(file, num_processor, advanced_mode);
    } else if (!advanced_mode) {
        default_processor_scheduler(file, num_processor, advanced_mode);
    }

    fclose(file);

    return 0;
}
