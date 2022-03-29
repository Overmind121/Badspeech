#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "node.h"

uint64_t branches = 0;

Node *bst_create(void) {
    Node *bst = NULL;
    return bst;
}

// Cited from Professor Darrel Long's Lecture
void bst_delete(Node **root) {
    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
    return;
}

// Cited from Professor Darrel Long's Lecture
static int max(int x, int y) {
    return x > y ? x : y;
}

// Cited from Professor Darrel Long's Lecture
uint32_t bst_height(Node *root) {
    if (root) {
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
    return 0;
}

uint32_t bst_size(Node *root) {
    if (root) {
        return 1 + bst_size(root->left) + bst_size(root->right);
    }
    return 0;
}

// Cited from Professor Darrel Long's Lecture
Node *bst_find(Node *root, char *oldspeak) {
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            branches += 1;
            return bst_find(root->left, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            branches += 1;
            return bst_find(root->right, oldspeak);
        }
    }
    return root;
}

// Cited from Professor Darrel Long's Lecture
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (oldspeak == NULL) {
        return root;
    }
    if (!root) {
        return node_create(oldspeak, newspeak);
    } else {

        if (strcmp(root->oldspeak, oldspeak) > 0) {
            branches += 1;
            root->left = bst_insert(root->left, oldspeak, newspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            branches += 1;
            root->right = bst_insert(root->right, oldspeak, newspeak);
        }
        return root;
    }
}

// Cited from Professor Darrel Long's Lecture
void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
}
