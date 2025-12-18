#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50

typedef struct _directory* Directory;

typedef struct _directory {
    char name[MAX_NAME];
    Directory parent;
    Directory child;
    Directory sibling;
} directory;

Directory createDirectory(const char* name, Directory parent);
int addDirectory(Directory current, const char* name);
int listDirectory(Directory current);
Directory changeDirectory(Directory current, const char* name);
Directory moveToParent(Directory current);
int freeTree(Directory dir);


int main() {
    Directory root = createDirectory("root", NULL);
    Directory current = root;

    int choice;
    char name[MAX_NAME];

    do {
        printf("\n-- Menu --\n");
        printf("1. md (make directory)\n");
        printf("2. cd dir (enter directory)\n");
        printf("3. cd.. (go back)\n");
        printf("4. dir (list contents)\n");
        printf("5. exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        switch (choice) {
        case 1:
            printf("Enter directory name: ");
            if (scanf("%49s", name) != 1) break;
            addDirectory(current, name);
            break;

        case 2:
            printf("Enter directory name: ");
            if (scanf("%49s", name) != 1) break;
            current = changeDirectory(current, name);
            break;

        case 3:
            current = moveToParent(current);
            break;

        case 4:
            listDirectory(current);
            break;

        case 5:
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid option\n");
        }

        while ((c = getchar()) != '\n' && c != EOF);
    } while (choice != 5);

    freeTree(root);
    return 0;
}


Directory createDirectory(const char* name, Directory parent) {
    Directory newDir = malloc(sizeof(directory));
    if (!newDir) {
        printf("neuspjela alokacija memorije\n");
        return EXIT_FAILURE;
    }

    strncpy(newDir->name, name, MAX_NAME - 1);
    newDir->name[MAX_NAME - 1] = '\0';

    newDir->parent = parent;
    newDir->child = NULL;
    newDir->sibling = NULL;

    return newDir;
}

int addDirectory(Directory current, const char* name) {
    Directory t = current->child;
    while (t) {
        if (strcmp(t->name, name) == 0) {
            printf("Direktorij '%s' vec postoji u  '%s'\n", name, current->name);
            return;
        }
        t = t->sibling;
    }

    Directory newDir = createDirectory(name, current);

    if (!current->child) {
        current->child = newDir;
    }
    else {
        Directory temp = current->child;
        while (temp->sibling) temp = temp->sibling;
        temp->sibling = newDir;
    }

    printf("Direktorij '%s' kreiran u '%s'\n", name, current->name);

    return 0;
}

int listDirectory(Directory current) {
    if (!current->child) {
        printf("Directorij je prazan\n");
        return;
    }

    Directory temp = current->child;
    printf("\nsadrzaj s '%s':\n", current->name);

    while (temp) {
        printf("  %s\n", temp->name);
        temp = temp->sibling;
    }
    return 0;
}

Directory changeDirectory(Directory current, const char* name) {
    Directory temp = current->child;

    while (temp) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->sibling;
    }

    printf("Direktorij '%s' ne postoji u '%s'\n", name, current->name);
    return current;
}

Directory moveToParent(Directory current) {
    if (current->parent) return current->parent;
    printf("vec u root direktoriju\n");
    return current;
}

int freeTree(Directory dir) {
    if (!dir) return;
    if (dir->child) freeTree(dir->child);
    if (dir->sibling) freeTree(dir->sibling);
    free(dir);
    return 0;
}

