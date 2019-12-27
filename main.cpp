//
// Created by aleks on 12/27/19.
//

#include <iostream>
#include <fuse.h>
#include "tree.cpp"

int main() {
    Tree t;

    t.addRoot(1);
    t.addNode(2, 1);
    t.addNode(3, 1);
    t.addNode(4, 3);

    TreeNode* tmp = t.find(4);

    if (tmp) {
        std::cout<<tmp->pid<<std::endl;
    }
    return 0;
}
