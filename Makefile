# Build mode, DEBUG or RELEASE
BUILD_MODE ?= DEBUG
TARGET ?= cell 
BUILD_DIR := build
SRC_DIRS ?= src
SRCS := $(shell find $(SRC_DIRS) -name *.c)

# Compiler flags
CFLAGS=-Wall -Wextra -std=c11
# Ignore warnings 
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-missing-braces
CFLAGS += -Wno-unused-value
CFLAGS += -Wno-pointer-sign
ifeq ($(BUILD_MODE),DEBUG)
	CFLAGS += -g -D_DEBUG
endif


ifeq ($(OS), Windows_NT)
	LDLIBS = -lraylib -lgdi32 -lwinmm -lopengl32 -lcomdlg32 -lole32
	LDFLAGS=-L --subsystem,windows
else
	UNAMEOS = $(shell uname)
	ifeq ($(UNAMEOS),Darwin)
		CC = clang
		LDLIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo libraylib.a
	endif
	ifeq ($(UNAMEOS),Linux)
		LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt
		ifeq ($(USE_WAYLAND_DISLAY),TRUE)
			LDLIBS += -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon
		else
			LDLIBS += -lX11
		endif
	endif
endif

$(BUILD_DIR)/$(TARGET):
	@mkdir -p $(BUILD_DIR)
	$(CC) $(SRCS) -o $@  $(LDLIBS) $(CFLAGS) $(LDFLAGS)


.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)/$(TARGET)

.PHONY: run
run:
	$(BUILD_DIR)/$(TARGET)
# end
