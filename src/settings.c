#include "settings.h"
#include <stdbool.h>
#include <stdint.h>

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

// save and terminate settings
void cfg_end(char *filename, bool window_fullscreen, bool window_maximized, int window_x, int window_y, int window_h, int window_w){
    doc_set(pcfg, "window.size.fullscreen", bool, window_fullscreen);
    doc_set(pcfg, "window.size.maximized", bool, window_maximized);
    doc_set(pcfg, "window.size.x", int, window_x);
    doc_set(pcfg, "window.size.y", int, window_y);
    doc_set(pcfg, "window.size.w", int, window_w);
    doc_set(pcfg, "window.size.h", int, window_h);

    doc_json_save(pcfg, filename);
    doc_delete(pcfg, ".");
}

// save settings
void cfg_save(char *filename, bool window_fullscreen, bool window_maximized, int window_x, int window_y, int window_h, int window_w){
    doc_set(pcfg, "window.size.fullscreen", bool, window_fullscreen);
    doc_set(pcfg, "window.size.maximized", bool, window_maximized);
    doc_set(pcfg, "window.size.x", int, window_x);
    doc_set(pcfg, "window.size.y", int, window_y);
    doc_set(pcfg, "window.size.w", int, window_w);
    doc_set(pcfg, "window.size.h", int, window_h);
}
