#include <iostream>
#include "proc.cpp"
#include "fs.cpp"

int main(int argc, char* argv[]) {
    proc::ProcessScraper ps;   /// obiect

    return fuse_main( argc, argv,&operations, NULL);
}
