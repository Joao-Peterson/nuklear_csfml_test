#ifndef _SETTINGS_HEADER_
#define _SETTINGS_HEADER_

#include "doc.h"
#include "doc_json.h"

// program settings, themes, font, paths, etc.
extern doc *pcfg;

// doc pointer to theme style
extern doc *style;

// macro for getting values from doc structures
#define cfg_get(name, type) (*(type*)((void*)doc_get_ptr(pcfg, name) + sizeof(doc)))

// macro for getting styles
#define style_get(name, type) (*(type*)((void*)doc_get_ptr(style, name) + sizeof(doc)))

// init configurations, return 0 if succeded, 1 if failed.
int cfg_init(char *cfg_filename);

// end settings and save cfg file
void cfg_end(char *filename, bool window_fullscreen, int window_x, int window_y, int window_h, int window_w);

// save data
void cfg_save(char *filename, bool window_fullscreen, int window_x, int window_y, int window_h, int window_w);

#endif