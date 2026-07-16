#pragma once

#define size(ARR) sizeof(ARR)/sizeof(*ARR)

#define foreach(VARNAME, COLLECTION) \
    for (__typeof__((COLLECTION)) _foreach_collection = COLLECTION, VARNAME = _foreach_collection; \
         VARNAME < _foreach_collection + arrlen(_foreach_collection); \
         VARNAME++)

// __typeof__((COLLECTION + SIZE)) covers the case of int COLLECTION & size_t SIZE
#define forn(VARNAME, COLLECTION, SIZE) \
    for (__typeof__((COLLECTION + SIZE)) _forn_collection = COLLECTION, VARNAME = _forn_collection; \
         VARNAME < _forn_collection + SIZE; \
         VARNAME++)

#define forchar(VARNAME, COLLECTION) \
    for (__typeof__((COLLECTION)) VARNAME = (COLLECTION); *VARNAME; VARNAME++)

#define forarr(VARNAME, COLLECTION) \
    for (__typeof__((*COLLECTION)) *_forarr_collection = COLLECTION, *VARNAME = _forarr_collection; \
         VARNAME < _forarr_collection + size(COLLECTION); \
         VARNAME++)

#define rgb(HEX) (Color) {HEX / 0x10000, (HEX / 0x100) % 0x100, HEX % 0x100, 255}

char *tprintf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
