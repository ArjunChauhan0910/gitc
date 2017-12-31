CC=gcc
CFLAGS=-Wall
LDFLAGS=-lncurses -lmenu -lgit2
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
OBIN=gitc

all:
	$(CC)  $(CFLAGS) src/gitc.c src/main.c $(LDFLAGS) -o $(OBIN)

install:
	install -Dm755 $(OBIN) $(BINDIR)/$(OBIN)


uninstall:
	rm -f $(BINDIR)/$(OBIN)


