CC=gcc
CFLAGS=-Wall -g 
BINARY=icsh
SRC = src/icsh.c src/command.c src/signal.c src/input.c src/redirect.c src/job.c

all: $(BINARY)

$(BINARY): $(SRC)
	$(CC) $(CFLAGS) -o $(BINARY) $(SRC)

.PHONY: clean

clean:
	rm -f $(BINARY)