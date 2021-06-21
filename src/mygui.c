#include "mygui.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "doc.h"
#include "global_state.h"
#include "stb_image.h"
#include "mygui_widgets.h"
#include "mygui_windows.h"

/* --------------------------------------------- Functions ------------------------------------------ */

// get a string width accordingly with the context font
float get_text_width(struct nk_context *context, char *text){
    return context->style.font->width(context->style.font->userdata, context->style.font->height, text, strlen(text));
}


// topbar
void mygui_topbar(struct nk_context *context, sfRenderWindow *window){
    char buffer1[200];
    char buffer2[200];
    char buffer3[200];
    
    // topbar

    // temporary background style pushed into stack
    nk_style_push_style_item(context, &context->style.window.fixed_background, nk_style_item_color(state.settings.theme.topbar));
    if(nk_group_begin(context, "topbar", NK_WINDOW_NO_SCROLLBAR)){

        float menu_width_acc = 0;
        float menu_button_width;

        // menus dropdown
        nk_menubar_begin(context);
        nk_layout_row_begin(context, NK_STATIC, state.settings.parameters.main_window.topbar.height, 2 + 4); // +4 for the title text and 3 buttons, minimize, maximize and close

        // file
        menu_button_width = get_text_width(context, "File") + state.settings.parameters.text.widget.padding;
        menu_width_acc += menu_button_width;
        nk_layout_row_push(context, menu_button_width);
        if(nk_menu_begin_label(context, "File", NK_TEXT_CENTERED, nk_vec2(state.settings.parameters.main_window.menu_dropdown.width, state.settings.parameters.main_window.menu_dropdown.heigh))){
            nk_layout_row_dynamic(context, state.settings.parameters.main_window.topbar.height, 1);

            if(nk_menu_item_label(context, "New file", NK_TEXT_LEFT)){
                
            }

            if(nk_menu_item_label(context, "Open file", NK_TEXT_LEFT)){

            }

            if(nk_menu_item_label(context, "Save file", NK_TEXT_LEFT)){

            }

            if(nk_menu_item_label(context, "Save file as", NK_TEXT_LEFT)){

            }

            nk_menu_end(context);
        }

        // settings
        menu_button_width = get_text_width(context, "Settings") + state.settings.parameters.text.widget.padding;
        menu_width_acc += menu_button_width;
        nk_layout_row_push(context, menu_button_width);
        if(nk_menu_begin_label(context, "Settings", NK_TEXT_CENTERED, nk_vec2(state.settings.parameters.main_window.menu_dropdown.width, state.settings.parameters.main_window.menu_dropdown.heigh))){

            nk_layout_row_dynamic(context, state.settings.parameters.main_window.topbar.height, 1);

            if(nk_menu_item_label(context, "Appearance", NK_TEXT_LEFT)){
                nk_window_show_update_size(
                    window, context, 
                    "Appearance", 
                    state.settings.parameters.windows.settings.visual.size.scale.w,
                    state.settings.parameters.windows.settings.visual.size.scale.h,
                    NK_SHOWN
                );
            }

            nk_menu_end(context);
        }

        // centered label 
        state.topbar_title_width = nk_window_get_width(context) - menu_width_acc - 3 * state.settings.parameters.main_window.topbar.buttons.width; 
        state.topbar_menus_width = menu_width_acc; 
        nk_layout_row_push(context, state.topbar_title_width);
        nk_label(context, state.topbar_title, NK_TEXT_CENTERED);

        // buttons
        nk_layout_row_push(context, state.settings.parameters.main_window.topbar.buttons.width);
        if(nk_button_image(context, state.texture->button.minimize)){        // minimize
            state.sfml_window_mode_flag = window_mode_minimized;
        }
        if(nk_button_image(context, state.texture->button.maximize)){        // maximize
            if(state.sfml_window_mode == window_mode_maximized || state.sfml_window_mode == window_mode_fullscreen)
                state.sfml_window_mode_flag = window_mode_float;
            else
                state.sfml_window_mode_flag = window_mode_maximized;
        }
        if(nk_button_image(context, state.texture->button.close)){           // close
            state.sfml_running = false;
        }

        nk_layout_row_end(context);
        nk_menubar_end(context);

        nk_group_end(context);
    }

    nk_style_pop_style_item(context);
}


