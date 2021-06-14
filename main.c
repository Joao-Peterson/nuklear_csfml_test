#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <SFML/Graphics.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#include <nuklear.h>

#include "inc/nuklear_csfml.h"
#include "inc/nk_mygui.h"
#include "inc/settings.h"
#include "inc/doc.h"
#include "inc/doc_json.h"
#include "inc/global_state.h"
#include "inc/settings.h"
#include "inc/csfml_window_util.h"

#define CFG_FILE "cfg.json"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

int main(int argc, char **argv){

    /* settings */
    if(cfg_init(CFG_FILE)){
        return 0;
    }    
    global_state.window_title = cfg_get("text.title", char*);
    global_state.topbar_title = cfg_get("text.title", char*);
    sfKeyCode fullscreen_key = cfg_get("keys.fullscreen", int);

    /* csfml */
    sfVideoMode mode;
    mode.height = cfg_get("window.size.h", int);
    mode.width  = cfg_get("window.size.w", int);
    mode.bitsPerPixel = 24;
    sfRenderWindow *window = sfRenderWindow_create(
        mode,
        global_state.window_title,
        0,
        NULL
    );
    sfRenderWindow_setVerticalSyncEnabled(window, sfTrue);
    sfRenderWindow_setActive(window, sfTrue);


    /* opengl viewport */
    if(!gladLoadGL()) {                                                             // load opengl extensions
        printf("Failed to load OpenGL extensions!\n");
        return -1;
    }


    /* nuklear */
    struct nk_font_atlas *atlas;
    nk_csfml_font_stash_begin(&atlas);                                              // fonts
    struct nk_font *font = nk_font_atlas_add_from_file(atlas, cfg_get("text.font.file", char*), cfg_get("text.font.height", int), NULL);
    if(font == NULL){
        printf("Missing or invalid font.\n");
        return 0;
    }
    nk_csfml_font_stash_end();

    struct nk_context *context;                                                     // nuklear context
    context = nk_csfml_init(window, &font->handle);
    // textures
    global_state.texture = nk_mygui_load_texture(context, cfg_get("theme.texture_file", char*));
    nk_mygui_styles(context);

    /* video settings */
    sfVector2u window_size;
    sfVector2i window_pos;
    window_size.x = cfg_get("window.size.w", int);
    window_size.y = cfg_get("window.size.h", int);
    window_pos.x = cfg_get("window.size.x", int);
    window_pos.y = cfg_get("window.size.y", int);
    sfRenderWindow_setSize(window, window_size);
    sfRenderWindow_setPosition(window, window_pos);

    global_state.sfml_window_mode = window_mode_float;
    if(cfg_get("window.size.fullscreen", bool)){
        global_state.sfml_window_mode_flag = window_mode_fullscreen;
    }
    else if(cfg_get("window.size.maximized", bool)){
        global_state.sfml_window_mode_flag = window_mode_maximized;
    }

    /* render loop */
    global_state.sfml_running = sfRenderWindow_isOpen(window);
    int clearence = cfg_get("window.resize.clearence", int);
    int minx = cfg_get("window.resize.min.x", int);
    int miny = cfg_get("window.resize.min.y", int);
    int topbar_height = cfg_get("window.topbar.height", int);
    while(global_state.sfml_running){ 
        sfEvent event;

        nk_input_begin(context);
        /* handle events loop */
        while(sfRenderWindow_pollEvent(window, &event)){
            switch(event.type){                                                     // sfml events
                case sfEvtClosed:
                    global_state.sfml_running = false;
                    sfRenderWindow_close(window);
                break;

                case sfEvtResized:                                                  // on window resize
                    glViewport(0, 0, event.size.width, event.size.height);
                break;

                case sfEvtMouseMoved:                                               // check mouse on the sides of window for resize
                    // only apply window transform logic when not in fulscreen/maximized mode
                    if(global_state.sfml_window_mode != window_mode_fullscreen && global_state.sfml_window_mode != window_mode_maximized){
                        if(global_state.mouse_button_held.mouse_left){              // moving while helding the mouse left down
                            csfml_window_transform(
                                window, 
                                event, 
                                global_state.cursor_pos, 
                                &global_state.mouse_button_held.anchor, 
                                minx, 
                                miny
                            );
                        }
                        else{                                                       // checking mouse pos for resize boundaries and cursor sprite 
                            global_state.cursor_pos = csfml_window_scan(
                                window, 
                                event, 
                                clearence,
                                global_state.topbar_menus_width, 
                                0, 
                                global_state.topbar_menus_width + global_state.topbar_title_width, 
                                topbar_height
                            );
                        }
                    }
                break;

                case sfEvtMouseButtonPressed:
                    switch(event.mouseButton.button){
                        case sfMouseLeft:
                            global_state.mouse_button_held.mouse_left = true;
                            global_state.mouse_button_held.anchor.x = event.mouseButton.x;
                            global_state.mouse_button_held.anchor.y = event.mouseButton.y;
                        break;
                    }
                break;

                case sfEvtMouseButtonReleased:
                    switch(event.mouseButton.button){
                        case sfMouseLeft:
                            global_state.mouse_button_held.mouse_left = false;
                        break;
                    }
                break;

                case sfEvtKeyPressed:
                    if(event.key.code == fullscreen_key){
                        if(global_state.sfml_window_mode == window_mode_fullscreen){
                            global_state.sfml_window_mode_flag = window_mode_float;
                        }
                        else{
                            global_state.sfml_window_mode_flag = window_mode_fullscreen;
                        }
                    }
                break;
            }

            nk_csfml_handle_event(&event);                                          // nuklear events
        }
        nk_input_end(context);

        /* check global state flags */
        if(!global_state.sfml_running) break;                                       // close program

        switch(global_state.sfml_window_mode_flag){                                 // change window mode based on flag

            /**
             * make a decision on what whatever mode is flag/requested based on the current state of the window
             */
            case window_mode_float:                                                 // float
                // global_state.topbar_title = "float";

                window_size.x = cfg_get("window.size.w", int);
                window_size.y = cfg_get("window.size.h", int);
                window_pos.x  = cfg_get("window.size.x", int);
                window_pos.y  = cfg_get("window.size.y", int);
            
                switch(global_state.sfml_window_mode){

                    case window_mode_maximized:                                     // float from maximized, just change in size
                        sfRenderWindow_setSize(window, window_size);
                        sfRenderWindow_setPosition(window, window_pos);
                        glViewport(0, 0, window_size.x, window_size.y);
                    break;

                    case window_mode_fullscreen:                                    // float from fullscreen, call to OS api function
                        csfml_window_float(window);
                        sfRenderWindow_setSize(window, window_size);
                        sfRenderWindow_setPosition(window, window_pos);
                    break;
                }

                global_state.sfml_window_mode = window_mode_float;
                global_state.sfml_window_mode_flag = 0;
            break;
                                     
            case window_mode_minimized:                                             // minimize
                global_state.sfml_window_mode = window_mode_minimized;
                global_state.sfml_window_mode_flag = 0;
                csfml_window_minimize(window);
            break;

            case window_mode_maximized:        
                // global_state.topbar_title = "maximized";

                switch(global_state.sfml_window_mode){
                    case window_mode_float:
                        window_size = sfRenderWindow_getSize(window);
                        window_pos = sfRenderWindow_getPosition(window);

                        cfg_save(
                            CFG_FILE, 
                            ((global_state.sfml_window_mode == window_mode_fullscreen) ? true : false), 
                            ((global_state.sfml_window_mode == window_mode_maximized) ? true : false), 
                            window_pos.x, 
                            window_pos.y, 
                            window_size.y, 
                            window_size.x
                        );                                                          // save floating window size/pos before going fullscreen/maximized
                    break;
                }

                global_state.sfml_window_mode = window_mode_maximized;
                global_state.sfml_window_mode_flag = 0;
                csfml_window_maximize(window);
            break;

            case window_mode_fullscreen:                                            // fullscreen
                // global_state.topbar_title = "fullscreen";

                switch(global_state.sfml_window_mode){
                    case window_mode_float:
                        window_size = sfRenderWindow_getSize(window);
                        window_pos = sfRenderWindow_getPosition(window);

                        cfg_save(
                            CFG_FILE, 
                            ((global_state.sfml_window_mode == window_mode_fullscreen) ? true : false), 
                            ((global_state.sfml_window_mode == window_mode_maximized) ? true : false), 
                            window_pos.x, 
                            window_pos.y, 
                            window_size.y, 
                            window_size.x
                        );                                                          // save floating window size/pos before going fullscreen/maximized
                    break;
                }

                global_state.sfml_window_mode = window_mode_fullscreen;
                global_state.sfml_window_mode_flag = 0;
                csfml_window_fullscreen(window);
            break;

            default:                                                                // no flag / invalid flag 
            break;
        }

        /* GUI */
        nk_mygui(context, window);

        /* Draw nuklear (opengl) */
        sfRenderWindow_setActive(window, sfTrue);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 255);
        /* IMPORTANT: `nk_csfml_render` modifies some global OpenGL state
        * with blending, scissor, face culling and depth test and defaults everything
        * back into a default state. Make sure to either save and restore or
        * reset your own state after drawing rendering the UI. */
        nk_csfml_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        sfRenderWindow_display(window);
    }

    /* save data */
    if(global_state.sfml_window_mode != window_mode_fullscreen && global_state.sfml_window_mode != window_mode_maximized){
        window_size = sfRenderWindow_getSize(window);
        window_pos  = sfRenderWindow_getPosition(window);
    }

    cfg_end(
        CFG_FILE, 
        ((global_state.sfml_window_mode == window_mode_fullscreen) ? true : false), 
        ((global_state.sfml_window_mode == window_mode_maximized) ? true : false), 
        window_pos.x, 
        window_pos.y, 
        window_size.y, 
        window_size.x
    );    

    /* exit */
    free(global_state.texture);
    nk_csfml_shutdown();
    return 0;
}