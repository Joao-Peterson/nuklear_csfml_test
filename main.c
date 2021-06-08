#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <SFML/Graphics.h>

#define NK_IMPLEMENTATION
#define NK_PRIVATE
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>

#define NK_CSFML_IMPLEMENTATION
#include "inc/nuklear_csfml.h"

#include "inc/settings.h"
#include "inc/nk_mygui.h"
#include "inc/doc.h"
#include "inc/doc_json.h"



#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024



int main(int argc, char **argv){

    /* settings */
    if(cfg_init("./cfg.json")){
        return 0;
    }    

    /* csfml */
    sfVideoMode mode = {.height = WINDOW_HEIGHT, .width = WINDOW_WIDTH, .bitsPerPixel = 24};
    sfRenderWindow *window = sfRenderWindow_create(
        mode,
        "Nuklear and CSFML!",
        sfDefaultStyle,
        NULL
    );

    sfRenderWindow_setVerticalSyncEnabled(window, sfTrue);
    sfRenderWindow_setActive(window, sfTrue);

    /* opengl viewport */
    if(!gladLoadGL()) {                                                             // load opengl extensions
        printf("Failed to load OpenGL extensions!\n");
        return -1;
    }

    sfVector2u window_size = sfRenderWindow_getSize(window);

    /* nuklear */

    // font
    struct nk_font_atlas *atlas;
    nk_csfml_font_stash_begin(&atlas);
    struct nk_font *font = nk_font_atlas_add_from_file(atlas, cfg_get("text.font.file", char*), cfg_get("text.font.height", int), NULL);
    if(font == NULL){
        printf("Missing or invalid font.\n");
        return 0;
    }
    nk_csfml_font_stash_end();

    // context
    struct nk_context *context;
    context = nk_csfml_init(window, &font->handle);
    struct nk_colorf bg;
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

    /* render loop */
    bool running = sfRenderWindow_isOpen(window);
    while(running){ 
        sfEvent event;

        nk_input_begin(context);
        /* handle events loop */
        while(sfRenderWindow_pollEvent(window, &event)){
            switch(event.type){                                                     // sfml events
                case sfEvtClosed:
                    running = false;
                    sfRenderWindow_close(window);
                break;

                case sfEvtResized:
                    glViewport(0, 0, event.size.width, event.size.height);
                break;
            }

            nk_csfml_handle_event(&event);                                          // nuklear events
        }
        nk_input_end(context);
        if(!running) break;

        /* GUI */
        nk_mygui(context, window);

        /* Draw nuklear (opengl) */
        sfRenderWindow_setActive(window, sfTrue);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        /* IMPORTANT: `nk_sfml_render` modifies some global OpenGL state
        * with blending, scissor, face culling and depth test and defaults everything
        * back into a default state. Make sure to either save and restore or
        * reset your own state after drawing rendering the UI. */
        nk_csfml_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        sfRenderWindow_display(window);
    }

    nk_csfml_shutdown();
    return 0;
}