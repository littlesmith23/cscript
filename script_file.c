/**
* @file script_file.c
 * @author Stefan Kleinschmiodt
 * @date 13. Nov 2024
 * @brief Contains the script file related function for cscript.
 *
 * Provides a handle to an internal structure managing the information
 * around a c script as well as function to load, manage and execute.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <linux/limits.h>

#include "script_file.h"
#include "script_file_type.h"
#include "tools.h"
#include "sha256.h"

sf_handle script_file_open(const char* file_path) {
    size_t len = 0;
    char * line = nullptr;

    if (file_path == nullptr) {
        fprintf(stderr, "script_file_open: file_path must not be null/n");
        exit(EXIT_FAILURE);
    }

    //Create an empty script_file structure
    const auto sf = (script_file*)malloc(sizeof(script_file));
    sf->file_path[0] = '\0';
    sf->file_name[0] = '\0';
    sf->gcc_args[0] = '\0';
    sf->executable_path[0] = '\0';
    sf->source_path[0] = '\0';
    sf->hash[0] = '\0';
    sf->start_line = 1;

    //Put the provided file path into the script_file structure
    strcpy(sf->file_path, file_path);
    //Extract the file name from the path and put it into the script_file structure
    strcpy(sf->file_name, get_file_name(file_path));
    //Create the hash for the path and put it into the script_file structure
    sprintf(sf->hash, "%s", sha256_file(sf->file_path));
    //Write the path where the source code will be extracted to
    sprintf(sf->source_path, "%s/%s.c", get_temp_dir(), sf->file_name);

    //Open the script file
    FILE *fpScriptFile = fopen(file_path, "r");
    if (fpScriptFile == nullptr) {
        fprintf(stderr, "script_file_open: could not open %s/n", file_path);
        exit(EXIT_FAILURE);
    }
    //Read the first line and check if it is a shebang line
    alloc_string(&line, 16384);
    if (getline(&line, &len, fpScriptFile) != -1) {
        if (strncmp("#!", line, 2) != 0) {
            fprintf(stderr, "script_file_open: wrong format in line 1:\n%s/n", line);
            fclose(fpScriptFile);
            free_string(&line);
            exit(EXIT_FAILURE);
        }
    }
    //Read the second line and check if it is a #gcc line
    if (getline(&line, &len, fpScriptFile) != -1) {
        if (strncmp("#gcc ", line, 5) == 0) {
            line[strlen(line) - 1] = '\0';
            strcpy(sf->gcc_args, line + 5);
            sf->start_line = 2;
        }
    }
    fclose(fpScriptFile);
    free_string(&line);

    return sf;
}

void extract_code(sf_handle handle) {
    const auto sf = (script_file*)handle;
    if (sf == nullptr) {
        fprintf(stderr, "extract_code: handle must not be null/n");
        exit(EXIT_FAILURE);
    }
    char * line = nullptr;
    size_t len = 0;
    //Open the script file for reading
    FILE *fpScriptFile = fopen(sf->file_path, "r");
    if (fpScriptFile == nullptr) {
        fprintf(stderr, "compile: could not open %s/n", sf->file_path);
        exit(EXIT_FAILURE);
    }
    //Open the source file for writing
    FILE *fpCFile = fopen(sf->source_path, "w");
    if (fpCFile == nullptr) {
        fprintf(stderr, "compile: could not open %s/n", sf->source_path);
        fclose(fpScriptFile);
        exit(EXIT_FAILURE);
    }
    //Write all lines from the script file(except shebang and #gcc) into
    //the source file.
    int l = 0;
    alloc_string(&line, 16384);
    while (getline(&line, &len, fpScriptFile) != -1) {
        if (l++ < sf->start_line) continue;
        fputs(line, fpCFile);
    }
    fclose(fpScriptFile);
    fclose(fpCFile);
    free_string(&line);
}

void script_file_set_executable_path(sf_handle handle, const char* path) {
    const auto sf = (script_file*)handle;
    if (sf == nullptr) {
        fprintf(stderr, "set_executable_path: handle must not be null/n");
        exit(EXIT_FAILURE);
    }
    if (path == nullptr) {
        fprintf(stderr, "set_executable_path: path must not be null/n");
        exit(EXIT_FAILURE);
    }
    sprintf(sf->executable_path, "%s/%s.bin", path, sf->file_name);

}

void script_file_compile(sf_handle handle) {
    const auto sf = (script_file*)handle;
    if (sf == nullptr) {
        fprintf(stderr, "compile: handle must not be null/n");
        exit(EXIT_FAILURE);
    }
    extract_code(sf);
    char gcc_line[PATH_MAX];
    //Create the gcc command line
    sprintf(gcc_line, "gcc %s -o %s %s", sf->gcc_args, sf->executable_path, sf->source_path);
#if DEBUG == 1
    printf("GCC: %s\n", gcc_line);
#endif
    //Execute the command line
    if (system(gcc_line) != 0) {
        //Delete the source file from the temp folder
        unlink(sf->source_path);
        fprintf(stderr, "compile: failed compiling %s/n", sf->file_name);
        exit(EXIT_FAILURE);
    }
    //Delete the source file from the temp folder
    unlink(sf->source_path);
}
void script_file_execute(sf_handle handle, int argc, char** argv) {
    const auto sf = (script_file*)handle;
    if (sf == nullptr) {
        fprintf(stderr, "compile: handle must not be null/n");
        exit(EXIT_FAILURE);
    }
    char binLine[PATH_MAX] = "";
    //Create the command line
    strcpy(binLine, sf->executable_path);
    //Add the arguments to the command line
    for (int i = 2; i < argc; i++) {
        strcat(binLine, " ");
        strcat(binLine, argv[i]);
    }
#if DEBUG == 1
    printf("DBG: script_file_execute: binfile %s\n", binLine);
#endif
    //Execute the command line
    if (system(binLine) != 0) {
        fprintf(stderr, "cscript: failed executing %s/n", binLine);
        exit(EXIT_FAILURE);
    }
}

void script_file_dump(sf_handle handle) {
    const auto sf = (script_file*)handle;
    if (sf == nullptr) {
        fprintf(stderr, "compile: handle must not be null/n");
        exit(EXIT_FAILURE);
    }
    printf("    sf->file_path: %s\n", sf->file_path);
    printf("    sf->file_name: %s\n", sf->file_name);
    printf("    sf->hash: %s\n", sf->hash);
    printf("    sf->gcc_args: %s\n", sf->gcc_args);
    printf("    sf->source_path: %s\n", sf->source_path);
    printf("    sf->executable_path: %s\n", sf->executable_path);
    printf("    sf->start_line: %d\n", sf->start_line);
}
