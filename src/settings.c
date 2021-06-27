#include "settings.h"
#include <stdio.h>
#include <stdlib.h>
#include "doc.h"
#include "doc_json.h"

/* ----------------------------------------- Functions -------------------------------------- */

// returns string with 
char *hex_nk_color(struct nk_color color){
    char *buffer = (char*)calloc(10, sizeof(char));
    snprintf(buffer, 10, "#%02X%02X%02X%02X", color.r, color.g, color.b, color.a);
    return buffer;
}


// read from cfg doc struct and load values
void read_settings(settings_t *settings){
    char buffer[500];

    // parameters
    settings->active_parameters = doc_get(settings->cfg, "parameters.active", char*);
    settings->parameters_size = doc_get(settings->cfg, "parameters.size", int);

    snprintf(buffer, 500, "parameters.array.%s", settings->active_parameters);
    doc *parameters = doc_get_ptr(settings->cfg, buffer);

    if(settings->parameters_array != NULL)
        free(settings->parameters_array);

    settings->parameters_array = (char**)calloc(settings->parameters_size, sizeof(char*));

    char **state_parameters_cursor = settings->parameters_array;
    for(doc_loop(parameters_cursor, parameters->parent)){
        *(state_parameters_cursor) = parameters_cursor->name;
        state_parameters_cursor++;
    }

    settings->parameters.main_window.title                                 = doc_get(parameters, "main_window.title", char*);
    settings->parameters.main_window.topbar.height                         = doc_get(parameters, "main_window.topbar.height", int);
    settings->parameters.main_window.topbar.buttons.width                  = doc_get(parameters, "main_window.topbar.buttons.width", int);
    settings->parameters.main_window.body.sidebar.width                    = doc_get(parameters, "main_window.body.sidebar.width", int);
    settings->parameters.main_window.footer.height                         = doc_get(parameters, "main_window.footer.height", int);
    settings->parameters.main_window.menu_dropdown.heigh                   = doc_get(parameters, "main_window.menu_dropdown.height", int);
    settings->parameters.main_window.menu_dropdown.width                   = doc_get(parameters, "main_window.menu_dropdown.width", int);
    settings->parameters.main_window.resize.clearence                      = doc_get(parameters, "main_window.resize.clearence", int);
    settings->parameters.main_window.resize.min.x                          = doc_get(parameters, "main_window.resize.min.x", int);
    settings->parameters.main_window.resize.min.y                          = doc_get(parameters, "main_window.resize.min.y", int);
    settings->parameters.main_window.size.x                                = doc_get(parameters, "main_window.size.x", int);
    settings->parameters.main_window.size.y                                = doc_get(parameters, "main_window.size.y", int);
    settings->parameters.main_window.size.w                                = doc_get(parameters, "main_window.size.w", int);
    settings->parameters.main_window.size.h                                = doc_get(parameters, "main_window.size.h", int);
    settings->parameters.main_window.size.fullscreen                       = doc_get(parameters, "main_window.size.fullscreen", bool);
    settings->parameters.main_window.size.maximized                        = doc_get(parameters, "main_window.size.maximized", bool);
    settings->parameters.main_window.border.size                           = doc_get(parameters, "main_window.border.size", int);
    settings->parameters.main_window.row.height                            = doc_get(parameters, "main_window.row.height", int);
    settings->parameters.main_window.row.spacing.x                         = doc_get(parameters, "main_window.row.spacing.x", int);
    settings->parameters.main_window.row.spacing.y                         = doc_get(parameters, "main_window.row.spacing.y", int);
    settings->parameters.main_window.row.padding.x                         = doc_get(parameters, "main_window.row.padding.x", int);
    settings->parameters.main_window.row.padding.y                         = doc_get(parameters, "main_window.row.padding.y", int);
    settings->parameters.main_window.group.padding.x                       = doc_get(parameters, "main_window.group.padding.x", int);
    settings->parameters.main_window.group.padding.y                       = doc_get(parameters, "main_window.group.padding.y", int);
    settings->parameters.windows.settings.visual.size.scale.w              = doc_get(parameters, "windows.settings.visual.size.scale.w", int);
    settings->parameters.windows.settings.visual.size.scale.h              = doc_get(parameters, "windows.settings.visual.size.scale.h", int);
    settings->parameters.windows.settings.visual.label_to_widget_x_padding = doc_get(parameters, "windows.settings.visual.label_to_widget_x_padding", int);
    settings->parameters.text.widget.padding                               = doc_get(parameters, "text.widget.padding", int);
    settings->parameters.text.padding.contextual_button.x                  = doc_get(parameters, "text.padding.contextual_button.x", int);
    settings->parameters.text.padding.contextual_button.y                  = doc_get(parameters, "text.padding.contextual_button.y", int);
    settings->parameters.text.font.height                                  = doc_get(parameters, "text.font.height", int);
    
    
    //theme
    settings->active_theme = doc_get(settings->cfg, "theme.active", char*);
    settings->themes_size = doc_get(settings->cfg, "theme.size", int);

    snprintf(buffer, 500, "theme.array.%s", settings->active_theme);
    doc *theme = doc_get_ptr(settings->cfg, buffer);

    if(settings->themes != NULL)
        free(settings->themes); 

    settings->themes = (char**)calloc(settings->themes_size, sizeof(char*));

    char **state_theme_cursor = settings->themes;
    for(doc_loop(theme_cursor, theme->parent)){
        *(state_theme_cursor) = theme_cursor->name;
        state_theme_cursor++;
    }

    settings->theme.topbar                                        = nk_rgba_hex(doc_get(theme, "topbar", char*));
    settings->theme.body                                          = nk_rgba_hex(doc_get(theme, "body", char*));
    settings->theme.sidebar                                       = nk_rgba_hex(doc_get(theme, "sidebar", char*));
    settings->theme.footer                                        = nk_rgba_hex(doc_get(theme, "footer", char*));
    settings->theme.window                                        = nk_rgba_hex(doc_get(theme, "window", char*));
    settings->theme.hover1                                        = nk_rgba_hex(doc_get(theme, "hover1", char*));
    settings->theme.selected1                                     = nk_rgba_hex(doc_get(theme, "selected1", char*));
    settings->theme.font                                          = nk_rgba_hex(doc_get(theme, "font", char*));
    settings->theme.border                                        = nk_rgba_hex(doc_get(theme, "border", char*));
    settings->theme.texture_file                                  = doc_get(theme, "texture_file", char*);


    // shortcut keys
    settings->keys.fullscreen = doc_get(settings->cfg, "keys.fullscreen", int);


    // fonts
    settings->active_font = doc_get(settings->cfg, "font.active", char*);
    settings->fonts_size = doc_get(settings->cfg, "font.size", int);
}


