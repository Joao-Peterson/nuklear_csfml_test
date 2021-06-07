#ifndef _SETTINGS_HEADER_
#define _SETTINGS_HEADER_

#include "doc.h"

// program settings, themes, font, paths, etc.
doc *pcfg;


// macro for getting values from doc structures
#define cfg_get(name, type) (*(type*)((void*)doc_get_ptr(pcfg, name) + sizeof(doc)))

#endif