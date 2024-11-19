//
// Created by stefan on 31.10.24.
//

#include "cache.h"
#include <dirent.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include "tools.h"
#include "sha256.h"
#include "script_file_type.h"

char cache_dir[PATH_MAX] = "";

void init_cache() {
    if (strlen(cache_dir) == 0) {
        sprintf(cache_dir, "%s/.cscript/cache/", getenv("HOME"));
        mkdir_p(cache_dir, 0700);
#if DEBUG == 1
        printf("DBG: init_cache: cache dir: %s\n", cache_dir);
#endif
    }
}

const char* get_cache_path(const char *filePath);

void cache_clear() {
    init_cache();
    char clear_cmd[4096];

    printf("clearing complete cscript cache\n%s", cache_dir);

    sprintf(clear_cmd, "rm -Rf %s", cache_dir);

    system(clear_cmd);
}

void cache_clear_single(sf_handle handle) {
    const auto sf = (script_file*)handle;
    if (sf == nullptr) {
        fprintf(stderr, "cache_clear_single: handle must not be null/n");
        exit(EXIT_FAILURE);
    }
    init_cache();
    const char *full_cache_path = get_cache_path(sf->file_path);
    printf("clearing single cscript cache\n%s", full_cache_path);
    printf("for script: %s", sf->file_name);
    char clear_cmd[4096];
    sprintf(clear_cmd, "rm -Rf %s", full_cache_path);
    system(clear_cmd);
}

bool cache_check(sf_handle handle) {
    const auto sf = (script_file*)handle;
    char * line = nullptr;
    size_t len = 0;
    if (sf == nullptr) {
        fprintf(stderr, "cache_check: handle must not be null/n");
        exit(EXIT_FAILURE);
    }
    init_cache();

    const char *full_cache_path = get_cache_path(sf->file_path);
    script_file_set_executable_path(sf, full_cache_path);
#if DEBUG == 1
    printf("DBG: cache_check: sf->executable_path: %s\n", sf->executable_path);
#endif

    if (!dir_exists(full_cache_path)) {
        mkdir_p(full_cache_path, 0700);
#if DEBUG == 1
        printf("DBG: cache_check: return false\n");
#endif
        return false;
    }
    char hash_file[PATH_MAX];
    sprintf(hash_file, "%s/hash", full_cache_path);
    FILE *fp = fopen(hash_file, "r");
    if (!fp) {
#if DEBUG == 1
        printf("DBG: cache_check: return false\n");
#endif
        return false;
    }
    bool result = false;
    if (getline(&line, &len, fp) != -1) {
#if DEBUG == 1
        printf("DBG: loaded hash    : %s\n", line);
        printf("DBG: calculated hash: %s\n", sf->hash);
#endif
        if (strcmp(line, sf->hash) == 0) {
            result = true;
        }
    }
    fclose(fp);
#if DEBUG == 1
    printf("DBG: cache_check: return %s\n", result ? "true" : "false");
#endif
    return result;
}

void cache_update(sf_handle handle) {
    const auto sf = (script_file*)handle;
    char * line = nullptr;
    size_t len = 0;
    if (sf == nullptr) {
        fprintf(stderr, "cache_check: handle must not be null/n");
        exit(EXIT_FAILURE);
    }
    init_cache();

    const char *cache_path = get_cache_path(sf->file_path);
    if (!dir_exists(cache_path)) {
        mkdir_p(cache_path, 0700);
    }
    char hash_file[PATH_MAX];
    sprintf(hash_file, "%s/hash", cache_path);
    FILE *fp = fopen(hash_file, "w");
    if (!fp) {
        fprintf(stderr, "cache_update: could not write to hash file: %s/n", hash_file);
        exit(EXIT_FAILURE);
    }
    fputs(sf->hash, fp);
    fclose(fp);
#if DEBUG == 1
    printf("DBG: cache_update: wrote hash file %s\n", hash_file);
#endif
}


const char* get_cache_path(const char *filePath) {
    static char cache_path[PATH_MAX];
    const char *fullPath = get_real_path(filePath);
    char* hash = sha256_string(fullPath);
    sprintf(cache_path, "%s/%s", cache_dir, hash);
#if DEBUG == 1
    printf("DBG: get_cache_path: cache path: %s\n", cache_path);
#endif
    return cache_path;
}
