//
// Created by stefan on 31.10.24.
//

#include "tools.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <uuid/uuid.h>

int mkdir_p(const char *path, const mode_t mode) {
    auto p = (char*)path;
    if (p[strlen(path) - 1] == '/') {
        p[strlen(path) - 1] = '\0';
    }
    p = strchr(path + 1, '/');
    int r = 0;
    while(p != nullptr) {
        *p = '\0';
        r = mkdir(path, mode);
        *p = '/';
        if(r != 0 && errno != EEXIST) {
            return r;
        }
        p = strchr(p + 1, '/');
    }
    r = mkdir(path, mode);
    if(r != 0 && errno != EEXIST) {
        return r;
    }
    errno = 0;
    return 0;
}


bool dir_exists(const char *path) {
    DIR *dir = opendir(path);
    if (dir) {
        closedir(dir);
        return true;
    }
    return false;
}

bool file_exists(const char *path) {
    if (access(path, F_OK) != 0) {
        return false;
    }
    return true;
}

const char * get_real_path(const char *path) {
    if (path == nullptr) {
        return nullptr;
    }
    static char realPath[PATH_MAX];
    realpath(path, realPath);
    return realPath;
}

const char* get_temp_dir() {
    const char *val = getenv("TMPDIR");
    if (val == nullptr || strlen(val) == 0) {
        val = getenv("TMPDIR");
    }
    if (val == nullptr || strlen(val) == 0) {
        val = getenv("TMP");
    }
    if (val == nullptr || strlen(val) == 0) {
        val = getenv("TEMP");
    }
    if (val == nullptr || strlen(val) == 0) {
        val = getenv("TEMPDIR");
    }
    size_t len = 0;
    bool addSlash = false;
    if (val != nullptr && strlen(val) > 0) {
        len = strlen(val);
        if (val[len - 1] != '/') {
            len++;
            addSlash = true;
        }
    } else {
        len = 5;
    }
    static char result[PATH_MAX];
    if (val != nullptr) {
        strcpy(result, val);
        if (addSlash) {
            result[len] = '/';
        }
    } else {
        strcpy(result, "/tmp/");
    }
    return result;
}

void alloc_string(char ** string, const size_t size) {
    if (string == nullptr) {
        fprintf(stderr, "alloc_string: null pointer error\n");
        exit(EXIT_FAILURE);
    }
    *string = malloc((size + 1) * sizeof(char));
    if (*string == nullptr) {
        fprintf(stderr, "alloc_string: string alloc failed\n");
        exit(EXIT_FAILURE);
    }
}

void free_string(char ** string) {
    if (string == nullptr) {
        fprintf(stderr, "free_string: null pointer error\n");
        exit(EXIT_FAILURE);
    }
    if (*string == nullptr) {
        fprintf(stderr, "free_string: null pointer error\n");
        exit(EXIT_FAILURE);
    }
    free(*string);
    *string = nullptr;
}

const char* get_file_name(const char* filePath) {
    if (filePath == nullptr) {
        fprintf(stderr, "cscript: null pointer error\n");
        exit(EXIT_FAILURE);
    }
    const char* p = strrchr(filePath, '/');
    if (p == nullptr) {
        return filePath;
    }
    return p + 1;
}

uint32_t swapByteOrder(const uint32_t word) {
    return
        (word & 0x000000ff) << 24 |
        (word & 0x0000ff00) <<  8 |
        (word & 0x00ff0000) >>  8 |
        (word & 0xff000000) >> 24;
}

uint32_t shiftLeft(const uint32_t word, const uint32_t shift) {
    return word << shift;
}

uint32_t shiftRight(const uint32_t word, const uint32_t shift) {
    return word >> shift;
}

uint32_t rotateRight(const uint32_t word, const uint32_t shift) {
    return shiftRight(word, shift) | shiftLeft(word, 32 - shift);
}

uint32_t rotateLeft(const uint32_t word, const uint32_t shift) {
    return shiftLeft(word, shift) | shiftRight(word, 32 - shift);
}
