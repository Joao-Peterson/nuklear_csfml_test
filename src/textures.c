#include "textures.h"
#include "stb_image.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* ------------------------------------------ Functions --------------------------------- */

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
texture_t *load_texture(char *filename){
    texture_t *texture = (texture_t*)calloc(1, sizeof(texture_t)); 
    
    glEnable(GL_TEXTURE_2D);    
    texture->image_map_opengl_id = image_load(filename);
    texture->button.minimize    = nk_subimage_id(texture->image_map_opengl_id, 512,512, nk_rect(0*90, 0, 90,50));
    texture->button.maximize    = nk_subimage_id(texture->image_map_opengl_id, 512,512, nk_rect(1*90, 0, 90,50));
    texture->button.close       = nk_subimage_id(texture->image_map_opengl_id, 512,512, nk_rect(2*90, 0, 90,50));

    return texture;
}

// free texture
void free_texture(texture_t *texture){
    free(texture);
}