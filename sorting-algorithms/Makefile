CC = gcc
CFLAGS = -Wall -Wextra -pedantic

BIN = bin

all: $(BIN)/main_ex1 $(BIN)/test_ex1

$(BIN):
	mkdir -p $(BIN)

$(BIN)/main_ex1: main_ex1.c hybridsort.c hybridsort.h record.c record.h | $(BIN)
	$(CC) $(CFLAGS) main_ex1.c hybridsort.c record.c -o bin/main_ex1

$(BIN)/test_ex1: test_ex1.c hybridsort.c hybridsort.h record.c record.h | $(BIN)
	$(CC) $(CFLAGS) test_ex1.c hybridsort.c record.c -o bin/test_ex1

clean:
	rm -f $(BIN)/main_ex1 $(BIN)/test_ex1
