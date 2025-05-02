#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* new = (TreeMap*) malloc(sizeof(TreeMap));
    if(new == NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if(searchTreeMap(tree, key) != NULL) return;
    TreeNode* parent = NULL;
    TreeNode* current = tree->root;
    while(current != NULL) {
        parent = current;
        if(tree->lower_than(key, current->pair->key)) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    TreeNode* new_node = createTreeNode(key, value);
    new_node->parent = parent;
    if(tree->lower_than(key, parent->pair->key)) {
        parent->left = new_node;
    }
    else {
        parent->right = new_node;
    }
    tree->current = new_node;
}

TreeNode * minimum(TreeNode * x){
    if(x == NULL) return NULL;
    while(x->left != NULL){
        x = x->left;
    }
    return x;
}

void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL) {
        if (node->parent->left == node) {
            node->parent->left = NULL;
        }
        else {
            node->parent->right = NULL;
        }
        free(node->pair);
        free(node);
    }
    else if (node->left == NULL || node->right == NULL) { 
        TreeNode* child;
        if(node->left != NULL) {
            child = node->left;
        }
        else {
            child = node->right;
        }
        if (node->parent->left == node) {
            node->parent->left = child;
        }
        else {
            node->parent->right = child;
        }
        child->parent = node->parent;
        free(node->pair);
        free(node);
    }
    else { 
        TreeNode* minNode = minimum(node->right);
        node->pair->key = minNode->pair->key;
        node->pair->value = minNode->pair->value;
        removeNode(tree, minNode);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* node = tree->root;
    while(node != NULL) {
        if(is_equal(tree, key, node->pair->key)) {
            tree->current = node;
            return node->pair;
        }
        else if(tree->lower_than(key, node->pair->key)) {
            node = node->left;
        }
        else {
            node = node->right;
        }
    }
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode* minNode = minimum(tree->root);
    tree->current = minNode;
    return minNode->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode* current = tree->current;
    if (current->right != NULL) {
        TreeNode* minNode = minimum(current->right);
        tree->current = minNode;
        return minNode->pair;
    }

    TreeNode* parent = current->parent;
    while (parent != NULL && current == parent->right) {
        current = parent;
        parent = parent->parent;
    }
    tree->current = parent;
    if (parent == NULL) return NULL;
    return parent->pair;
}

