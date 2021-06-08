#ifndef _SETTINGS_HEADER_
#define _SETTINGS_HEADER_

#include "doc.h"
#include "doc_json.h"

// program settings, themes, font, paths, etc.
doc *pcfg;

// doc pointer to theme style
doc *style;

// macro for getting values from doc structures
#define cfg_get(name, type) (*(type*)((void*)doc_get_ptr(pcfg, name) + sizeof(doc)))

// macro for getting styles
#define style_get(name, type) (*(type*)((void*)doc_get_ptr(style, name) + sizeof(doc)))

// init configurations, return 0 if succeded, 1 if failed.
int cfg_init(char *cfg_filename){
    pcfg = doc_json_open("./cfg.json");                                             // user settings
    if(pcfg == NULL){
        printf("File \"cfg.json\" is missing.\n");
        return 1;
    }

    char theme[200];
    snprintf(theme, 200, "theme.%s", doc_get(pcfg, "theme.active", char*));
    style = doc_get_ptr(pcfg, theme);
    if(style == NULL){
        printf("Invalid style.\n");
        return 1;
    }

    return 0;
}

#endif