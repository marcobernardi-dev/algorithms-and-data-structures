CC = gcc
CFLAGS = -Wall -Wextra -pedantic -I../hash-table

BIN = bin

all: $(BIN)/main_ex3 $(BIN)/test_ex3

$(BIN):
	mkdir -p $(BIN)

$(BIN)/main_ex3: main_ex3.c priority_queue.c priority_queue.h ../hash-table/hashtable.c ../hash-table/hashtable.h | $(BIN)
	$(CC) $(CFLAGS) main_ex3.c priority_queue.c ../hash-table/hashtable.c -o $@

$(BIN)/test_ex3: test_ex3.c priority_queue.c priority_queue.h ../hash-table/hashtable.c ../hash-table/hashtable.h | $(BIN)
	$(CC) $(CFLAGS) test_ex3.c priority_queue.c ../hash-table/hashtable.c -o $@

clean:
	rm -f $(BIN)/main_ex3 $(BIN)/test_ex3