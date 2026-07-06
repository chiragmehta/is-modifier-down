CC ?= clang
NAME := is-modifier-down
SRC := $(NAME).c

CFLAGS ?= -arch arm64 -Oz -DNDEBUG -Wall -Wextra -Wpedantic
LDFLAGS ?= -Wl,-dead_strip
FRAMEWORKS := -framework CoreGraphics

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) $(FRAMEWORKS) -o $(NAME)
	strip -x $(NAME)

clean:
	rm -f $(NAME)

.PHONY: all clean
