#ifndef _NK_MYGUI_HEADER_
#define _NK_MYGUI_HEADER_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef NK_NUKLEAR_H_
#include <nuklear.h>
#endif 

#include <SFML/Graphics.h>
#include "settings.h"
#include "doc.h"
#include "global_state.h"

/* --------------------------------------------- Macros --------------------------------------------- */

// window/group debug border macro
#define debug_border() ((cfg_get("debug.border", bool)) ? NK_WINDOW_BORDER : false)

// macro get nk_color from cfg file
#define get_color_cfg_nk(style_option) nk_rgba_hex(style_get(style_option, char*));

/* --------------------------------------------- Functions ------------------------------------------ */

// get a string width accordingly with the context font
float nk_mygui_get_text_width(struct nk_context *context, char *text){
    return nk_font_text_width(context->style.font->userdata, context->style.font->height, text, strlen(text));
}


// gui style
void nk_mygui_styles(struct nk_context *context){
    
    // menu window / groups
    context->style.window.background = get_color_cfg_nk("window");
    context->style.window.border_color = nk_rgba_hex(cfg_get("debug.color", char*));
    context->style.window.padding = nk_vec2(0,0);                                   // pad between window and groups
    context->style.window.menu_border = cfg_get("debug.border_width", int) * debug_border();
    context->style.window.menu_padding = nk_vec2(0,0);
    // context->style.window.min_row_height_padding = 0;
    context->style.window.spacing = nk_vec2(0, 0);
    context->style.window.group_padding = nk_vec2(0,0);                             // set zero padding between groups nesting
    context->style.window.group_border_color = nk_rgba_hex(cfg_get("debug.color", char*));
    context->style.window.group_border = cfg_get("debug.border_width", int) * debug_border();

    // buttons
    context->style.button.border_color = nk_rgba_hex(cfg_get("debug.color", char*));
    context->style.button.border = cfg_get("debug.border_width", int) * debug_border();                                  
    context->style.button.normal.type = NK_STYLE_ITEM_COLOR;
    context->style.button.normal.data.color = get_color_cfg_nk("topbar");
    context->style.button.active.type = NK_STYLE_ITEM_COLOR;
    context->style.button.active.data.color = get_color_cfg_nk("selected1");
    context->style.button.hover.type = NK_STYLE_ITEM_COLOR;
    context->style.button.hover.data.color = get_color_cfg_nk("hover1");
    context->style.button.text_normal = get_color_cfg_nk("font");
    context->style.button.text_hover = get_color_cfg_nk("font");
    context->style.button.text_active = get_color_cfg_nk("font");

    // menu button
    context->style.menu_button.border_color = nk_rgba_hex(cfg_get("debug.color", char*));
    context->style.menu_button.border = cfg_get("debug.border_width", int) * debug_border();                                  
    context->style.menu_button.normal.type = NK_STYLE_ITEM_COLOR;
    context->style.menu_button.normal.data.color = get_color_cfg_nk("topbar");
    context->style.menu_button.active.type = NK_STYLE_ITEM_COLOR;
    context->style.menu_button.active.data.color = get_color_cfg_nk("selected1");
    context->style.menu_button.hover.type = NK_STYLE_ITEM_COLOR;
    context->style.menu_button.hover.data.color = get_color_cfg_nk("hover1");
    context->style.menu_button.text_normal = get_color_cfg_nk("font");
    context->style.menu_button.text_hover = get_color_cfg_nk("font");
    context->style.menu_button.text_active = get_color_cfg_nk("font");

    // contextual button, buttons inside menus
    context->style.window.contextual_padding = nk_vec2(0,0);
    context->style.contextual_button.border_color = nk_rgba_hex(cfg_get("debug.color", char*));
    context->style.contextual_button.border = cfg_get("debug.border_width", int) * debug_border();
    context->style.contextual_button.normal.type = NK_STYLE_ITEM_COLOR;
    context->style.contextual_button.normal.data.color = get_color_cfg_nk("window");
    context->style.contextual_button.active.type = NK_STYLE_ITEM_COLOR;
    context->style.contextual_button.active.data.color = get_color_cfg_nk("selected1");
    context->style.contextual_button.hover.type = NK_STYLE_ITEM_COLOR;
    context->style.contextual_button.hover.data.color = get_color_cfg_nk("hover1");
    context->style.contextual_button.text_normal = get_color_cfg_nk("font");
    context->style.contextual_button.text_hover = get_color_cfg_nk("font");
    context->style.contextual_button.text_active = get_color_cfg_nk("font");

    // label
    context->style.text.color = get_color_cfg_nk("font");

    // // selectable
    // context->style.selectable.hover.type = NK_STYLE_ITEM_COLOR;
    // context->style.selectable.hover.data.color = get_color_cfg_nk("hover1");

    // // option
    // context->style.option.hover.type = NK_STYLE_ITEM_COLOR;
    // context->style.option.hover.data.color = get_color_cfg_nk("hover1");
    // context->style.option.active.type = NK_STYLE_ITEM_COLOR;
    // context->style.option.active.data.color = get_color_cfg_nk("hover1");

}


