CC=gcc
CFLAGS=-Wall -Wextra -Iinclude

SRCDIR=src
OBJDIR=obj
EXECDIR=bin

SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXEC=$(EXECDIR)/supemon.exe

all: ${EXEC}

${EXEC}: ${OBJS} | ${EXECDIR}
	${CC} $^ -o $@

${OBJDIR}/%.o: ${SRCDIR}/%.c | ${OBJDIR}
	${CC} ${CFLAGS} -c $< -o $@

${OBJDIR}:
	mkdir ${OBJDIR}

${EXECDIR}:
	mkdir ${EXECDIR}

clean:
	del /Q ${OBJDIR}\*.o ${EXECDIR}\*.exe

run: all
	${EXEC}

.PHONY: all clean run