// sidebar
void mygui_sidebar(struct nk_context *context, sfRenderWindow *window){

    nk_style_push_style_item(context, &context->style.window.fixed_background, nk_style_item_color(state.settings.theme.sidebar));
    if(nk_group_begin(context, "sidebar", NK_WINDOW_NO_SCROLLBAR)){
        

        nk_group_end(context);
    }
    nk_style_pop_style_item(context);
}


// body
void mygui_body(struct nk_context *context, sfRenderWindow *window){
    nk_style_push_style_item(context, &context->style.window.fixed_background, nk_style_item_color(state.settings.theme.body));
    if(nk_group_begin(context, "body", NK_WINDOW_NO_SCROLLBAR)){
        
        nk_layout_row_static(
            context, 
            nk_window_get_height(context) - state.settings.parameters.main_window.topbar.height - state.settings.parameters.main_window.footer.height, 
            state.settings.parameters.main_window.body.sidebar.width,
             2
        );

        mygui_sidebar(context, window);

        nk_group_end(context);
    }
    nk_style_pop_style_item(context);
}


// footer
void mygui_footer(struct nk_context *context, sfRenderWindow *window){
    nk_style_push_style_item(context, &context->style.window.fixed_background, nk_style_item_color(state.settings.theme.footer));
    if(nk_group_begin(context, "footer", NK_WINDOW_NO_SCROLLBAR)){


        nk_group_end(context);
    }
    nk_style_pop_style_item(context);
}


/* --------------------------------------------- Public functions ----------------------------------- */

// main gui routine
void mygui(struct nk_context *context, sfRenderWindow *window){
    sfVector2u window_size = sfRenderWindow_getSize(window);

    // main gui window
    if(nk_begin(context, "main_window", nk_rect(0, 0, window_size.x, window_size.y), NK_WINDOW_BACKGROUND | NK_WINDOW_NO_SCROLLBAR)){  

        // main group inside main window
        nk_layout_row_dynamic(context, nk_window_get_height(context), 1);
        if(nk_group_begin(context, "main_group", NK_WINDOW_NO_SCROLLBAR)){

            float main_window_height = nk_window_get_height(context);

            // topbar
            nk_layout_row_dynamic(context, state.settings.parameters.main_window.topbar.height, 1);
            mygui_topbar(context, window);

            // body
            nk_layout_row_dynamic(context, main_window_height - state.settings.parameters.main_window.topbar.height - state.settings.parameters.main_window.footer.height, 1);
            mygui_body(context, window);

            // footer context
            nk_layout_row_dynamic(context, state.settings.parameters.main_window.footer.height, 1);
            mygui_footer(context, window);

        }
        nk_group_end(context);
    }
    nk_end(context);
    
    // floating windows
    mygui_windows(context, window);
}


// =======================================================================
//                        Scroll wheel
// =======================================================================

// static int
// ui_piemenu(struct nk_context *ctx, struct nk_vec2 pos, float radius,
//             struct nk_image *icons, int item_count)
// {
//     int ret = -1;
//     struct nk_rect total_space;
//     struct nk_rect bounds;
//     int active_item = 0;

//     /* pie menu popup */
//     struct nk_color border = ctx->style.window.border_color;
//     struct nk_style_item background = ctx->style.window.fixed_background;
//     ctx->style.window.fixed_background = nk_style_item_hide();
//     ctx->style.window.border_color = nk_rgba(0,0,0,0);

//     total_space  = nk_window_get_content_region(ctx);
//     ctx->style.window.spacing = nk_vec2(0,0);
//     ctx->style.window.padding = nk_vec2(0,0);

