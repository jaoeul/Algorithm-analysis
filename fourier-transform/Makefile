CC=clang++

SRC_FILES := main.cpp
SRC_FILES += algo.cpp
#CFLAGS    := -Werror -Wall
LD_FLAGS  := -lraylib -lm

.PHONY: app
all: app
	$(CC) $(CFLAGS) -o $? $(SRC_FILES) $(LD_FLAGS)
