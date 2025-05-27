CC=gcc
CFLAGS=-Wall -g 
BINARY=icsh
SRC = icsh.c command.c signal.c input.c redirect.c

all: $(BINARY)

$(BINARY): $(SRC)
	$(CC) $(CFLAGS) -o $(BINARY) $(SRC)

.PHONY: clean

clean:
	rm -f $(BINARY)