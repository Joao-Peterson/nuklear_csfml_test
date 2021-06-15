#ifndef _SETTINGS_HEADER_
#define _SETTINGS_HEADER_

#include "doc.h"
#include "doc_json.h"

// program settings, themes, font, paths, etc.
extern doc *pcfg;

// doc pointer to theme style
extern doc *settings_style;

// macro for getting values from doc structures
#define cfg_get(name, type) (*(type*)((void*)doc_get_ptr(pcfg, name) + sizeof(doc)))

// macro for getting styles
#define style_get(name, type) (*(type*)((void*)doc_get_ptr(settings_style, name) + sizeof(doc)))

// define settings
#define cfg_set(name, type, new_value, ...) \
    if(__check_string_bindata(doc_get_ptr(pcfg, name))){ \
        ((doc_bindata*)doc_get_ptr(pcfg, name))->len = strtoull(#__VA_ARGS__, NULL, 10); \
    } \
    *(type*)((void*)__check_obj_is_value(doc_get_ptr(pcfg,name)) + sizeof(doc)) = new_value

// define style
#define style_set(name, type, new_value, ...) \
    if(__check_string_bindata(doc_get_ptr(settings_style, name))){ \
        ((doc_bindata*)doc_get_ptr(settings_style, name))->len = strtoull(#__VA_ARGS__, NULL, 10); \
    } \
    *(type*)((void*)__check_obj_is_value(doc_get_ptr(settings_style,name)) + sizeof(doc)) = new_value

// init configurations, return 0 if succeded, 1 if failed.
int cfg_init(char *cfg_filename);

// end settings and save cfg file
void cfg_end(void);

#endif