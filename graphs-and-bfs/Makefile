CC = gcc

CFLAGS = -Wall -Wextra -pedantic -I../hash-table

BIN = bin

all: $(BIN)/main_ex4 $(BIN)/test_ex4

$(BIN):
	mkdir -p $(BIN)

$(BIN)/main_ex4: main_ex4.c graph.c graph.h breadth_first_visit.c breadth_first_visit.h ../hash-table/hashtable.c ../hash-table/hashtable.h | $(BIN)
	$(CC) $(CFLAGS) main_ex4.c graph.c breadth_first_visit.c ../hash-table/hashtable.c -o $@

$(BIN)/test_ex4: test_ex4.c graph.c graph.h breadth_first_visit.c breadth_first_visit.h ../hash-table/hashtable.c ../hash-table/hashtable.h | $(BIN)
	$(CC) $(CFLAGS) test_ex4.c graph.c breadth_first_visit.c ../hash-table/hashtable.c -o $@

clean:
	rm -rf $(BIN)
	rm -f main_ex4 test_ex4