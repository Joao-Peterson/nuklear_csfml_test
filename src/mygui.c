#include "mygui.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "doc.h"
#include "stb_image.h"
#include "settings.h"
#include "nuklear_csfml.h"

/* --------------------------------------------- Macros --------------------------------------------- */

// macro get nk_color from cfg file. Ex: get_color_cfg_nk("body")
#define get_color_cfg_nk(style_option) nk_rgba_hex(style_get(style_option, char*))

// flags for auxiliary windows
#define MYGUI_WINDOWS_FLAGS (                                           \
NK_WINDOW_TITLE  | NK_WINDOW_MOVABLE  | NK_WINDOW_SCALABLE         |    \
NK_WINDOW_BORDER | NK_WINDOW_CLOSABLE | NK_WINDOW_SCROLL_AUTO_HIDE | NK_WINDOW_MINIMIZABLE )

/* --------------------------------------------- Aux Functions -------------------------------------- */

// get a string width accordingly with the context font
float get_text_width(struct nk_context *context, char *text){
    return context->style.font->width(context->style.font->userdata, context->style.font->height, text, strlen(text));
}


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
void nk_window_show_update_size(int window_w, int window_h, struct nk_context *context, const char *name, int w_percent, int h_percent, enum nk_show_states state){
    nk_window_show(context, name, state);

    struct nk_rect visual_size = mygui_rect_window_centered_by_scale(w_percent, h_percent, window_w, window_h);

    nk_window_set_size(context, name, nk_vec2(visual_size.w, visual_size.h));
    nk_window_set_position(context, name, nk_vec2(visual_size.x, visual_size.y));
}


// get active font
struct nk_user_font *mygui_get_active_font(mygui_interface_t *interface){
    int index = 0;

    doc *fonts = doc_get_ptr(interface->settings->cfg, "font.fonts");
    for(doc_loop(font,fonts)){
        doc *heights = doc_get_ptr(font, "heights");
        for(doc_loop(height, heights)){

            if(!strcmp(font->name, interface->settings->active_font) && (doc_get(height, ".", int) == interface->settings->parameters.text.font.height)){
                struct nk_font *font_cursor = interface->settings->atlas->fonts;

                for(int i = 0; i < index; i++)
                    font_cursor = font_cursor->next;

                return &font_cursor->handle;
            }
                
            index++;
        }
    }    
}


// load fonts
void mygui_load_fonts(mygui_interface_t *interface){
    
    // loads all fonts once
    int i = 0;

    interface->settings->fonts_names = (char**)calloc(interface->settings->fonts_size, sizeof(char*));

    nk_csfml_font_stash_begin(&interface->settings->atlas);

    doc *fonts = doc_get_ptr(interface->settings->cfg, "font.fonts");
    for(doc_loop(font,fonts)){

        char *file_path = doc_get(font, "file", char*);
        interface->settings->fonts_names[i] = font->name;

        doc *heights = doc_get_ptr(font, "heights");
        for(doc_loop(height, heights)){
            if(nk_font_atlas_add_from_file(interface->settings->atlas, file_path, doc_get(height, ".", int), NULL) == NULL){
                printf("font file \"%s\" doesn't exist\n.", file_path);
                exit(1);
            }    
        }

        i++;
    }
    
    nk_csfml_font_stash_end();

    // if(settings->atlas != NULL){
    //     nk_font_atlas_clear(settings->atlas);
    // }

    // nk_csfml_font_stash_begin(&settings->atlas);

    // doc *fonts = doc_get_ptr(settings->cfg, "font.fonts");
    // for(doc_loop(font,fonts)){

    //     char *file_path = doc_get(font, "file", char*);
    //     settings->fonts_names[i] = font->name;

    //     doc *heights = doc_get_ptr(font, "heights");
    //     for(doc_loop(height, heights)){
    //         if(nk_font_atlas_add_from_file(settings->atlas, file_path, doc_get(height, ".", int), NULL) == NULL){
    //             printf("font file \"%s\" doesn't exist\n.", file_path);
    //             exit(1);
    //         }    
    //     }
    // }
    
    // nk_csfml_font_stash_end();
}

