# Simple makefile by Philip R. Simonson
CC=gcc
CFLAGS+=-Wall -W -O -pedantic -std=c99 -g
LDFLAGS+=

SOURCES:=$(wildcard *.c)
OBJECTS:=$(SOURCES:%.c=%.c.o)
TARGETS:=$(SOURCES:%.c=%)

.PHONY: all clean
all: $(TARGETS)

%.c.o: %.c
	@echo CC $(CFLAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -c -o $@ $<

%: %.c.o
	@echo CCLD $(CFLAGS) $(LDFLAGS) -o $@ $<
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(TARGETS): $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(TARGETS)
