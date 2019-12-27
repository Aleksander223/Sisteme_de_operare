procfs: main.c
	mkdir -p bin && gcc -o bin/procfs main.c -D_FILE_OFFSET_BITS=64 -std=c11
