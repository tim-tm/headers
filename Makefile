CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g -ggdb2 -Iinclude/

BUILDDIR=build
SRCDIR=test

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%,$(SRC))

all: $(OBJ)

$(OBJ): always

$(BUILDDIR)/%: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

test: $(OBJ)
	sh test.sh

clean:
	rm -rf $(BUILDDIR)

always:
	mkdir -p $(BUILDDIR)
