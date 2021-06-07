# ---------------------------------------------------------------
# https://www.rapidtables.com/code/linux/gcc/gcc-l.html <- how to link libs

CC := gcc

C_FLAGS :=

I_FLAGS :=
I_FLAGS += -I./inc
I_FLAGS += -I./glad/include
I_FLAGS += -IC:/msys64/mingw64/include
I_FLAGS += -IC:/clibs/Nuklear

L_FLAGS :=
L_FLAGS += -L./lib
L_FLAGS += -L./
L_FLAGS += -lcsfml-audio-2
L_FLAGS += -lcsfml-graphics-2
L_FLAGS += -lcsfml-network-2
L_FLAGS += -lcsfml-system-2
L_FLAGS += -lcsfml-window-2

SOURCES := main.c glad/src/glad.c src/doc.c src/doc_json.c src/parse_utils.c src/base64.c
MAIN_APP := main.exe
BUILD_DIR := build/

# ---------------------------------------------------------------

OBJS := $(SOURCES:.c=.o)

OBJS_BUILD := $(addprefix $(BUILD_DIR), $(notdir $(SOURCES:.c=.o)))

# ---------------------------------------------------------------

.PHONY : build

build : C_FLAGS += -g
build : $(MAIN_APP)

release : C_FLAGS += -O3
release : $(MAIN_APP)

$(MAIN_APP) : $(OBJS) $(RES_OUT)
	$(CC) $(L_FLAGS) $(OBJS_BUILD) $(RES_OUT) -o $@

%.o : %.c
	$(CC) $(C_FLAGS) $(I_FLAGS) -c $< -o $(addprefix $(BUILD_DIR), $(notdir $@))

clear : 
	rm -f $(MAIN_APP)
	rm -f $(OBJS_BUILD)
	rm -f $(RES_OUT)
