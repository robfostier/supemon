# Détection de l'OS
ifeq ($(OS),Windows_NT)
    EXT=.exe
    RM=del /Q
    MKDIR=mkdir
else
    EXT=
    RM=rm -f
    MKDIR=mkdir -p
endif

# Variables
CC=gcc
CFLAGS=-Wall -Wextra -Iinclude
LDFLAGS=-lm

SRCDIR=src
OBJDIR=obj
EXECDIR=bin

SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXEC=$(EXECDIR)/supemon$(EXT)

# Règles
all: $(EXEC)

$(EXEC): $(OBJS) | $(EXECDIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	$(MKDIR) $(OBJDIR)

$(EXECDIR):
	$(MKDIR) $(EXECDIR)

clean:
	-$(RM) $(OBJDIR)/* $(EXECDIR)/*

run: all
	$(EXEC)

.PHONY: all clean run