// write state from settings to the cfg doc struct
void write_settings(settings_t *settings){
    char buffer[500];

    // parameters set
    doc_set_string(settings->cfg, "parameters.active", char*, settings->active_parameters, strlen(settings->active_parameters) + 1);

    snprintf(buffer, 500, "parameters.array.%s", settings->active_parameters);
    doc *parameters = doc_get_ptr(settings->cfg, buffer);

    doc_set_string(parameters, "main_window.title", char*, settings->parameters.main_window.title, strlen(settings->parameters.main_window.title) + 1);                                                                                                
    doc_set(parameters, "main_window.topbar.height", int, settings->parameters.main_window.topbar.height);                                                                                  
    doc_set(parameters, "main_window.topbar.buttons.width", int, settings->parameters.main_window.topbar.buttons.width);                                                                                       
    doc_set(parameters, "main_window.body.sidebar.width", int, settings->parameters.main_window.body.sidebar.width);                                                                                         
    doc_set(parameters, "main_window.footer.height", int, settings->parameters.main_window.footer.height);                                                                                          
    doc_set(parameters, "main_window.menu_dropdown.height", int, settings->parameters.main_window.menu_dropdown.heigh);                                                                                                    
    doc_set(parameters, "main_window.menu_dropdown.width", int, settings->parameters.main_window.menu_dropdown.width);                                                                                        
    doc_set(parameters, "main_window.resize.clearence", int, settings->parameters.main_window.resize.clearence);                                                                                               
    doc_set(parameters, "main_window.resize.min.x", int, settings->parameters.main_window.resize.min.x);                                                                                                           
    doc_set(parameters, "main_window.resize.min.y", int, settings->parameters.main_window.resize.min.y);                                                                                                               
    doc_set(parameters, "main_window.size.x", int, settings->parameters.main_window.size.x);                                                                                                                     
    doc_set(parameters, "main_window.size.y", int, settings->parameters.main_window.size.y);                                                                                                                         
    doc_set(parameters, "main_window.size.w", int, settings->parameters.main_window.size.w);                                                                                                                 
    doc_set(parameters, "main_window.size.h", int, settings->parameters.main_window.size.h);                                                                                                                 
    doc_set(parameters, "main_window.size.fullscreen", bool, settings->parameters.main_window.size.fullscreen);                                                                                                           
    doc_set(parameters, "main_window.size.maximized", bool, settings->parameters.main_window.size.maximized);                                                                                                
    doc_set(parameters, "main_window.border.size", int, settings->parameters.main_window.border.size);                                                                                                                
    doc_set(parameters, "main_window.row.height", int, settings->parameters.main_window.row.height);                                                                                                                     
    doc_set(parameters, "main_window.row.spacing.x", int, settings->parameters.main_window.row.spacing.x);                                                                                                              
    doc_set(parameters, "main_window.row.spacing.y", int, settings->parameters.main_window.row.spacing.y);                                                                                              
    doc_set(parameters, "main_window.row.padding.x", int, settings->parameters.main_window.row.padding.x);                                                                                                                      
    doc_set(parameters, "main_window.row.padding.y", int, settings->parameters.main_window.row.padding.y);                                                                                                                      
    doc_set(parameters, "main_window.group.padding.x", int, settings->parameters.main_window.group.padding.x);                                                                                                                      
    doc_set(parameters, "main_window.group.padding.y", int, settings->parameters.main_window.group.padding.y);                                                                                                                      
    doc_set(parameters, "windows.settings.visual.size.scale.w", int, settings->parameters.windows.settings.visual.size.scale.w);                                                                                       
    doc_set(parameters, "windows.settings.visual.size.scale.h", int, settings->parameters.windows.settings.visual.size.scale.h);                                                                                           
    doc_set(parameters, "windows.settings.visual.label_to_widget_x_padding", int, settings->parameters.windows.settings.visual.label_to_widget_x_padding);                                                                                           
    doc_set(parameters, "text.widget.padding", int, settings->parameters.text.widget.padding);                                                                                                                                
    doc_set(parameters, "text.padding.contextual_button.x", int, settings->parameters.text.padding.contextual_button.x);                                                                                                       
    doc_set(parameters, "text.padding.contextual_button.y", int, settings->parameters.text.padding.contextual_button.y);                                                                                                                           
    doc_set(parameters, "text.font.height", int, settings->parameters.text.font.height);                                                                                                                           
    

    // theme
    doc_set_string(settings->cfg, "theme.active", char*, settings->active_theme, strlen(settings->active_theme) + 1);

    snprintf(buffer, 500, "theme.array.%s", settings->active_theme);
    doc *theme = doc_get_ptr(settings->cfg, buffer);

    char *hex_topbar       = hex_nk_color(settings->theme.topbar);      
    char *hex_body         = hex_nk_color(settings->theme.body);        
    char *hex_sidebar      = hex_nk_color(settings->theme.sidebar);     
    char *hex_footer       = hex_nk_color(settings->theme.footer);      
    char *hex_window       = hex_nk_color(settings->theme.window);      
    char *hex_hover1       = hex_nk_color(settings->theme.hover1);      
    char *hex_selected1    = hex_nk_color(settings->theme.selected1);   
    char *hex_font         = hex_nk_color(settings->theme.font);        
    char *hex_border       = hex_nk_color(settings->theme.border);      

    doc_set_string(theme, "topbar", char*    , hex_topbar    , strlen(hex_topbar    ) + 1);                                                                          
    doc_set_string(theme, "body", char*      , hex_body      , strlen(hex_body      ) + 1);                                                                                    
    doc_set_string(theme, "sidebar", char*   , hex_sidebar   , strlen(hex_sidebar   ) + 1);                                                                         
    doc_set_string(theme, "footer", char*    , hex_footer    , strlen(hex_footer    ) + 1);                                                      
    doc_set_string(theme, "window", char*    , hex_window    , strlen(hex_window    ) + 1);                                                                      
    doc_set_string(theme, "hover1", char*    , hex_hover1    , strlen(hex_hover1    ) + 1);                                                  
    doc_set_string(theme, "selected1", char* , hex_selected1 , strlen(hex_selected1 ) + 1);                                                       
    doc_set_string(theme, "font", char*      , hex_font      , strlen(hex_font      ) + 1);                                                                    
    doc_set_string(theme, "border", char*    , hex_border    , strlen(hex_border    ) + 1);                                                      

    doc_set_string(theme, "texture_file", char* , settings->theme.texture_file, strlen(settings->theme.texture_file ) + 1);


    // shortcut keys
    doc_set(settings->cfg, "keys.fullscreen", int, settings->keys.fullscreen);


    // fonts
    doc_set_string(settings->cfg, "font.active", char*, settings->active_font, strlen(settings->active_font) + 1);
}


// initialize global state
settings_t *settings_init(char *cfg_filename){
    doc *cfg = doc_json_open(cfg_filename);
    if(cfg == NULL)
        return NULL;

    settings_t *settings = (settings_t*)calloc(1, sizeof(settings_t));

    settings->cfg_filename = cfg_filename;
    settings->cfg = cfg;

    read_settings(settings);

    return settings;
}


// reload parameters
void settings_reload(settings_t *settings, char *theme, char *parameters, char *font){
    write_settings(settings);

    if(parameters != NULL){
        doc_set_string(settings->cfg, "parameters.active", char*, parameters, strlen(parameters) + 1);
    }

    if(theme != NULL){
        doc_set_string(settings->cfg, "theme.active", char*, theme, strlen(theme) + 1);
    }

    if(font != NULL){
        doc_set_string(settings->cfg, "font.active", char*, font, strlen(font) + 1);
    }

    read_settings(settings);
}


// end state and save config
void settings_end(settings_t *settings){
    write_settings(settings);
    doc_json_save(settings->cfg, settings->cfg_filename);
    doc_delete(settings->cfg, ".");
}
