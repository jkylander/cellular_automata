##
# Cellular Automata
#
# @file
# @version 0.1
CC= clang
TARGET ?= a.out
SRC_DIRS ?= ./src
SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(addsuffix .o, $(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)
CFLAGS=-Wall -Wextra

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

#CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

LDLIBS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDLIBS)
.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
# end
