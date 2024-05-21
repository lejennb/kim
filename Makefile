CC=gcc

.PHONY: base debug static stripped
base:
	$(CC) src/*.c -o src/kim

debug:
	$(CC) -g src/*.c -o src/kim

static:
	$(CC) -static src/*.c -o src/kim

stripped:
	$(CC) -s src/*.c -o src/kim
