#include <iostream>
#include "fs.cpp"

int main(int argc, char* argv[]) {


    return fuse_main( argc, argv,&operations, NULL);
}
