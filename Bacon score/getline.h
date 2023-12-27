//
// Created by umimu on 11/4/2023.
//


#ifndef GITHUB_GETLINE_H
#define GITHUB_GETLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

// if typedef doesn't exist (msvc, blah)
typedef intptr_t ssize_t;

ssize_t getline(char **lineptr, size_t *n, FILE *stream);

#endif //GITHUB_GETLINE_H
