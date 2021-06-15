#ifndef _MYGUI_WINDOWS_HEADER_
#define _MYGUI_WINDOWS_HEADER_

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

#include "settings.h"

/* --------------------------------------------- Macros --------------------------------------------- */

#define MYGUI_WINDOWS_FLAGS (                                           \
NK_WINDOW_TITLE  | NK_WINDOW_MOVABLE  | NK_WINDOW_SCALABLE         |    \
NK_WINDOW_BORDER | NK_WINDOW_CLOSABLE | NK_WINDOW_SCROLL_AUTO_HIDE | NK_WINDOW_MINIMIZABLE )

/* --------------------------------------------- Functions ------------------------------------------ */

struct nk_rect mygui_rect_window_centered_by_scale(int w_percent, int h_percent, int top_w, int top_h){
    struct nk_rect rect;
    rect.w = top_w * ( w_percent / 100.0);
    rect.h = top_h * ( h_percent / 100.0);
    rect.x = (top_w - rect.w) / 2.0;
    rect.y = (top_h - rect.h) / 2.0;
    return rect;
}

void mygui_example(struct nk_context *context, sfRenderWindow *window){
    sfVector2u window_size = sfRenderWindow_getSize(window);
    struct nk_rect visual_size = mygui_rect_window_centered_by_scale(
        cfg_get("windows.settings.visual.size.scale.w", int),
        cfg_get("windows.settings.visual.size.scale.h", int),
        window_size.x,
        window_size.y
    );
    if(nk_begin(context, cfg_get("windows.settings.visual.label", char*), visual_size, MYGUI_WINDOWS_FLAGS | (global_state.first_render_loop ? NK_WINDOW_HIDDEN : 0))){

    }
    nk_end(context);
}

#endif