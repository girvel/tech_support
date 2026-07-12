#pragma once

typedef enum {
    PROGRAM_PROCEED,
    PROGRAM_EXIT,
} ProgramStatus;

typedef struct {
    const char *name;
    void *(*init)(int, char**);
    ProgramStatus (*update)(void *);
} Program;

