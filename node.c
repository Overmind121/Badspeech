#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node Node;
struct Node {
    char *oldspeak;
    char *newspeak;
    Node *left;
    Node *right;
};

Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (oldspeak) {
        n->oldspeak = strdup(oldspeak);
        if (!newspeak) {
            n->newspeak = NULL;
        } else {
            n->newspeak = strdup(newspeak);
        }
        n->left = NULL;
        n->right = NULL;
    } else {
        free(n);
        n = NULL;
    }
    return n;
}

void node_delete(Node **n) {
    free((*n)->oldspeak);
    free((*n)->newspeak);
    free(*n);
    n = NULL;

    return;
}

void node_print(Node *n) {
    if (!n) {
        return;
    }
    if (n->oldspeak && n->newspeak) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else {
        printf("%s\n", n->oldspeak);
    }

    return;
}
