TARGET ?= a.out
CC = g++
LFLAGS = -lsqlite3 -ljsoncpp

SRC_DIRS = /Users/nick/software/projects/love-letter/
SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I, $(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=c++17

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
