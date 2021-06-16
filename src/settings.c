#include "settings.h"
#include <stdbool.h>
#include <stdint.h>

// program settings, themes, font, paths, etc.
doc *pcfg;

// doc pointer to theme style
doc *settings_style;

// the filename
char *filename;

// init configurations, return 0 if succeded, 1 if failed.
int cfg_init(char *cfg_filename){
    filename = cfg_filename;
    pcfg = doc_json_open(cfg_filename);                                             // user settings
    if(pcfg == NULL){
        printf("File \"%s\" is missing.\n", cfg_filename);
        return 1;
    }

    char theme[200];
    snprintf(theme, 200, "theme.array.%s", doc_get(pcfg, "theme.active", char*));
    settings_style = doc_get_ptr(pcfg, theme);
    if(settings_style == NULL){
        printf("Invalid style.\n");
        return 1;
    }

    return 0;
}

// save and terminate settings
void cfg_end(void){
    doc_json_save(pcfg, filename);
    doc_delete(pcfg, ".");
}
