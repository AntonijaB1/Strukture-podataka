#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct Node* position;
typedef struct Node {
    int value;
    position left;
    position right;
}Node;

position createElement(int value);
position insert(position root, int value);
int inorder(position root, FILE* file);
int replace(position current);
int freeTree(position root);

int main() {

    srand((unsigned)time(NULL));

    position root = NULL;
    int i;

    for (i = 0; i < 10; i++) {
        int random = rand() % (90 - 10 + 1) + 10;

        printf("Dodan broj: %d\n", random);
        root = insert(root, random);
    }

    FILE* file = fopen("stablo_prije.txt", "w");
    if (file == NULL) {
        printf("Greska pri otvaranju datoteke.\n");
        return 1;
    }

    inorder(root, file);

    fclose(file);


    replace(root);


    file = fopen("stablo_after.txt", "w");
    if (file == NULL) {
        printf("Greska pri otvaranju datoteke.\n");
        return 1;
    }

    inorder(root, file);

    fclose(file);

    freeTree(root);
    return EXIT_SUCCESS;
}

position createElement(int value) {
    position new = (position)malloc(sizeof(Node));
    if (new == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return NULL;
    }
    new->value = value;
    new->left = NULL;
    new->right = NULL;
    return new;
}

position insert(position root, int value) {
    if (root == NULL)return createElement(value);

    if (value >= root->value) {
        root->left = insert(root->left, value);
    }
    else if (value < root->value) {
        root->right = insert(root->right, value);
    }

    return root;
}

int inorder(position root, FILE* file) {
    if (root) {
        inorder(root->left, file);
        fprintf(file, "%d ", root->value);
        inorder(root->right, file);
    }
    return EXIT_SUCCESS;
}

int replace(position current) {
    if (current == NULL)return 0;

    int old_value = current->value;

    current->value = replace(current->left) + replace(current->right);

    return old_value + current->value;
}

int freeTree(position root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
    return EXIT_SUCCESS;
}

