procfs: main.cpp tree.cpp
	mkdir -p bin && g++ -o bin/procfs main.cpp -D_FILE_OFFSET_BITS=64
