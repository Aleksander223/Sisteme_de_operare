/*
  Tree data structure
*/
#pragma once
#include <vector>

struct TreeNode {
    int pid;
    TreeNode* parent;
    std::vector<TreeNode*> children;
};

class Tree {
private:
    TreeNode* root;

    // helper function to find a node
    TreeNode* findNode(int id, TreeNode* aux) {
        if (aux->pid == id) {
            return aux;
        }

        TreeNode* tmp;

        for (int i = 0; i < aux->children.size(); i++) {
            tmp =  findNode(id, aux->children[i]);

            if (tmp) {
                return tmp;
            }
        }

        return NULL;
    }

public:
    Tree() {
        root = NULL;
    }

    TreeNode* find(int id) {
        return findNode(id, root);
    }

    void addRoot(int id) {
        root = new TreeNode;

        root->pid = id;
        root->parent = NULL;
    }

    void addNode(int id, int par) {
        TreeNode* tmp = new TreeNode;

        TreeNode* parent = find(par);

        if (parent == NULL) {
            return;
        }

        tmp->pid = id;
        tmp->parent = parent;

        parent->children.push_back(tmp);
    }
};
