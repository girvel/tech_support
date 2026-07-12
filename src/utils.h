#pragma once

#define foreach(VARNAME, COLLECTION) \
    for (__typeof__((COLLECTION)) _foreach_collection = COLLECTION, VARNAME = _foreach_collection; \
         VARNAME < _foreach_collection + arrlen(_foreach_collection); \
         VARNAME++)

#define forchar(VARNAME, COLLECTION) \
    for (__typeof__((COLLECTION)) VARNAME = (COLLECTION); \
         *VARNAME; \
         VARNAME++)
