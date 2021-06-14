#include "nk_mygui.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "settings.h"
#include "doc.h"
#include "global_state.h"
#include "stb_image.h"


// die function, idk, copied from skinning.c example on Nuklear repo
static void die(const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputs("\n", stderr);
    exit(EXIT_FAILURE);
}


// load image, copied from skinning.c example on Nuklear repo
static GLuint image_load(const char *filename){
    int x,y,n;
    GLuint tex;
    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
    if (!data) die("failed to load image: %s", filename);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return tex;
}


// get a string width accordingly with the context font
float nk_mygui_get_text_width(struct nk_context *context, char *text){
    return context->style.font->width(context->style.font->userdata, context->style.font->height, text, strlen(text));
}


// load images
texture_t *nk_mygui_load_texture(struct nk_context *context, char *filename){
    texture_t *texture = (texture_t*)calloc(1, sizeof(texture_t)); 
    
    glEnable(GL_TEXTURE_2D);    
    texture->image_map_opengl_id = image_load(filename);
    texture->button.minimize    = nk_subimage_id(texture->image_map_opengl_id, 512,512, nk_rect(0*90, 0, 90,50));
    texture->button.maximize    = nk_subimage_id(texture->image_map_opengl_id, 512,512, nk_rect(1*90, 0, 90,50));
    texture->button.close       = nk_subimage_id(texture->image_map_opengl_id, 512,512, nk_rect(2*90, 0, 90,50));

    return texture;
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
    context->style.button.rounding = 0;
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

        int topbar_height   = cfg_get("window.topbar.height", int);
        int topbar_size     = cfg_get("window.topbar.size", int);
        int padding         = cfg_get("text.widget.padding", int);
        int button_width    = cfg_get("window.topbar.buttons.width", int);

        // menus dropdown
        nk_menubar_begin(context);
        nk_layout_row_begin(context, NK_STATIC, topbar_height, topbar_size + 4); // +4 for the title text and 3 buttons, minimize, maximize and close

        // for every menu in cfg file
        for(int j = 0; j < topbar_size; ++j){
            // get topbar items 
            snprintf(buffer1, 200, "%s[%i]", "window.topbar", j);
            snprintf(buffer2, 200, "%s.label", buffer1);
            char *topbar_field_label = doc_get(pcfg, buffer2, char *);

            float menu_width = nk_mygui_get_text_width(context, topbar_field_label) + padding;
            menu_width_acc += menu_width;

            // push the menus width into row
            nk_layout_row_push(context, menu_width);

            // callback over click on menu button
            if(nk_menu_begin_label(context, topbar_field_label, NK_TEXT_CENTERED, nk_vec2(cfg_get("window.menu_dropdown.width", int), cfg_get("window.menu_dropdown.height", int)))){
                nk_layout_row_dynamic(context, topbar_height, 1);
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
        global_state.topbar_title_width = nk_window_get_width(context) - menu_width_acc - 3 * button_width; 
        global_state.topbar_menus_width = menu_width_acc; 
        nk_layout_row_push(context, global_state.topbar_title_width);
        nk_label(context, global_state.topbar_title, NK_TEXT_CENTERED);

        // buttons
        nk_layout_row_push(context, button_width);
        if(nk_button_image(context, global_state.texture->button.minimize)){        // minimize
            global_state.sfml_minimize_flag = true;
        }
        if(nk_button_image(context, global_state.texture->button.maximize)){        // maximize
            global_state.sfml_toggle_full_float = true;
        }
        if(nk_button_image(context, global_state.texture->button.close)){           // close
            global_state.sfml_running = false;
        }

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