/* --------------------------------------------- Styling -------------------------------------------- */

// gui style
void mygui_styles(struct nk_context *context, mygui_interface_t *interface){
    
    // font
    nk_style_set_font(context, mygui_get_active_font(interface));
    
    // texture
    if(interface->texture != NULL)
        free_texture(interface->texture);

    interface->texture = load_texture(interface->settings->theme.texture_file);

    // menu window / groups
    context->style.window.background = interface->settings->theme.window;
    context->style.window.padding = nk_vec2(0,0);                                   // pad between window and groups
    context->style.window.menu_border = 0;
    context->style.window.menu_padding = nk_vec2(0,0);
    context->style.window.spacing = nk_vec2(0,0);
    // context->style.window.spacing.x = interface->settings->parameters.main_window.row.spacing.x;
    // context->style.window.spacing.y = interface->settings->parameters.main_window.row.spacing.y;
    context->style.window.group_padding = nk_vec2(0,0);                             // set zero padding between groups nesting
    context->style.window.group_border = interface->settings->parameters.main_window.border.size;

    // buttons
    context->style.button.rounding = 0;
    context->style.button.border = 0;                                  
    context->style.button.normal = nk_style_item_color(interface->settings->theme.topbar);
    context->style.button.active = nk_style_item_color(interface->settings->theme.selected1);
    context->style.button.hover = nk_style_item_color(interface->settings->theme.hover1);
    context->style.button.text_normal = interface->settings->theme.font;
    context->style.button.text_hover = interface->settings->theme.font;
    context->style.button.text_active = interface->settings->theme.font;

    // menu button
    context->style.menu_button.border = 0;                                  
    context->style.menu_button.normal = nk_style_item_color(interface->settings->theme.topbar);
    context->style.menu_button.active = nk_style_item_color(interface->settings->theme.selected1);
    context->style.menu_button.hover = nk_style_item_color(interface->settings->theme.hover1);
    context->style.menu_button.text_normal = interface->settings->theme.font;
    context->style.menu_button.text_hover = interface->settings->theme.font;
    context->style.menu_button.text_active = interface->settings->theme.font;

    // contextual button, buttons inside menus/ combo boxes
    context->style.window.contextual_padding = nk_vec2(0,0);
    context->style.window.contextual_border = interface->settings->parameters.main_window.border.size;

    context->style.contextual_button.border = 0;
    context->style.contextual_button.normal = nk_style_item_color(interface->settings->theme.window);
    context->style.contextual_button.active = nk_style_item_color(interface->settings->theme.selected1);
    context->style.contextual_button.hover = nk_style_item_color(interface->settings->theme.hover1);
    context->style.contextual_button.text_normal = interface->settings->theme.font;
    context->style.contextual_button.text_hover = interface->settings->theme.font;
    context->style.contextual_button.text_active = interface->settings->theme.font;
    context->style.contextual_button.padding.x = interface->settings->parameters.text.padding.contextual_button.x;
    context->style.contextual_button.padding.y = interface->settings->parameters.text.padding.contextual_button.y;
    
    // combo box
    context->style.window.combo_border = 0;
    context->style.window.combo_border_color = interface->settings->theme.border;
    context->style.window.combo_padding = nk_vec2(0,0);

    context->style.combo.content_padding.x = interface->settings->parameters.text.padding.contextual_button.x;
    context->style.combo.content_padding.y = interface->settings->parameters.text.padding.contextual_button.y;
    context->style.combo.border = interface->settings->parameters.main_window.border.size;
    context->style.combo.border_color = interface->settings->theme.border;
    context->style.combo.label_normal = interface->settings->theme.font;
    context->style.combo.label_active = interface->settings->theme.font;
    context->style.combo.label_hover = interface->settings->theme.font;
    context->style.combo.normal = nk_style_item_color(interface->settings->theme.window);
    context->style.combo.active = nk_style_item_color(interface->settings->theme.selected1);
    context->style.combo.hover = nk_style_item_color(interface->settings->theme.hover1);

    context->style.combo.button.normal = nk_style_item_color(interface->settings->theme.window);
    context->style.combo.button.active = nk_style_item_color(interface->settings->theme.selected1);
    context->style.combo.button.hover = nk_style_item_color(interface->settings->theme.hover1);
    context->style.combo.button.padding = nk_vec2(7,7);
    context->style.combo.button.text_normal = interface->settings->theme.font;
    context->style.combo.button.text_active = interface->settings->theme.font;
    context->style.combo.button.text_hover = interface->settings->theme.font;

    // label
    context->style.text.color = interface->settings->theme.font;

    // windows
    context->style.window.fixed_background = nk_style_item_color(interface->settings->theme.body);
    context->style.window.border = interface->settings->parameters.main_window.border.size;
    context->style.window.border_color = interface->settings->theme.border;
    
    context->style.window.header.normal = nk_style_item_color(interface->settings->theme.topbar);
    context->style.window.header.active = nk_style_item_color(interface->settings->theme.topbar);
    context->style.window.header.hover = nk_style_item_color(interface->settings->theme.topbar);

    context->style.window.header.label_normal = interface->settings->theme.font;
    context->style.window.header.label_active = interface->settings->theme.font;
    context->style.window.header.label_hover = interface->settings->theme.font;

    context->style.window.header.close_button.text_normal = interface->settings->theme.font;
    context->style.window.header.close_button.text_active = interface->settings->theme.font;
    context->style.window.header.close_button.text_hover = interface->settings->theme.font;
    context->style.window.header.close_button.normal = nk_style_item_color(interface->settings->theme.topbar);
    context->style.window.header.close_button.active = nk_style_item_color(interface->settings->theme.topbar);
    context->style.window.header.close_button.hover = nk_style_item_color(interface->settings->theme.hover1);

    context->style.window.header.minimize_button.text_normal = interface->settings->theme.font;
    context->style.window.header.minimize_button.text_active = interface->settings->theme.font;
    context->style.window.header.minimize_button.text_hover = interface->settings->theme.font;
    context->style.window.header.minimize_button.normal = nk_style_item_color(interface->settings->theme.topbar);
    context->style.window.header.minimize_button.active = nk_style_item_color(interface->settings->theme.topbar);
    context->style.window.header.minimize_button.hover = nk_style_item_color(interface->settings->theme.hover1);

    context->style.window.scaler = nk_style_item_color(interface->settings->theme.font);

    context->style.scrollh.active = nk_style_item_color(interface->settings->theme.window);
    context->style.scrollh.normal = nk_style_item_color(interface->settings->theme.window);
    context->style.scrollh.hover = nk_style_item_color(interface->settings->theme.window);
    context->style.scrollh.cursor_active = nk_style_item_color(interface->settings->theme.hover1);
    context->style.scrollh.cursor_normal = nk_style_item_color(interface->settings->theme.border);
    context->style.scrollh.cursor_hover = nk_style_item_color(interface->settings->theme.hover1);

    context->style.scrollv.active = nk_style_item_color(interface->settings->theme.window);
    context->style.scrollv.normal = nk_style_item_color(interface->settings->theme.window);
    context->style.scrollv.hover = nk_style_item_color(interface->settings->theme.window);
    context->style.scrollv.cursor_active = nk_style_item_color(interface->settings->theme.hover1);
    context->style.scrollv.cursor_normal = nk_style_item_color(interface->settings->theme.border);
    context->style.scrollv.cursor_hover = nk_style_item_color(interface->settings->theme.hover1);

    // proprieties
}

