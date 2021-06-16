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
#include "nk_mygui.h"

/* --------------------------------------------- Macros --------------------------------------------- */

#define MYGUI_WINDOWS_FLAGS (                                           \
NK_WINDOW_TITLE  | NK_WINDOW_MOVABLE  | NK_WINDOW_SCALABLE         |    \
NK_WINDOW_BORDER | NK_WINDOW_CLOSABLE | NK_WINDOW_SCROLL_AUTO_HIDE | NK_WINDOW_MINIMIZABLE )

/* --------------------------------------------- Functions ------------------------------------------ */

// return rect centered in a top window by proportions in percent
struct nk_rect mygui_rect_window_centered_by_scale(int w_percent, int h_percent, int top_w, int top_h){
    struct nk_rect rect;
    rect.w = top_w * ( w_percent / 100.0);
    rect.h = top_h * ( h_percent / 100.0);
    rect.x = (top_w - rect.w) / 2.0;
    rect.y = (top_h - rect.h) / 2.0;
    return rect;
}

// update window state and resize/move it
void nk_window_show_update_size(sfRenderWindow *window, struct nk_context *context, const char *name, int w_percent, int h_percent, enum nk_show_states state){
    nk_window_show(context, name, state);

    sfVector2u window_size = sfRenderWindow_getSize(window);
    struct nk_rect visual_size = mygui_rect_window_centered_by_scale(w_percent, h_percent, window_size.x, window_size.y);

    nk_window_set_size(context, name, nk_vec2(visual_size.w, visual_size.h));
    nk_window_set_position(context, name, nk_vec2(visual_size.x, visual_size.y));
}

// visual settings window
void mygui_visual(struct nk_context *context, sfRenderWindow *window){

    if(nk_begin(context, cfg_get("windows.settings.visual.label", char*), nk_rect(0,0,100,100), MYGUI_WINDOWS_FLAGS | (global_state.first_render_loop ? NK_WINDOW_HIDDEN : 0))){

        /**
         * TODO: make a group inside the window for padding the options more to the center without messing with the scrollbar and scaler 
         * TODO: remove the little space between the combobox buttons and the combobox window to the right side 
         * TODO: add scaling option 
         */

        nk_layout_row_begin(context, NK_STATIC, cfg_get("window.row.height", int), 3);                  // first row                  

        // theme
        char *label = "Theme: ";
        nk_layout_row_push(context, nk_get_text_width(context, label));
        nk_label(context, label, NK_TEXT_LEFT);

        struct nk_vec2 combo_size;
        combo_size.x = cfg_get("window.menu_dropdown.width", int);
        combo_size.y = cfg_get("window.menu_dropdown.height", int);
        nk_layout_row_push(context, combo_size.x);

        doc *theme_array = doc_get_ptr(pcfg, "theme.array");
        if(nk_combo_begin_label(context, doc_get(pcfg, "theme.active", char*), combo_size)){            // combo box of themes
            nk_layout_row_dynamic(context, cfg_get("window.topbar.height", int), 1);

            for(doc_loop(theme, theme_array)){                                                          // list itens
                if(nk_combo_item_label(context, theme->name, NK_TEXT_LEFT)){                            // call back on clicked item
                    doc_set(pcfg, "theme.active", char*, theme->name, strlen(theme->name) + 1);         // set active theme onto doc pcfg
                    settings_style = theme;                                                             // change pointer to theme
                    mygui_styles(context);                                                              // re do styles
                }
            }

            nk_combo_end(context);
        }

        nk_layout_row_end(context);
    }

    nk_end(context);
}

// call all windows as hidden
void mygui_windows(struct nk_context *context, sfRenderWindow *window){
    mygui_visual(context, window);
}

#endif