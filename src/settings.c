#include "settings.h"

// program settings, themes, font, paths, etc.
doc *pcfg;

// doc pointer to theme style
doc *style;

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