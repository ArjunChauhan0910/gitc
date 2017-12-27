CC=gcc
CFLAGS=-Wall
LDFLAGS=-lncurses -lmenu -lgit2
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
OBIN=gitc

all:
	@echo "Run make to compile and build"
	@echo "Run make install to install"
	@echo "Run make clean to uninstall"

install: 
	$(CC) $(CFLAGS) src/gitc_main.c src/gitc_headers.c $(LDFLAGS) -o $(OBIN)
	cp -f $(OBIN) $(BINDIR)/


uninstall:
	rm -f $(BINDIR)/$(OBIN)


