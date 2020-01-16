#include <iostream>
#include "proc.cpp"
#include "fs.h"

int main(int argc, char* argv[]) {
    // proc::ProcessScraper ps;   /// obiect
    // ps.printProcesses();
    return fuse_main( argc, argv,&operations, NULL);
}