//     if (nk_popup_begin(ctx, NK_POPUP_STATIC, "piemenu", NK_WINDOW_NO_SCROLLBAR,
//         nk_rect(pos.x - total_space.x - radius, pos.y - radius - total_space.y,
//         2*radius,2*radius)))
//     {
//         int i = 0;
//         struct nk_command_buffer* out = nk_window_get_canvas(ctx);
//         const struct nk_input *in = &ctx->input;

//         total_space = nk_window_get_content_region(ctx);
//         ctx->style.window.spacing = nk_vec2(4,4);
//         ctx->style.window.padding = nk_vec2(8,8);
//         nk_layout_row_dynamic(ctx, total_space.h, 1);
//         nk_widget(&bounds, ctx);

//         /* outer circle */
//         nk_fill_circle(out, bounds, nk_rgb(50,50,50));
//         {
//             /* circle buttons */
//             float step = (2 * 3.141592654f) / (float)(MAX(1,item_count));
//             float a_min = 0; float a_max = step;

//             struct nk_vec2 center = nk_vec2(bounds.x + bounds.w / 2.0f, bounds.y + bounds.h / 2.0f);
//             struct nk_vec2 drag = nk_vec2(in->mouse.pos.x - center.x, in->mouse.pos.y - center.y);
//             float angle = (float)atan2(drag.y, drag.x);
//             if (angle < -0.0f) angle += 2.0f * 3.141592654f;
//             active_item = (int)(angle/step);

//             for (i = 0; i < item_count; ++i) {
//                 struct nk_rect content;
//                 float rx, ry, dx, dy, a;
//                 nk_fill_arc(out, center.x, center.y, (bounds.w/2.0f),
//                     a_min, a_max, (active_item == i) ? nk_rgb(45,100,255): nk_rgb(60,60,60));

//                 /* separator line */
//                 rx = bounds.w/2.0f; ry = 0;
//                 dx = rx * (float)cos(a_min) - ry * (float)sin(a_min);
//                 dy = rx * (float)sin(a_min) + ry * (float)cos(a_min);
//                 nk_stroke_line(out, center.x, center.y,
//                     center.x + dx, center.y + dy, 1.0f, nk_rgb(50,50,50));

//                 /* button content */
//                 a = a_min + (a_max - a_min)/2.0f;
//                 rx = bounds.w/2.5f; ry = 0;
//                 content.w = 30; content.h = 30;
//                 content.x = center.x + ((rx * (float)cos(a) - ry * (float)sin(a)) - content.w/2.0f);
//                 content.y = center.y + (rx * (float)sin(a) + ry * (float)cos(a) - content.h/2.0f);
//                 nk_draw_image(out, content, &icons[i], nk_rgb(255,255,255));
//                 a_min = a_max; a_max += step;
//             }
//         }
//         {
//             /* inner circle */
//             struct nk_rect inner;
//             inner.x = bounds.x + bounds.w/2 - bounds.w/4;
//             inner.y = bounds.y + bounds.h/2 - bounds.h/4;
//             inner.w = bounds.w/2; inner.h = bounds.h/2;
//             nk_fill_circle(out, inner, nk_rgb(45,45,45));

//             /* active icon content */
//             bounds.w = inner.w / 2.0f;
//             bounds.h = inner.h / 2.0f;
//             bounds.x = inner.x + inner.w/2 - bounds.w/2;
//             bounds.y = inner.y + inner.h/2 - bounds.h/2;
//             nk_draw_image(out, bounds, &icons[active_item], nk_rgb(255,255,255));
//         }
//         nk_layout_space_end(ctx);
//         if (!nk_input_is_mouse_down(&ctx->input, NK_BUTTON_RIGHT)) {
//             nk_popup_close(ctx);
//             ret = active_item;
//         }
//     } else ret = -2;
//     ctx->style.window.spacing = nk_vec2(4,4);
//     ctx->style.window.padding = nk_vec2(8,8);
//     nk_popup_end(ctx);

//     ctx->style.window.fixed_background = background;
//     ctx->style.window.border_color = border;
//     return ret;
// }