#include "global_state.h"
#include "doc.h"
#include "doc_json.h"
#include "stb_image.h"

/* ----------------------------------------- Globals ---------------------------------------- */

// global state
state_t state;

/* ----------------------------------------- Functions -------------------------------------- */

// returns string with 
char *hex_nk_color(struct nk_color color){
    char *buffer = (char*)calloc(10, sizeof(char));
    snprintf(buffer, 10, "#%02X%02X%02X%02X", color.r, color.g, color.b, color.a);
    return buffer;
}


// read from cfg doc struct and load values
void read_settings(void){
    state.settings.active_parameters = doc_get(state.settings.cfg, "parameters.active", int);
    state.settings.parameters_size = doc_get(state.settings.cfg, "parameters.size", int);

    state.settings.active_theme = doc_get(state.settings.cfg, "theme.active", char*);
    state.settings.themes_size = doc_get(state.settings.cfg, "theme.size", int);

    char buffer[500];

    snprintf(buffer, 500, "parameters.array[%i]", state.settings.active_parameters);
    doc *parameters = doc_get_ptr(state.settings.cfg, buffer);

    state.settings.parameters.main_window.title                        = doc_get(parameters, "main_window.title", char*);
    state.settings.parameters.main_window.topbar.height                = doc_get(parameters, "main_window.topbar.height", int);
    state.settings.parameters.main_window.topbar.buttons.width         = doc_get(parameters, "main_window.topbar.buttons.width", int);
    state.settings.parameters.main_window.body.sidebar.width           = doc_get(parameters, "main_window.body.sidebar.width", int);
    state.settings.parameters.main_window.footer.height                = doc_get(parameters, "main_window.footer.height", int);
    state.settings.parameters.main_window.menu_dropdown.heigh          = doc_get(parameters, "main_window.menu_dropdown.height", int);
    state.settings.parameters.main_window.menu_dropdown.width          = doc_get(parameters, "main_window.menu_dropdown.width", int);
    state.settings.parameters.main_window.resize.clearence             = doc_get(parameters, "main_window.resize.clearence", int);
    state.settings.parameters.main_window.resize.min.x                 = doc_get(parameters, "main_window.resize.min.x", int);
    state.settings.parameters.main_window.resize.min.y                 = doc_get(parameters, "main_window.resize.min.y", int);
    state.settings.parameters.main_window.size.x                       = doc_get(parameters, "main_window.size.x", int);
    state.settings.parameters.main_window.size.y                       = doc_get(parameters, "main_window.size.y", int);
    state.settings.parameters.main_window.size.w                       = doc_get(parameters, "main_window.size.w", int);
    state.settings.parameters.main_window.size.h                       = doc_get(parameters, "main_window.size.h", int);
    state.settings.parameters.main_window.size.fullscreen              = doc_get(parameters, "main_window.size.fullscreen", bool);
    state.settings.parameters.main_window.size.maximized               = doc_get(parameters, "main_window.size.maximized", bool);
    state.settings.parameters.main_window.border.size                  = doc_get(parameters, "main_window.border.size", int);
    state.settings.parameters.main_window.row.height                   = doc_get(parameters, "main_window.row.height", int);
    state.settings.parameters.main_window.row.spacing.x                = doc_get(parameters, "main_window.row.spacing.x", int);
    state.settings.parameters.main_window.row.spacing.y                = doc_get(parameters, "main_window.row.spacing.y", int);
    state.settings.parameters.main_window.row.padding.x                = doc_get(parameters, "main_window.row.padding.x", int);
    state.settings.parameters.main_window.row.padding.y                = doc_get(parameters, "main_window.row.padding.y", int);
    state.settings.parameters.main_window.group.padding.x              = doc_get(parameters, "main_window.group.padding.x", int);
    state.settings.parameters.main_window.group.padding.y              = doc_get(parameters, "main_window.group.padding.y", int);
    state.settings.parameters.windows.settings.visual.size.scale.w     = doc_get(parameters, "windows.settings.visual.size.scale.w", int);
    state.settings.parameters.windows.settings.visual.size.scale.h     = doc_get(parameters, "windows.settings.visual.size.scale.h", int);
    state.settings.parameters.text.widget.padding                      = doc_get(parameters, "text.widget.padding", int);
    state.settings.parameters.text.font.file                           = doc_get(parameters, "text.font.file", char*);
    state.settings.parameters.text.font.height                         = doc_get(parameters, "text.font.height", int);
    state.settings.parameters.text.padding.contextual_button.x         = doc_get(parameters, "text.padding.contextual_button.x", int);
    state.settings.parameters.text.padding.contextual_button.y         = doc_get(parameters, "text.padding.contextual_button.y", int);
    
    snprintf(buffer, 500, "theme.array.%s", state.settings.active_theme);
    doc *theme = doc_get_ptr(state.settings.cfg, buffer);

    if(state.settings.themes == NULL)
        state.settings.themes = (char**)calloc( state.settings.themes_size, sizeof(char*));

    char **state_theme_cursor = state.settings.themes;
    for(doc_loop(theme_cursor, theme->parent)){
        *(state_theme_cursor) = theme_cursor->name;
        state_theme_cursor++;
    }

    state.settings.theme.topbar                                        = nk_rgba_hex(doc_get(theme, "topbar", char*));
    state.settings.theme.body                                          = nk_rgba_hex(doc_get(theme, "body", char*));
    state.settings.theme.sidebar                                       = nk_rgba_hex(doc_get(theme, "sidebar", char*));
    state.settings.theme.footer                                        = nk_rgba_hex(doc_get(theme, "footer", char*));
    state.settings.theme.window                                        = nk_rgba_hex(doc_get(theme, "window", char*));
    state.settings.theme.hover1                                        = nk_rgba_hex(doc_get(theme, "hover1", char*));
    state.settings.theme.selected1                                     = nk_rgba_hex(doc_get(theme, "selected1", char*));
    state.settings.theme.font                                          = nk_rgba_hex(doc_get(theme, "font", char*));
    state.settings.theme.border                                        = nk_rgba_hex(doc_get(theme, "border", char*));
    state.settings.theme.texture_file                                  = doc_get(theme, "texture_file", char*);

    state.settings.keys.fullscreen = doc_get(state.settings.cfg, "keys.fullscreen", int);
}


