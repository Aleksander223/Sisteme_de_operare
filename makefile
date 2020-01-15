procfs: main.cpp proc.cpp
	mkdir -p bin && g++ -o bin/procfs main.cpp -D_FILE_OFFSET_BITS=64 -std=c++17 -lstdc++fs
