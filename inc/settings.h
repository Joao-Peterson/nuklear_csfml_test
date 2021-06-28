#ifndef _SETTINGS_HEADER_
#define _SETTINGS_HEADER_

#include <SFML/Graphics.h>
#include "nuklear_def.h"
#include "doc.h"

/* --------------------------------------------- Structs -------------------------------------------- */

// key struct
typedef struct{
    sfKeyCode code;                                                                 /**< sfKeyCode enumerator value */
    bool ctrl;                                                                      /**< boolean to indicate if the shortcut needs a combination of ctrl, shift, alt */
    bool shift;
    bool alt;
}key_combo_t;

// settings
typedef struct{
    doc *cfg;                                                                       /**< pointer to doc structure containg all configutation */
    char *cfg_filename;                                                             /**< filename of the cfg json */

    char *active_theme;                                                             /**< active theme name */
    char **themes;
    int themes_size;

    char *active_parameters;                                                        /**< active parameters */
    int parameters_size;
    char **parameters_array;

    struct parameters{                                                              /**< parameters for different elements*/
        struct main_window{
            char *title;                                                            /**< default title for the main window*/
            struct topbar{
                int height;                                                         /**< height of the topbar*/
                struct buttons{
                    int width;                                                      /**< width of the minimize, maximize, close buttons*/
                }buttons;
            }topbar;
            struct body{
                struct sidebar{
                    int width;                                                      /**< width of the sidebar*/
                }sidebar;
            }body;
            struct footer{
                int height;                                                         /**< height of the footer*/
            }footer;
            struct menu_dropdown{
                int width;                                                          /**< width for the drop down menus*/
                int heigh;                                                          /**< height for the drop down menus*/
            }menu_dropdown;
            struct resize{
                int clearence;                                                      /**< offset from the window border to the inside, where the mouse can be used to resize the window*/
                struct min{
                    int x;                                                          /**< min width of the window*/
                    int y;                                                          /**< min height of the window*/
                }min;
            }resize;
            struct size_1{
                int x;                                                              /**< the current x pos of the floating window*/
                int y;                                                              /**< the current y pos of the floating window*/
                int w;                                                              /**< the current width of the floating window*/
                int h;                                                              /**< the current height of the floating window*/
                bool fullscreen;                                                    /**< true if the window is in fullscreen mode*/    
                bool maximized;                                                     /**< true if the window is in maximized mode*/
            }size;
            struct border{
                int size;                                                           /**< border thickness for the window*/
            }border;
            struct row{
                int height;                                                         /**< default row height for rows in windows*/
                struct spacing_1{
                    int x;                                                          /**< default spacing between elements in rows*/
                    int y;                                                          /**< default spacing between rows*/        
                }spacing;
                struct padding_1{
                    int x;                                                          /**< default padding in x between rows and the window*/
                    int y;                                                          /**< default padding in y between rows and the window*/
                }padding;
            }row;
            struct group{                       
                struct padding_2{                                                   /**< padding between window and the group isnide of it*/
                    int x;
                    int y;
                }padding;
            }group;
        }main_window;
        struct windows{                                                             /**< data for the different windows*/
            struct settings{
                struct visual{
                    struct size_2{
                        struct scale{
                            int w;                                                  /**< width scale of the visual settings window */
                            int h;                                                  /**< height scale of the visual settings window*/
                        }scale;
                    }size;
                    int label_to_widget_x_padding;                                  /**< distance between the label and the combo box*/
                }visual;
            }settings;
        }windows;
        struct text{
            struct widget{
                int padding;                                                        /**< padding between text and the widget*/
            }widget;
            struct font{
                int height;                                                         /**< current font, height */
            }font;
            struct padding_3{
                struct contextual_button{
                    int x;                                                          /**< padding of the buttons inside the dropdown menus in x*/
                    int y;                                                          /**< padding of the buttons inside the dropdown menus in y*/
                }contextual_button;
            }padding;
        }text;
    }parameters;

    struct theme{                                                                   /**< colors and textures for different elements*/
        struct nk_color topbar;
        struct nk_color body;
        struct nk_color sidebar;
        struct nk_color footer;
        struct nk_color window;
        struct nk_color hover1;
        struct nk_color selected1;
        struct nk_color font;
        struct nk_color border;
        char *texture_file;
    }theme;

    struct keys{                                                                    /**< short cut keys for actions */
        key_combo_t fullscreen;                                                     
        key_combo_t up_scale_plus;
        key_combo_t up_scale_equal;
        key_combo_t down_scale_minus;
        key_combo_t down_scale_hyphen;
    }keys;

    char *active_font;                                                              /**< name of active font */
    char **fonts_names;                                                             /**< array with fonts names */
    int fonts_size;                                                                 /**< number of loaded fonts */
    struct nk_font_atlas *atlas;                                                    /**< nuklear font atlas pointer */
}settings_t;

/* ----------------------------------------- Functions -------------------------------------- */

// initialize global state
settings_t *settings_init(char *cfg_filename);


// reload parameters
void settings_reload(settings_t *settings, char *theme, char *parameters, char *font);


// end state and save config
void settings_end(settings_t *settings);

// check key based on settings
bool check_key_combo(sfKeyEvent key, key_combo_t type);

#endif