// topbar
void nk_mygui_topbar(struct nk_context *context, sfRenderWindow *window){
    char buffer1[200];
    char buffer2[200];
    char buffer3[200];
    
    // topbar
    context->style.window.fixed_background.type = NK_STYLE_ITEM_COLOR;
    context->style.window.fixed_background.data.color = get_color_cfg_nk("topbar");
    if(nk_group_begin(context, "topbar", NK_WINDOW_NO_SCROLLBAR | debug_border())){

        float menu_width_acc = 0;

        // menus dropdown
        nk_menubar_begin(context);
        nk_layout_row_begin(context, NK_STATIC, cfg_get("window.topbar.height", int), cfg_get("window.topbar.size", int) + 1);

        // for every menu in cfg file
        for(int j = 0; j < doc_get(pcfg, "window.topbar.size", int); ++j){
            snprintf(buffer1, 200, "%s[%i]", "window.topbar", j);
            snprintf(buffer2, 200, "%s.label", buffer1);

            char *topbar_field_label = doc_get(pcfg, buffer2, char *);

            float menu_width = nk_mygui_get_text_width(context, topbar_field_label) + cfg_get("text.widget.padding", int);
            menu_width_acc += menu_width;

            // push the menus width into row
            nk_layout_row_push(context, menu_width);

            // callback over click on menu button
            if(nk_menu_begin_label(context, topbar_field_label, NK_TEXT_CENTERED, nk_vec2(cfg_get("window.menu_dropdown.width", int), cfg_get("window.menu_dropdown.height", int)))){
                nk_layout_row_dynamic(context, doc_get(pcfg, "window.topbar.height", int), 1);
                snprintf(buffer2, 200, "%s.size", buffer1);

                // for every field inside the menu
                for(int i = 0; i < doc_get(pcfg, buffer2, int); ++i){
                    snprintf(buffer3, 200, "%s[%i].label", buffer1, i);
                    nk_menu_item_label(context, doc_get(pcfg, buffer3, char*), NK_TEXT_LEFT);
                }
                nk_menu_end(context);
            }
        }

        // centered label 
        nk_layout_row_push(context, nk_window_get_width(context) - 2*menu_width_acc);
        nk_label(context, global_state.topbar_title, NK_TEXT_CENTERED);

        nk_layout_row_end(context);
        nk_menubar_end(context);

        nk_group_end(context);
    }
}


// sidebar
void nk_mygui_sidebar(struct nk_context *context, sfRenderWindow *window){

    context->style.window.fixed_background.type = NK_STYLE_ITEM_COLOR;
    context->style.window.fixed_background.data.color = get_color_cfg_nk("sidebar");
    if(nk_group_begin(context, "sidebar", NK_WINDOW_NO_SCROLLBAR | debug_border())){
        

        nk_group_end(context);
    }
}


// body
void nk_mygui_body(struct nk_context *context, sfRenderWindow *window){
    
    context->style.window.fixed_background.type = NK_STYLE_ITEM_COLOR;
    context->style.window.fixed_background.data.color = get_color_cfg_nk("body");
    if(nk_group_begin(context, "body", NK_WINDOW_NO_SCROLLBAR | debug_border())){
        
        nk_layout_row_static(context, nk_window_get_height(context) - cfg_get("window.topbar.height", int) - cfg_get("window.footer.height", int), cfg_get("window.body.sidebar.width", int), 2);
        nk_mygui_sidebar(context, window);

        nk_group_end(context);
    }
}


// footer
void nk_mygui_footer(struct nk_context *context, sfRenderWindow *window){
    context->style.window.fixed_background.type = NK_STYLE_ITEM_COLOR;
    context->style.window.fixed_background.data.color = get_color_cfg_nk("footer");
    if(nk_group_begin(context, "footer", NK_WINDOW_NO_SCROLLBAR | debug_border())){


        nk_group_end(context);
    }
}


// main gui routine
void nk_mygui(struct nk_context *context, sfRenderWindow *window){
    sfVector2u window_size = sfRenderWindow_getSize(window);

    // main gui window
    // if(nk_begin(context, "main_window", nk_rect(0, 0, window_size.x, window_size.y), NK_WINDOW_BACKGROUND | NK_WINDOW_NO_INPUT | NK_WINDOW_NO_SCROLLBAR)){  
    if(nk_begin(context, "main_window", nk_rect(0, 0, window_size.x, window_size.y), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND | NK_WINDOW_NO_INPUT)){  


        // styles
        nk_mygui_styles(context);

        // main group inside main window
        nk_layout_row_dynamic(context, nk_window_get_height(context), 1);
        if(nk_group_begin(context, "main_group", NK_WINDOW_BACKGROUND | NK_WINDOW_NO_SCROLLBAR)){

            float main_window_height = nk_window_get_height(context);

            // topbar
            nk_layout_row_dynamic(context, cfg_get("window.topbar.height", int), 1);
            nk_mygui_topbar(context, window);

            // body
            nk_layout_row_dynamic(context, main_window_height - cfg_get("window.topbar.height", int) - cfg_get("window.footer.height", int), 1);
            nk_mygui_body(context, window);

            // footer context
            nk_layout_row_dynamic(context, cfg_get("window.footer.height", int), 1);
            nk_mygui_footer(context, window);


            nk_group_end(context);
            nk_end(context);
        }
    }
}


#endif








// static int property = 20;

// nk_layout_row_static(context, 30, 80, 1);
// if (nk_button_label(context, "button"))
//     fprintf(stdout, "button pressed\n");

// nk_layout_row_dynamic(context, 25, 1);
// nk_property_int(context, "Compression:", 0, &property, 100, 10, 1);

// struct nk_command_buffer *canvas = nk_window_get_canvas(context);



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