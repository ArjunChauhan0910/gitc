CFLAGS=-Wall -pedantic -ofast
LDFLAGS=-lncurses -lmenu -lgit2
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
OBIN=gitc
CC=c99

all:
	$(CC) $(CFLAGS) src/gitc.c src/main.c $(LDFLAGS) -o $(OBIN)

install:
	install -Dm755 $(OBIN) $(BINDIR)/$(OBIN)
	install -D gitc-completion.bash /usr/share/bash-completion/completions/gitc
	rm $(OBIN)

clean:
	rm $(OBIN)

uninstall:
	rm -f $(BINDIR)/$(OBIN)
	rm -f /usr/share/bash-completion/completions/gitc
