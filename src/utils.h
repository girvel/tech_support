#pragma once

#define size(ARR) sizeof(ARR)/sizeof(*ARR)

#define foreach(VARNAME, COLLECTION) \
    for (__typeof__((COLLECTION)) _foreach_collection = COLLECTION, VARNAME = _foreach_collection; \
         VARNAME < _foreach_collection + arrlen(_foreach_collection); \
         VARNAME++)

#define forchar(VARNAME, COLLECTION) \
    for (__typeof__((COLLECTION)) VARNAME = (COLLECTION); *VARNAME; VARNAME++)

#define forarr(VARNAME, COLLECTION) \
    for (__typeof__((*COLLECTION)) *_forarr_collection = COLLECTION, *VARNAME = _forarr_collection; \
         VARNAME < _forarr_collection + size(COLLECTION); \
         VARNAME++)
