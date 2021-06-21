#ifndef _MYGUI_HEADER_
#define _MYGUI_HEADER_

#include <stdbool.h>
#include <stdarg.h>
#include <SFML/Graphics.h>
#include "glad/glad.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#include <nuklear.h>

/* --------------------------------------------- Macros --------------------------------------------- */

// macro get nk_color from cfg file. Ex: get_color_cfg_nk("body")
#define get_color_cfg_nk(style_option) nk_rgba_hex(style_get(style_option, char*))

/* --------------------------------------------- Structs -------------------------------------------- */

typedef struct{
    GLint image_map_opengl_id;
    struct button_t{
        struct nk_image minimize;
        struct nk_image maximize;
        struct nk_image close;
    }button;
}texture_t;

/* --------------------------------------------- Functions ------------------------------------------ */

// get a string width accordingly with the context font
float get_text_width(struct nk_context *context, char *text);

// main gui routine
void mygui(struct nk_context *context, sfRenderWindow *window);

#endif
