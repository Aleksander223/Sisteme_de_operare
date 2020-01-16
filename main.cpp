#include <iostream>
//#include "fs.cpp"
#include "proc.cpp"
int main(int argc, char* argv[]) {
    return 0;

    return fuse_main( argc, argv,&operations, NULL);

}
