CC=gcc

.PHONY: debug

debug:
	mkdir -p build
	$(CC) -g src/*.c -o build/kim

