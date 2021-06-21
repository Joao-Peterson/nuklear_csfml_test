# ---------------------------------------------------------------
# https://www.rapidtables.com/code/linux/gcc/gcc-l.html <- how to link libs

CC := gcc

C_FLAGS :=

I_FLAGS :=
I_FLAGS += -I./inc
I_FLAGS += -IC:/msys64/mingw64/include
I_FLAGS += -IC:/clibs/Nuklear

L_FLAGS :=
L_FLAGS += -L./lib
L_FLAGS += -L./
L_FLAGS += -ldoc
L_FLAGS += -lcsfml-audio-2
L_FLAGS += -lcsfml-graphics-2
L_FLAGS += -lcsfml-network-2
L_FLAGS += -lcsfml-system-2
L_FLAGS += -lcsfml-window-2

SOURCES := main.c src/glad.c src/csfml_window_util.c src/stb_image.c src/global_state.c
SOURCES += src/mygui.c src/nuklear.c 

HEADERS := inc/csfml_window_util.h inc/stb_image.h inc/global_state.h inc/mygui.h 
HEADERS += inc/mygui_windows.h inc/mygui_widgets.h

MAIN_APP := main.exe
BUILD_DIR := build
SRC_DIR := src

# ---------------------------------------------------------------

OBJS := $(SOURCES:.c=.o)
OBJS_BUILD := $(addprefix $(BUILD_DIR)/, $(OBJS))

# ---------------------------------------------------------------

.PHONY : build

build : C_FLAGS += -g
build : build_dir $(MAIN_APP)

release : C_FLAGS += -O3
release : build_dir $(MAIN_APP)

$(MAIN_APP) : $(HEADERS) $(OBJS_BUILD)
	$(CC) $(OBJS_BUILD) $(L_FLAGS) -o $@

$(BUILD_DIR)/%.o : %.c
	$(CC) $(C_FLAGS) $(I_FLAGS) -c $< -o $@

build_dir :
	@mkdir -p $(BUILD_DIR) 
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR) 

clear : 
	rm -f -r $(BUILD_DIR)
	rm -f $(MAIN_APP)