// write state from settings to the cfg doc struct
void write_settings(void){

    doc_set(state.settings.cfg, "parameters.active", int, state.settings.active_parameters);
    doc_set(state.settings.cfg, "theme.active", char*, state.settings.active_theme);

    char buffer[500];

    snprintf(buffer, 500, "parameters.array[%i]", state.settings.active_parameters);
    doc *parameters = doc_get_ptr(state.settings.cfg, buffer);

    doc_set_string(parameters, "main_window.title", char*, state.settings.parameters.main_window.title, strlen(state.settings.parameters.main_window.title) + 1);                                                                                                
    doc_set(parameters, "main_window.topbar.height", int, state.settings.parameters.main_window.topbar.height);                                                                                  
    doc_set(parameters, "main_window.topbar.buttons.width", int, state.settings.parameters.main_window.topbar.buttons.width);                                                                                       
    doc_set(parameters, "main_window.body.sidebar.width", int, state.settings.parameters.main_window.body.sidebar.width);                                                                                         
    doc_set(parameters, "main_window.footer.height", int, state.settings.parameters.main_window.footer.height);                                                                                          
    doc_set(parameters, "main_window.menu_dropdown.height", int, state.settings.parameters.main_window.menu_dropdown.heigh);                                                                                                    
    doc_set(parameters, "main_window.menu_dropdown.width", int, state.settings.parameters.main_window.menu_dropdown.width);                                                                                        
    doc_set(parameters, "main_window.resize.clearence", int, state.settings.parameters.main_window.resize.clearence);                                                                                               
    doc_set(parameters, "main_window.resize.min.x", int, state.settings.parameters.main_window.resize.min.x);                                                                                                           
    doc_set(parameters, "main_window.resize.min.y", int, state.settings.parameters.main_window.resize.min.y);                                                                                                               
    doc_set(parameters, "main_window.size.x", int, state.settings.parameters.main_window.size.x);                                                                                                                     
    doc_set(parameters, "main_window.size.y", int, state.settings.parameters.main_window.size.y);                                                                                                                         
    doc_set(parameters, "main_window.size.w", int, state.settings.parameters.main_window.size.w);                                                                                                                 
    doc_set(parameters, "main_window.size.h", int, state.settings.parameters.main_window.size.h);                                                                                                                 
    doc_set(parameters, "main_window.size.fullscreen", bool, state.settings.parameters.main_window.size.fullscreen);                                                                                                           
    doc_set(parameters, "main_window.size.maximized", bool, state.settings.parameters.main_window.size.maximized);                                                                                                
    doc_set(parameters, "main_window.border.size", int, state.settings.parameters.main_window.border.size);                                                                                                                
    doc_set(parameters, "main_window.row.height", int, state.settings.parameters.main_window.row.height);                                                                                                                     
    doc_set(parameters, "main_window.row.spacing.x", int, state.settings.parameters.main_window.row.spacing.x);                                                                                                              
    doc_set(parameters, "main_window.row.spacing.y", int, state.settings.parameters.main_window.row.spacing.y);                                                                                              
    doc_set(parameters, "main_window.row.padding.x", int, state.settings.parameters.main_window.row.padding.x);                                                                                                                      
    doc_set(parameters, "main_window.row.padding.y", int, state.settings.parameters.main_window.row.padding.y);                                                                                                                      
    doc_set(parameters, "main_window.group.padding.x", int, state.settings.parameters.main_window.group.padding.x);                                                                                                                      
    doc_set(parameters, "main_window.group.padding.y", int, state.settings.parameters.main_window.group.padding.y);                                                                                                                      
    doc_set(parameters, "windows.settings.visual.size.scale.w", int, state.settings.parameters.windows.settings.visual.size.scale.w);                                                                                       
    doc_set(parameters, "windows.settings.visual.size.scale.h", int, state.settings.parameters.windows.settings.visual.size.scale.h);                                                                                           
    doc_set(parameters, "text.widget.padding", int, state.settings.parameters.text.widget.padding);                                                                                                                                
    doc_set_string(parameters, "text.font.file", char*, state.settings.parameters.text.font.file, strlen(state.settings.parameters.text.font.file) + 1);                                                                                                                                       
    doc_set(parameters, "text.font.height", int, state.settings.parameters.text.font.height);                                                                                                               
    doc_set(parameters, "text.padding.contextual_button.x", int, state.settings.parameters.text.padding.contextual_button.x);                                                                                                       
    doc_set(parameters, "text.padding.contextual_button.y", int, state.settings.parameters.text.padding.contextual_button.y);                                                                                                                           
    
    snprintf(buffer, 500, "theme.array.%s", state.settings.active_theme);
    doc *theme = doc_get_ptr(state.settings.cfg, buffer);

    char *hex_topbar       = hex_nk_color(state.settings.theme.topbar);      
    char *hex_body         = hex_nk_color(state.settings.theme.body);        
    char *hex_sidebar      = hex_nk_color(state.settings.theme.sidebar);     
    char *hex_footer       = hex_nk_color(state.settings.theme.footer);      
    char *hex_window       = hex_nk_color(state.settings.theme.window);      
    char *hex_hover1       = hex_nk_color(state.settings.theme.hover1);      
    char *hex_selected1    = hex_nk_color(state.settings.theme.selected1);   
    char *hex_font         = hex_nk_color(state.settings.theme.font);        
    char *hex_border       = hex_nk_color(state.settings.theme.border);      

    doc_set_string(theme, "topbar", char*    , hex_topbar    , strlen(hex_topbar    ) + 1);                                                                          
    doc_set_string(theme, "body", char*      , hex_body      , strlen(hex_body      ) + 1);                                                                                    
    doc_set_string(theme, "sidebar", char*   , hex_sidebar   , strlen(hex_sidebar   ) + 1);                                                                         
    doc_set_string(theme, "footer", char*    , hex_footer    , strlen(hex_footer    ) + 1);                                                      
    doc_set_string(theme, "window", char*    , hex_window    , strlen(hex_window    ) + 1);                                                                      
    doc_set_string(theme, "hover1", char*    , hex_hover1    , strlen(hex_hover1    ) + 1);                                                  
    doc_set_string(theme, "selected1", char* , hex_selected1 , strlen(hex_selected1 ) + 1);                                                       
    doc_set_string(theme, "font", char*      , hex_font      , strlen(hex_font      ) + 1);                                                                    
    doc_set_string(theme, "border", char*    , hex_border    , strlen(hex_border    ) + 1);                                                      

    doc_set_string(theme, "texture_file", char* , state.settings.theme.texture_file, strlen(state.settings.theme.texture_file ) + 1);                                                 

    doc_set(state.settings.cfg, "keys.fullscreen", int, state.settings.keys.fullscreen);
}


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


