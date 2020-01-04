//
// Created by aleks on 12/27/19.
//

#include <iostream>
#include <fuse.h>
#include "proc.cpp"

int main() {
    proc::ProcessScraper ps;
    ps.printProcesses();
    return 0;
}
