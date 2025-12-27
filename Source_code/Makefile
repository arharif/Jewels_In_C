# CC=gcc or clang or gcc-13 (on OSX)
CC ?= gcc

# If a scanner+parser is needed.
LEX=flex
YACC=bison
YFLAGS=-d -v -t

# ---- SDL2 flags ----
SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LIBS   := $(shell sdl2-config --libs)

# Math, big numbers and SDL2 extra libs
LDLIBS = -lm -lgmp -lSDL2_gfx -lSDL2_ttf -lSDL2_image $(SDL2_LIBS)

# Base compile flags
override CFLAGS += -std=gnu2x -MMD -Wall -pedantic -Wextra -Wshadow -Wpointer-arith \
                   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes

# Add this flag ONLY for clang (gcc doesn't know it)
ifeq ($(findstring clang,$(shell $(CC) --version 2>/dev/null)),clang)
override CFLAGS += -Wno-gnu-zero-variadic-macro-arguments
endif

# Preprocessor flags (includes, defines)
CPPFLAGS += $(SDL2_CFLAGS)

# For MacOS (assuming recent homebrew)
ifeq ($(shell uname -s),Darwin)
	CPPFLAGS += -I/opt/homebrew/include
	LDFLAGS  += -L/opt/homebrew/lib
endif

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:%.c=%.o)
DEPS    := $(SOURCES:%.c=%.d)

# Debug mode by default
all: CFLAGS += -g -O0
all: projet

# Optimized mode (no -g)
nowerror: CFLAGS += -O3
nowerror: projet

projet: $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf projet *.o *.d TAGS core