// load images
texture_t *load_texture(struct nk_context *context, char *filename){
    texture_t *texture = (texture_t*)calloc(1, sizeof(texture_t)); 
    
    glEnable(GL_TEXTURE_2D);    
    texture->image_map_opengl_id = image_load(filename);
    texture->button.minimize    = nk_subimage_id(texture->image_map_opengl_id, 512,512, nk_rect(0*90, 0, 90,50));
    texture->button.maximize    = nk_subimage_id(texture->image_map_opengl_id, 512,512, nk_rect(1*90, 0, 90,50));
    texture->button.close       = nk_subimage_id(texture->image_map_opengl_id, 512,512, nk_rect(2*90, 0, 90,50));

    return texture;
}


// gui style
void mygui_styles(struct nk_context *context){
    
    nk_style_set_font()

    // texture
    state.texture = load_texture(context, state.settings.theme.texture_file);

    // menu window / groups
    context->style.window.background = state.settings.theme.window;
    context->style.window.padding = nk_vec2(0,0);                                   // pad between window and groups
    context->style.window.menu_border = 0;
    context->style.window.menu_padding = nk_vec2(0,0);
    context->style.window.spacing = nk_vec2(0,0);
    // context->style.window.spacing.x = state.settings.parameters.main_window.row.spacing.x;
    // context->style.window.spacing.y = state.settings.parameters.main_window.row.spacing.y;
    context->style.window.group_padding = nk_vec2(0,0);                             // set zero padding between groups nesting
    context->style.window.group_border = state.settings.parameters.main_window.border.size;

    // buttons
    context->style.button.rounding = 0;
    context->style.button.border = 0;                                  
    context->style.button.normal = nk_style_item_color(state.settings.theme.topbar);
    context->style.button.active = nk_style_item_color(state.settings.theme.selected1);
    context->style.button.hover = nk_style_item_color(state.settings.theme.hover1);
    context->style.button.text_normal = state.settings.theme.font;
    context->style.button.text_hover = state.settings.theme.font;
    context->style.button.text_active = state.settings.theme.font;

    // menu button
    context->style.menu_button.border = 0;                                  
    context->style.menu_button.normal = nk_style_item_color(state.settings.theme.topbar);
    context->style.menu_button.active = nk_style_item_color(state.settings.theme.selected1);
    context->style.menu_button.hover = nk_style_item_color(state.settings.theme.hover1);
    context->style.menu_button.text_normal = state.settings.theme.font;
    context->style.menu_button.text_hover = state.settings.theme.font;
    context->style.menu_button.text_active = state.settings.theme.font;

    // contextual button, buttons inside menus/ combo boxes
    context->style.window.contextual_padding = nk_vec2(0,0);
    context->style.window.contextual_border = state.settings.parameters.main_window.border.size;

    context->style.contextual_button.border = 0;
    context->style.contextual_button.normal = nk_style_item_color(state.settings.theme.window);
    context->style.contextual_button.active = nk_style_item_color(state.settings.theme.selected1);
    context->style.contextual_button.hover = nk_style_item_color(state.settings.theme.hover1);
    context->style.contextual_button.text_normal = state.settings.theme.font;
    context->style.contextual_button.text_hover = state.settings.theme.font;
    context->style.contextual_button.text_active = state.settings.theme.font;
    context->style.contextual_button.padding.x = state.settings.parameters.text.padding.contextual_button.x;
    context->style.contextual_button.padding.y = state.settings.parameters.text.padding.contextual_button.y;
    
    // combo box
    context->style.window.combo_border = 0;
    context->style.window.combo_border_color = state.settings.theme.border;
    context->style.window.combo_padding = nk_vec2(0,0);

    context->style.combo.content_padding.x = state.settings.parameters.text.padding.contextual_button.x;
    context->style.combo.content_padding.y = state.settings.parameters.text.padding.contextual_button.y;
    context->style.combo.border = state.settings.parameters.main_window.border.size;
    context->style.combo.border_color = state.settings.theme.border;
    context->style.combo.label_normal = state.settings.theme.font;
    context->style.combo.label_active = state.settings.theme.font;
    context->style.combo.label_hover = state.settings.theme.font;
    context->style.combo.normal = nk_style_item_color(state.settings.theme.window);
    context->style.combo.active = nk_style_item_color(state.settings.theme.selected1);
    context->style.combo.hover = nk_style_item_color(state.settings.theme.hover1);

    context->style.combo.button.normal = nk_style_item_color(state.settings.theme.window);
    context->style.combo.button.active = nk_style_item_color(state.settings.theme.selected1);
    context->style.combo.button.hover = nk_style_item_color(state.settings.theme.hover1);
    context->style.combo.button.padding = nk_vec2(7,7);
    context->style.combo.button.text_normal = state.settings.theme.font;
    context->style.combo.button.text_active = state.settings.theme.font;
    context->style.combo.button.text_hover = state.settings.theme.font;

    // label
    context->style.text.color = state.settings.theme.font;

    // windows
    context->style.window.fixed_background = nk_style_item_color(state.settings.theme.body);
    context->style.window.border = state.settings.parameters.main_window.border.size;
    context->style.window.border_color = state.settings.theme.border;
    
    context->style.window.header.normal = nk_style_item_color(state.settings.theme.topbar);
    context->style.window.header.active = nk_style_item_color(state.settings.theme.topbar);
    context->style.window.header.hover = nk_style_item_color(state.settings.theme.topbar);

    context->style.window.header.label_normal = state.settings.theme.font;
    context->style.window.header.label_active = state.settings.theme.font;
    context->style.window.header.label_hover = state.settings.theme.font;

    context->style.window.header.close_button.text_normal = state.settings.theme.font;
    context->style.window.header.close_button.text_active = state.settings.theme.font;
    context->style.window.header.close_button.text_hover = state.settings.theme.font;
    context->style.window.header.close_button.normal = nk_style_item_color(state.settings.theme.topbar);
    context->style.window.header.close_button.active = nk_style_item_color(state.settings.theme.topbar);
    context->style.window.header.close_button.hover = nk_style_item_color(state.settings.theme.hover1);

    context->style.window.header.minimize_button.text_normal = state.settings.theme.font;
    context->style.window.header.minimize_button.text_active = state.settings.theme.font;
    context->style.window.header.minimize_button.text_hover = state.settings.theme.font;
    context->style.window.header.minimize_button.normal = nk_style_item_color(state.settings.theme.topbar);
    context->style.window.header.minimize_button.active = nk_style_item_color(state.settings.theme.topbar);
    context->style.window.header.minimize_button.hover = nk_style_item_color(state.settings.theme.hover1);

    context->style.window.scaler = nk_style_item_color(state.settings.theme.font);

    context->style.scrollh.active = nk_style_item_color(state.settings.theme.window);
    context->style.scrollh.normal = nk_style_item_color(state.settings.theme.window);
    context->style.scrollh.hover = nk_style_item_color(state.settings.theme.window);
    context->style.scrollh.cursor_active = nk_style_item_color(state.settings.theme.hover1);
    context->style.scrollh.cursor_normal = nk_style_item_color(state.settings.theme.border);
    context->style.scrollh.cursor_hover = nk_style_item_color(state.settings.theme.hover1);

    context->style.scrollv.active = nk_style_item_color(state.settings.theme.window);
    context->style.scrollv.normal = nk_style_item_color(state.settings.theme.window);
    context->style.scrollv.hover = nk_style_item_color(state.settings.theme.window);
    context->style.scrollv.cursor_active = nk_style_item_color(state.settings.theme.hover1);
    context->style.scrollv.cursor_normal = nk_style_item_color(state.settings.theme.border);
    context->style.scrollv.cursor_hover = nk_style_item_color(state.settings.theme.hover1);

    // proprieties
    // context->style.property.
    
}


// initialize global state
int global_state_init(char *cfg_filename){
    doc *cfg = doc_json_open(cfg_filename);
    if(cfg == NULL)
        return 1;

    state.settings.cfg_filename = cfg_filename;

    state.settings.cfg = cfg;
    read_settings();

    state.window_title = state.settings.parameters.main_window.title;
    state.topbar_title = state.settings.parameters.main_window.title;

    return 0;
}


// reload parameters
void global_state_reload(char *theme, int parameter){
    write_settings();
    doc_set(state.settings.cfg, "parameters.active", int, parameter);
    doc_set_string(state.settings.cfg, "theme.active", char*, theme, strlen(theme) + 1);
    read_settings();
}


// end state and save config
void global_state_end(void){
    write_settings();
    doc_json_save(state.settings.cfg, state.settings.cfg_filename);
    // doc_delete(state.settings.cfg, ".");
    free(state.texture);
}
