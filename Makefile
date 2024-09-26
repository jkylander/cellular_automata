CFLAGS=-Wall -Wextra -std=c99
TARGET ?= cell 
BUILD_DIR := ./build
SRC_DIRS ?= ./src
SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(addsuffix .o, $(basename $(SRCS)))

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

LDLIBS=-lraylib -lgdi32 -lwinmm
LDFLAGS=-L --subsystem,windows
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@  $(LDLIBS) $(CFLAGS) $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)

.PHONY: run
run:
	$(BUILD_DIR)/$(TARGET)
# end
