#ifndef _TEXTURES_HEADER_
#define _TEXTURES_HEADER_

#include "nuklear_def.h"
#include "glad/glad.h"

/* ------------------------------------------ Structs ----------------------------------- */

typedef struct{
    GLint image_map_opengl_id;
    struct button_t{
        struct nk_image minimize;
        struct nk_image maximize;
        struct nk_image close;
    }button;
}texture_t;

/* ------------------------------------------ Functions --------------------------------- */

// load images
texture_t *load_texture(char *filename);

// free texture
void free_texture(texture_t *texture);

#endif