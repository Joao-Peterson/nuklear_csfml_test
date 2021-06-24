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

#include "global_state.h"
#include "mygui.h"

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

    if(nk_begin(context, "Appearance", nk_rect(0,0,100,100), MYGUI_WINDOWS_FLAGS | (state.first_render_loop ? NK_WINDOW_HIDDEN : 0))){

        nk_style_push_vec2(context, &context->style.window.group_padding, nk_vec2(state.settings.parameters.main_window.group.padding.x, state.settings.parameters.main_window.group.padding.y));

        nk_layout_row_dynamic(context, nk_window_get_content_region_size(context).y, 1);

        if(nk_group_begin(context, "Appearance_group", 0)){

            nk_style_push_vec2(context, &context->style.window.spacing, nk_vec2(state.settings.parameters.main_window.row.spacing.x, state.settings.parameters.main_window.row.spacing.y));

            char *label;
            struct nk_vec2 combo_size;
            combo_size.x = state.settings.parameters.main_window.menu_dropdown.width;
            combo_size.y = state.settings.parameters.main_window.menu_dropdown.heigh;

            // theme
            nk_layout_row_begin(context, NK_STATIC, state.settings.parameters.main_window.row.height, 3);       // first row                  
            label = "Theme: ";
            nk_layout_row_push(context, state.settings.parameters.windows.settings.visual.label_to_widget_x_padding);
            nk_label(context, label, NK_TEXT_LEFT);
            nk_layout_row_push(context, combo_size.x);
            if(nk_combo_begin_label(context, state.settings.active_theme, combo_size)){                         // combo box of themes
                nk_layout_row_dynamic(context, state.settings.parameters.main_window.topbar.height, 1);

                for(doc_size_t i = 0; i < state.settings.themes_size; i++){                                     // list itens
                    if(nk_combo_item_label(context, state.settings.themes[i], NK_TEXT_LEFT)){                   // call back on clicked item
                        global_state_reload(state.settings.themes[i], NULL, NULL);
                        mygui_styles(context);                                                                  // redo styles
                    }
                }

                nk_combo_end(context);
            }
            nk_layout_row_end(context);


            // scale
            nk_layout_row_begin(context, NK_STATIC, state.settings.parameters.main_window.row.height, 3);       // first row                  
            label = "Scale: ";
            nk_layout_row_push(context, state.settings.parameters.windows.settings.visual.label_to_widget_x_padding);
            nk_label(context, label, NK_TEXT_LEFT);
            nk_layout_row_push(context, combo_size.x);
            if(nk_combo_begin_label(context, state.settings.active_parameters, combo_size)){                     // combo box of themes
                nk_layout_row_dynamic(context, state.settings.parameters.main_window.topbar.height, 1);

                for(doc_size_t i = 0; i < state.settings.parameters_size; i++){                                 // list itens
                    if(nk_combo_item_label(context, state.settings.parameters_array[i], NK_TEXT_LEFT)){         // call back on clicked item
                        global_state_reload(NULL, state.settings.parameters_array[i], NULL);
                        mygui_styles(context);                                                                  // redo styles
                    }
                }

                nk_combo_end(context);
            }
            nk_layout_row_end(context);

            
            // font
            nk_layout_row_begin(context, NK_STATIC, state.settings.parameters.main_window.row.height, 3);       // first row                  
            label = "Font: ";
            nk_layout_row_push(context, state.settings.parameters.windows.settings.visual.label_to_widget_x_padding);
            nk_label(context, label, NK_TEXT_LEFT);
            nk_layout_row_push(context, combo_size.x);
            if(nk_combo_begin_label(context, state.settings.active_font, combo_size)){                          // combo box of themes
                nk_layout_row_dynamic(context, state.settings.parameters.main_window.topbar.height, 1);

                for(doc_size_t i = 0; i < state.settings.fonts_size; i++){                                      // list itens
                    if(nk_combo_item_label(context, state.settings.fonts_names[i], NK_TEXT_LEFT)){              // call back on clicked item
                        global_state_reload(NULL, NULL, state.settings.fonts_names[i]);
                        mygui_styles(context);                                                                  // redo styles
                    }
                }

                nk_combo_end(context);
            }
            nk_layout_row_end(context);

            nk_style_pop_vec2(context);
            nk_group_end(context);
        }

        nk_style_pop_vec2(context);
    }

    nk_end(context);
}

// call all windows as hidden
void mygui_windows(struct nk_context *context, sfRenderWindow *window){
    mygui_visual(context, window);
}

#endif