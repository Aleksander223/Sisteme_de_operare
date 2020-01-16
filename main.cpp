#include <iostream>
#include "proc.cpp"

int main(int argc, char* argv[]) {
    proc::ps.printProcesses();
    return 0;

    // return fuse_main( argc, argv,&operations, NULL);
}