/* --------------------------------------------- Windows -------------------------------------------- */

// visual settings->window
void mygui_window_visual(struct nk_context *context, mygui_interface_t *interface){

    if(nk_begin(context, "Appearance", nk_rect(0,0,100,100), MYGUI_WINDOWS_FLAGS | (interface->first_render_loop ? NK_WINDOW_HIDDEN : 0))){

        nk_style_push_vec2(context, &context->style.window.group_padding, nk_vec2(interface->settings->parameters.main_window.group.padding.x, interface->settings->parameters.main_window.group.padding.y));

        nk_layout_row_dynamic(context, nk_window_get_content_region_size(context).y, 1);

        if(nk_group_begin(context, "Appearance_group", 0)){

            nk_style_push_vec2(context, &context->style.window.spacing, nk_vec2(interface->settings->parameters.main_window.row.spacing.x, interface->settings->parameters.main_window.row.spacing.y));

            char *label;
            struct nk_vec2 combo_size;
            combo_size.x = interface->settings->parameters.main_window.menu_dropdown.width;
            combo_size.y = interface->settings->parameters.main_window.menu_dropdown.heigh;

            // theme
            nk_layout_row_begin(context, NK_STATIC, interface->settings->parameters.main_window.row.height, 3);       // first row                  
            label = "Theme: ";
            nk_layout_row_push(context, interface->settings->parameters.windows.settings.visual.label_to_widget_x_padding);
            nk_label(context, label, NK_TEXT_LEFT);
            nk_layout_row_push(context, combo_size.x);
            if(nk_combo_begin_label(context, interface->settings->active_theme, combo_size)){                         // combo box of themes
                nk_layout_row_dynamic(context, interface->settings->parameters.main_window.topbar.height, 1);

                for(doc_size_t i = 0; i < interface->settings->themes_size; i++){                                     // list itens
                    if(nk_combo_item_label(context, interface->settings->themes[i], NK_TEXT_LEFT)){                   // call back on clicked item
                        settings_reload(interface->settings, interface->settings->themes[i], NULL, NULL);
                        mygui_styles(context, interface);                                                               // redo styles
                    }
                }

                nk_combo_end(context);
            }
            nk_layout_row_end(context);


            // scale
            nk_layout_row_begin(context, NK_STATIC, interface->settings->parameters.main_window.row.height, 3);       // first row                  
            label = "Scale: ";
            nk_layout_row_push(context, interface->settings->parameters.windows.settings.visual.label_to_widget_x_padding);
            nk_label(context, label, NK_TEXT_LEFT);
            nk_layout_row_push(context, combo_size.x);
            if(nk_combo_begin_label(context, interface->settings->active_parameters, combo_size)){                     // combo box of themes
                nk_layout_row_dynamic(context, interface->settings->parameters.main_window.topbar.height, 1);

                for(doc_size_t i = 0; i < interface->settings->parameters_size; i++){                                 // list itens
                    if(nk_combo_item_label(context, interface->settings->parameters_array[i], NK_TEXT_LEFT)){         // call back on clicked item
                        settings_reload(interface->settings, NULL, interface->settings->parameters_array[i], NULL);
                        mygui_styles(context, interface);                                                               // redo styles
                    }
                }

                nk_combo_end(context);
            }
            nk_layout_row_end(context);

            
            // font
            nk_layout_row_begin(context, NK_STATIC, interface->settings->parameters.main_window.row.height, 3);       // first row                  
            label = "Font: ";
            nk_layout_row_push(context, interface->settings->parameters.windows.settings.visual.label_to_widget_x_padding);
            nk_label(context, label, NK_TEXT_LEFT);
            nk_layout_row_push(context, combo_size.x);
            if(nk_combo_begin_label(context, interface->settings->active_font, combo_size)){                          // combo box of themes
                nk_layout_row_dynamic(context, interface->settings->parameters.main_window.topbar.height, 1);

                for(doc_size_t i = 0; i < interface->settings->fonts_size; i++){                                      // list itens
                    if(nk_combo_item_label(context, interface->settings->fonts_names[i], NK_TEXT_LEFT)){              // call back on clicked item
                        settings_reload(interface->settings, NULL, NULL, interface->settings->fonts_names[i]);
                        mygui_styles(context, interface);                                                               // redo styles
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

/* --------------------------------------------- Main window ---------------------------------------- */

// topbar
void mygui_topbar(struct nk_context *context, mygui_interface_t *interface){
    char buffer1[200];
    char buffer2[200];
    char buffer3[200];
    
    // topbar

    // temporary background style pushed into stack
    nk_style_push_style_item(context, &context->style.window.fixed_background, nk_style_item_color(interface->settings->theme.topbar));
    if(nk_group_begin(context, "topbar", NK_WINDOW_NO_SCROLLBAR)){

        float menu_width_acc = 0;
        float menu_button_width;

        // menus dropdown
        nk_menubar_begin(context);
        nk_layout_row_begin(context, NK_STATIC, interface->settings->parameters.main_window.topbar.height, 2 + 4); // +4 for the title text and 3 buttons, minimize, maximize and close

        // file
        menu_button_width = get_text_width(context, "File") + interface->settings->parameters.text.widget.padding;
        menu_width_acc += menu_button_width;
        nk_layout_row_push(context, menu_button_width);
        if(nk_menu_begin_label(context, "File", NK_TEXT_CENTERED, nk_vec2(interface->settings->parameters.main_window.menu_dropdown.width, interface->settings->parameters.main_window.menu_dropdown.heigh))){
            nk_layout_row_dynamic(context, interface->settings->parameters.main_window.topbar.height, 1);

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
        menu_button_width = get_text_width(context, "Settings") + interface->settings->parameters.text.widget.padding;
        menu_width_acc += menu_button_width;
        nk_layout_row_push(context, menu_button_width);
        if(nk_menu_begin_label(context, "Settings", NK_TEXT_CENTERED, nk_vec2(interface->settings->parameters.main_window.menu_dropdown.width, interface->settings->parameters.main_window.menu_dropdown.heigh))){

            nk_layout_row_dynamic(context, interface->settings->parameters.main_window.topbar.height, 1);

            if(nk_menu_item_label(context, "Appearance", NK_TEXT_LEFT)){
                nk_window_show_update_size(
                    interface->window_w, 
                    interface->window_h, 
                    context, 
                    "Appearance", 
                    interface->settings->parameters.windows.settings.visual.size.scale.w,
                    interface->settings->parameters.windows.settings.visual.size.scale.h,
                    NK_SHOWN
                );
            }

            nk_menu_end(context);
        }

        // centered label 
        interface->topbar_title_width = nk_window_get_width(context) - menu_width_acc - 3 * interface->settings->parameters.main_window.topbar.buttons.width; 
        interface->topbar_menus_width = menu_width_acc; 
        nk_layout_row_push(context, interface->topbar_title_width);
        nk_label(context, interface->topbar_title, NK_TEXT_CENTERED);

        // buttons
        nk_layout_row_push(context, interface->settings->parameters.main_window.topbar.buttons.width);
        if(nk_button_image(context, interface->texture->button.minimize)){        // minimize
            interface->window_mode_flag = window_mode_minimized;
        }
        if(nk_button_image(context, interface->texture->button.maximize)){        // maximize
            if(interface->window_mode == window_mode_maximized || interface->window_mode == window_mode_fullscreen)
                interface->window_mode_flag = window_mode_float;
            else
                interface->window_mode_flag = window_mode_maximized;
        }
        if(nk_button_image(context, interface->texture->button.close)){           // close
            interface->running_flag = false;
        }

        nk_layout_row_end(context);
        nk_menubar_end(context);

        nk_group_end(context);
    }

    nk_style_pop_style_item(context);
}


// sidebar
void mygui_sidebar(struct nk_context *context, mygui_interface_t *interface){

    nk_style_push_style_item(context, &context->style.window.fixed_background, nk_style_item_color(interface->settings->theme.sidebar));
    if(nk_group_begin(context, "sidebar", NK_WINDOW_NO_SCROLLBAR)){
        

        nk_group_end(context);
    }
    nk_style_pop_style_item(context);
}


// body
void mygui_body(struct nk_context *context, mygui_interface_t *interface){
    nk_style_push_style_item(context, &context->style.window.fixed_background, nk_style_item_color(interface->settings->theme.body));
    if(nk_group_begin(context, "body", NK_WINDOW_NO_SCROLLBAR)){
        
        nk_layout_row_static(
            context, 
            nk_window_get_height(context) - interface->settings->parameters.main_window.topbar.height - interface->settings->parameters.main_window.footer.height, 
            interface->settings->parameters.main_window.body.sidebar.width,
             2
        );

        mygui_sidebar(context, interface);

        nk_group_end(context);
    }
    nk_style_pop_style_item(context);
}


// footer
void mygui_footer(struct nk_context *context, mygui_interface_t *interface){
    nk_style_push_style_item(context, &context->style.window.fixed_background, nk_style_item_color(interface->settings->theme.footer));
    if(nk_group_begin(context, "footer", NK_WINDOW_NO_SCROLLBAR)){


        nk_group_end(context);
    }
    nk_style_pop_style_item(context);
}


/* --------------------------------------------- Functions ------------------------------------------ */

// initialize mygui interface
mygui_interface_t *mygui_init(settings_t *settings){

    mygui_interface_t *interface = (mygui_interface_t*)calloc(1, sizeof(mygui_interface_t));

    interface->running_flag = true;                      
    interface->first_render_loop = true;              
    interface->window_mode_flag = 0;
    
    interface->settings = settings;              

    interface->topbar_title = interface->settings->parameters.main_window.title;                     

    interface->texture = NULL;                                                      // will be laoded by mygui_styles()                     

    interface->window_w = interface->settings->parameters.main_window.size.w;                 
    interface->window_h = interface->settings->parameters.main_window.size.h;                 

    interface->window_mode = window_mode_float;
    if(interface->settings->parameters.main_window.size.fullscreen){                // set flag to transform window based on previous window mode
        interface->window_mode_flag = window_mode_fullscreen;
    }
    else if(interface->settings->parameters.main_window.size.maximized){
        interface->window_mode_flag = window_mode_maximized;
    }

    interface->topbar_menus_width = 0;                 
    interface->topbar_title_width = 0;

    mygui_load_fonts(interface);                 

    return interface;
}

// end the gui interface
void mygui_end(mygui_interface_t *interface){

    if(interface->texture != NULL)
        free_texture(interface->texture);
    
    free(interface);
}

// main gui routine
void mygui(struct nk_context *context, mygui_interface_t *interface){

    // main gui window
    if(nk_begin(context, "main_window", nk_rect(0, 0, interface->window_w, interface->window_h), NK_WINDOW_BACKGROUND | NK_WINDOW_NO_SCROLLBAR)){  

        // main group inside main window
        nk_layout_row_dynamic(context, nk_window_get_height(context), 1);
        if(nk_group_begin(context, "main_group", NK_WINDOW_NO_SCROLLBAR)){

            float main_window_height = nk_window_get_height(context);

            // topbar
            nk_layout_row_dynamic(context, interface->settings->parameters.main_window.topbar.height, 1);
            mygui_topbar(context, interface);

            // body
            nk_layout_row_dynamic(context, interface->window_h - interface->settings->parameters.main_window.topbar.height - interface->settings->parameters.main_window.footer.height, 1);
            mygui_body(context, interface);

            // footer context
            nk_layout_row_dynamic(context, interface->settings->parameters.main_window.footer.height, 1);
            mygui_footer(context, interface);

        }
        nk_group_end(context);
    }
    nk_end(context);
    

    // floating windows
    mygui_window_visual(context, interface);



    if(interface->first_render_loop)
        interface->first_render_loop = false;
}

