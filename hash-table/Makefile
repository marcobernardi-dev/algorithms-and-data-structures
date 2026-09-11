CC = gcc

CFLAGS = -Wall -Wextra -pedantic

BIN = bin

all: $(BIN)/main_ex2 $(BIN)/test_ex2

$(BIN):
	mkdir -p $(BIN)

$(BIN)/main_ex2: main_ex2.c hashtable.c hashtable.h | $(BIN)
	$(CC) $(CFLAGS) main_ex2.c hashtable.c -o $(BIN)/main_ex2

$(BIN)/test_ex2: test_ex2.c hashtable.c hashtable.h | $(BIN)
	$(CC) $(CFLAGS) test_ex2.c hashtable.c -o $(BIN)/test_ex2

clean:
	rm -f $(BIN)/main_ex2 $(BIN)/test_ex2