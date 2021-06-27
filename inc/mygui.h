#ifndef _MYGUI_HEADER_
#define _MYGUI_HEADER_

#include <stdbool.h>
#include <stdarg.h>

#include "nuklear_def.h"
#include "textures.h"
#include "settings.h"

/* --------------------------------------------- Enumerators ---------------------------------------- */

// window mode
typedef enum{
    window_mode_minimized = 1,
    window_mode_maximized,
    window_mode_float,
    window_mode_fullscreen
}window_mode_enum;

/* --------------------------------------------- Structs -------------------------------------------- */

typedef struct{
    // flags used by the mygui module to flag events to the nuklear library and the context
    bool running_flag;                      /**< flags the context to shutdown if false*/
    window_mode_enum window_mode_flag;      /**< flags the change of window mode based on the window_mode_enum enumerator*/

    // values to be set, that the module need to work
    window_mode_enum window_mode;           /**< indicates what mode/state the window is based on window_mode_enum enumerator*/
    int window_w;                           /**< the current width of the window context*/
    int window_h;                           /**< the current height of the window context*/
    char *topbar_title;                     /**< the title to be displayed on the topbar of the nuklear window, will be concatenated with other info*/
    texture_t *texture;                     /**< pointer to a texture struct containing nk_image for nuklear widgets */
    settings_t *settings;                   /**< pointer to settings structure */
    
    // value the module exports for external use 
    int topbar_menus_width;                 /**< width of the topbar menus, after wich the title coes on the x direction*/
    int topbar_title_width;                 /**< width of the topbar title, where the move cursor/cmd should be possible*/

    // internal value, do not modify
    bool first_render_loop;                 /**< true if first time of render loop*/
}mygui_interface_t;

/* --------------------------------------------- Functions ------------------------------------------ */

// initialize mygui interface
mygui_interface_t *mygui_init(settings_t *settings);

// end the gui interface
void mygui_end(mygui_interface_t *interface);

// main gui routine
void mygui(struct nk_context *context, mygui_interface_t *interface);

// gui style
void mygui_styles(struct nk_context *context, mygui_interface_t *interface);

// get active font
struct nk_user_font *mygui_get_active_font(mygui_interface_t *interface);

#endif
