#ifndef _GLOBAL_STATE_HEADER_
#define _GLOBAL_STATE_HEADER_

#include <stdbool.h>

typedef struct{
    bool sfml_running;
    char *window_title;
    char *topbar_title;
}state_t;

state_t global_state;

#endif

#ifdef GLOBAL_STATE_IMPLEMENTATION

state_t global_state = {
    .sfml_running = false,
    .window_title = " ",
    .topbar_title = " "
};

#endif