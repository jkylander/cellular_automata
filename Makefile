##
# Cellular Automata
#
# @file
# @version 0.1
CC= cc 
CFLAGS=-Wall -Wextra -std=c99
EXE = .exe
TARGET ?= cell$(EXE)
SRC_DIRS ?= ./src
SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(addsuffix .o, $(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

#CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

LDLIBS=-lraylib -lgdi32 -lwinmm
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@  $(LDLIBS) $(CFLAGS) 

.PHONE: run
run:
	$(TARGET)
.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
# end
