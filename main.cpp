#include <iostream>
#include <fuse.h>
#include "proc.cpp"

int main() {
    proc::ProcessScraper ps;   /// obiect
    ps.printProcesses();
    return 0;
}
