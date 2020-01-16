procfs: main.cpp proc.cpp fs.h
	mkdir -p bin && g++ -o bin/procfs main.cpp -D_FILE_OFFSET_BITS=64 -std=c++17 -lstdc++fs -pthread `pkg-config fuse --cflags --libs`
