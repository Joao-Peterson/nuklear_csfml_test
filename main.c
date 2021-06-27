#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <SFML/Graphics.h>

#include "nuklear_def.h"
#include "nuklear_csfml.h"
#include "mygui.h"
#include "doc.h"
#include "doc_json.h"
#include "settings.h"
#include "csfml_window_util.h"

#define CFG_FILE "cfg.json"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

int main(int argc, char **argv){

    /* settings */
    settings_t *settings = settings_init(CFG_FILE);
    if(settings == NULL){
        return 0;
    }


    /* csfml */
    sfVideoMode mode;
    mode.width  = settings->parameters.main_window.size.w;
    mode.height = settings->parameters.main_window.size.h;
    mode.bitsPerPixel = 24;
    sfRenderWindow *window = sfRenderWindow_create(
        mode,
        settings->parameters.main_window.title,
        0,
        NULL
    );
    sfRenderWindow_setVerticalSyncEnabled(window, sfTrue);
    sfRenderWindow_setActive(window, sfTrue);
    mouse_info_t mouse = {.cursor_pos = 0, .mouse_button_held = {}};


    /* opengl viewport */
    if(!gladLoadGL()) {                                                             // load opengl extensions
        printf("Failed to load OpenGL extensions!\n");
        return -1;
    }


    /* mygui init */
    mygui_interface_t *interface = mygui_init(settings);
    if(settings == NULL){
        return 0;
    }


    /* nuklear */
    struct nk_context *context;                                                     // nuklear context
    context = nk_csfml_init(window, mygui_get_active_font(interface));
    
    /* video settings */
    mygui_styles(context, interface);                                               // textures and styles
    sfVector2u window_size;
    sfVector2i window_pos;
    window_size.x = settings->parameters.main_window.size.w;
    window_size.y = settings->parameters.main_window.size.h;
    window_pos.x  = settings->parameters.main_window.size.x;
    window_pos.y  = settings->parameters.main_window.size.y;
    sfRenderWindow_setSize(window, window_size);
    sfRenderWindow_setPosition(window, window_pos);


    /* render loop */
    int clearence       = settings->parameters.main_window.resize.clearence;
    int minx            = settings->parameters.main_window.resize.min.x;
    int miny            = settings->parameters.main_window.resize.min.y;
    int topbar_height   = settings->parameters.main_window.topbar.height;
    interface->running_flag = true;
    while(interface->running_flag){ 

        sfEvent event;
        nk_input_begin(context);

        /* handle events loop */
        while(sfRenderWindow_pollEvent(window, &event)){
            switch(event.type){                                                     // sfml events
                case sfEvtClosed:
                    interface->running_flag = false;
                    sfRenderWindow_close(window);
                break;

                case sfEvtResized:                                                  // on window resize
                    glViewport(0, 0, event.size.width, event.size.height);
                break;

                case sfEvtMouseMoved:                                               // check mouse on the sides of window for resize
                    // only apply window transform logic when not in fulscreen/maximized mode
                    if(interface->window_mode != window_mode_fullscreen && interface->window_mode != window_mode_maximized){
                        if(mouse.mouse_button_held.mouse_left){                     // moving while helding the mouse left down
                            csfml_window_transform(
                                window, 
                                event, 
                                mouse.cursor_pos, 
                                &mouse.mouse_button_held.anchor, 
                                minx, 
                                miny
                            );
                        }
                        else{                                                       // checking mouse pos for resize boundaries and cursor sprite 
                            mouse.cursor_pos = csfml_window_scan(
                                window, 
                                event, 
                                clearence,
                                interface->topbar_menus_width, 
                                0, 
                                interface->topbar_menus_width + interface->topbar_title_width, 
                                topbar_height
                            );
                        }
                    }
                break;

                case sfEvtMouseButtonPressed:
                    switch(event.mouseButton.button){                               
                        case sfMouseLeft:
                            mouse.mouse_button_held.mouse_left = true;
                            mouse.mouse_button_held.anchor.x = event.mouseButton.x;
                            mouse.mouse_button_held.anchor.y = event.mouseButton.y;
                        break;
                    }
                break;

                case sfEvtMouseButtonReleased:
                    switch(event.mouseButton.button){
                        case sfMouseLeft:
                            mouse.mouse_button_held.mouse_left = false;
                        break;
                    }
                break;

                case sfEvtLostFocus:                                                // reset held mouse buttons if they fly off the window
                    mouse.mouse_button_held.mouse_left = false;
                    mouse.mouse_button_held.mouse_right = false;
                    mouse.mouse_button_held.mouse_middle = false;
                    mouse.mouse_button_held.mouse_xButton1 = false;
                    mouse.mouse_button_held.mouse_xButton2 = false;
                break;

                case sfEvtKeyPressed:
                    if(event.key.code == settings->keys.fullscreen){                // fullscreen key
                        if(interface->window_mode == window_mode_fullscreen){
                            interface->window_mode_flag = window_mode_float;
                        }
                        else{
                            interface->window_mode_flag = window_mode_fullscreen;
                        }
                    }
                break;
            }

            nk_csfml_handle_event(&event);                                          // nuklear events
        }
        nk_input_end(context);

        /* check global state flags */
        if(!interface->running_flag) break;                                         // close program
        switch(interface->window_mode_flag){                                        // change window mode based on flag

            /**
             * make a decision on what whatever mode is flag/requested based on the current state of the window
             */
            case window_mode_float:                                                 // float
                window_size.x = settings->parameters.main_window.size.w;
                window_size.y = settings->parameters.main_window.size.h;
                window_pos.x  = settings->parameters.main_window.size.x;
                window_pos.y  = settings->parameters.main_window.size.y;
            
                switch(interface->window_mode){

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

                interface->window_mode = window_mode_float;
                interface->window_mode_flag = 0;
            break;
                                     
            case window_mode_minimized:                                             // minimize
                interface->window_mode = window_mode_minimized;
                interface->window_mode_flag = 0;
                csfml_window_minimize(window);
            break;

            case window_mode_maximized:                                             // maximize
                switch(interface->window_mode){
                    case window_mode_float:
                        window_size = sfRenderWindow_getSize(window);
                        window_pos = sfRenderWindow_getPosition(window);

                        settings->parameters.main_window.size.fullscreen = (bool)(interface->window_mode == window_mode_fullscreen);
                        settings->parameters.main_window.size.maximized = (bool)(interface->window_mode == window_mode_maximized);
                        settings->parameters.main_window.size.x = window_pos.x;
                        settings->parameters.main_window.size.y = window_pos.y;
                        settings->parameters.main_window.size.w = window_size.x;
                        settings->parameters.main_window.size.h = window_size.y;    // save floating window size/pos before going fullscreen/maximized
                    break;
                }

                interface->window_mode = window_mode_maximized;
                interface->window_mode_flag = 0;
                csfml_window_maximize(window);
            break;

            case window_mode_fullscreen:                                            // fullscreen
                switch(interface->window_mode){
                    case window_mode_float:
                        window_size = sfRenderWindow_getSize(window);
                        window_pos = sfRenderWindow_getPosition(window);

                        settings->parameters.main_window.size.fullscreen = (bool)(interface->window_mode == window_mode_fullscreen);
                        settings->parameters.main_window.size.maximized = (bool)(interface->window_mode == window_mode_maximized);
                        settings->parameters.main_window.size.x = window_pos.x;
                        settings->parameters.main_window.size.y = window_pos.y;
                        settings->parameters.main_window.size.w = window_size.x;
                        settings->parameters.main_window.size.h = window_size.y;    // save floating window size/pos before going fullscreen/maximized
                    break;
                }

                interface->window_mode = window_mode_fullscreen;
                interface->window_mode_flag = 0;
                csfml_window_fullscreen(window);
            break;

            default:                                                                // no flag / invalid flag 
            break;
        }

        /* GUI */
        sfVector2u real_window_size = sfRenderWindow_getSize(window);
        interface->window_w = real_window_size.x;
        interface->window_h = real_window_size.y;
        mygui(context, interface);

        /* Draw nuklear (opengl) */
        sfRenderWindow_setActive(window, sfTrue);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 255);

        /** 
         * IMPORTANT: `nk_csfml_render` modifies some global OpenGL state
         * with blending, scissor, face culling and depth test and defaults everything
         * back into a default state. Make sure to either save and restore or
         * reset your own state after drawing rendering the UI. 
         */
        nk_csfml_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        sfRenderWindow_display(window);
    }

    /* save data */
    if(interface->window_mode != window_mode_fullscreen && interface->window_mode != window_mode_maximized){
        window_size = sfRenderWindow_getSize(window);
        window_pos  = sfRenderWindow_getPosition(window);
    }
    settings->parameters.main_window.size.fullscreen = (bool)(interface->window_mode == window_mode_fullscreen);
    settings->parameters.main_window.size.maximized = (bool)(interface->window_mode == window_mode_maximized);
    settings->parameters.main_window.size.x = window_pos.x;
    settings->parameters.main_window.size.y = window_pos.y;
    settings->parameters.main_window.size.w = window_size.x;
    settings->parameters.main_window.size.h = window_size.y;                   // save floating window size/pos before going fullscreen/maximized
    settings_end(settings);
    mygui_end(interface);

    /* exit */
    nk_csfml_shutdown();
    return 0;
}