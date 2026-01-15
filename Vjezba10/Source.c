#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CityTree {
    char name[50];
    int num_citizens;
    struct CityTree* left;
    struct CityTree* right;
} CityTree;

typedef struct StateList {
    char name[50];
    CityTree* root;
    struct StateList* next;
} StateList;

typedef struct CityList {
    char name[50];
    int num_citizens;
    struct CityList* next;
} CityList;

typedef struct StateTree {
    char name[50];
    CityList* head_list;
    struct StateTree* left;
    struct StateTree* right;
} StateTree;


StateList* readFromFileList(char* filename);
StateList* createStateListNode(char* name);
StateList* insertStateSorted(StateList* head, StateList* newNode);
CityTree* createCityNode(char* name, int population);
CityTree* insertCityTree(CityTree* root, CityTree* newNode);
CityTree* readCitiesFromFile(char* filename);
int printCityTree(CityTree* root);
int searchCityTree(CityTree* root, int min_population);
StateList* findState(StateList* head, char* name);

StateTree* insertStateTree(StateTree* root, StateTree* newNode);
CityList* insertCityList(CityList* head, CityList* newNode);
StateTree* createStateTreeNode(char* name);
CityList* createCityListNode(char* name, int population);
CityList* readCitiesToList(char* filename);
StateTree* readFromFileTree(char* filename);
int printStateTree(StateTree* root);
int searchCityList(CityList* head, int min_population);
StateTree* findStateTree(StateTree* root, char* name);


int main() {
    char state_name[50];
    int min_population;

    StateList* state_list = readFromFileList("drzave.txt");

    printf("\nsortirana lista drzava\n");
    StateList* temp = state_list;
    while (temp) {
        printf("%s\n", temp->name);
        printCityTree(temp->root);
        temp = temp->next;
    }

    printf("\n[PRETRAGA - DIO A]\n");
    printf("Unesite ime drzave: ");
    scanf("%49s", state_name);

    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &min_population);

    StateList* found_list = findState(state_list, state_name);
    if (!found_list) {
        printf("Drzava ne postoji!\n");
    }
    else {
        printf("Gradovi s vise od %d stanovnika:\n", min_population);
        searchCityTree(found_list->root, min_population);
    }

    StateTree* state_tree = readFromFileTree("drzave.txt");

    printf("\n-- STABLO DRZAVA --\n");
    printStateTree(state_tree);

    printf("\n[PRETRAGA - DIO B]\n");
    printf("Unesite ime drzave: ");
    scanf("%49s", state_name);

    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &min_population);

    StateTree* found_tree = findStateTree(state_tree, state_name);
    if (!found_tree) {
        printf("Drzava ne postoji!\n");
    }
    else {
        printf("Gradovi s vise od %d stanovnika:\n", min_population);
        searchCityList(found_tree->head_list, min_population);
    }

    return 0;
}

//lista drzava i stablo gradova

StateList* readFromFileList(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    StateList* head = NULL;
    char state_name[50], city_file[50];

    while (fscanf(file, "%49s %49s", state_name, city_file) == 2) {
        StateList* newState = createStateListNode(state_name);
        newState->root = readCitiesFromFile(city_file);
        head = insertStateSorted(head, newState);
    }

    fclose(file);
    return head;
}

StateList* createStateListNode(char* name) {
    StateList* node = malloc(sizeof(StateList));
    strcpy(node->name, name);
    node->root = NULL;
    node->next = NULL;
    return node;
}

StateList* insertStateSorted(StateList* head, StateList* newNode) {
    if (!head || strcmp(newNode->name, head->name) < 0) {
        newNode->next = head;
        return newNode;
    }

    StateList* curr = head;
    while (curr->next && strcmp(newNode->name, curr->next->name) > 0)
        curr = curr->next;

    newNode->next = curr->next;
    curr->next = newNode;
    return head;
}

//stavblo gradova

CityTree* createCityNode(char* name, int population) {
    CityTree* node = malloc(sizeof(CityTree));
    strcpy(node->name, name);
    node->num_citizens = population;
    node->left = node->right = NULL;
    return node;
}

CityTree* insertCityTree(CityTree* root, CityTree* newNode) {
    if (!root) return newNode;

    if (newNode->num_citizens < root->num_citizens)
        root->left = insertCityTree(root->left, newNode);
    else if (newNode->num_citizens > root->num_citizens)
        root->right = insertCityTree(root->right, newNode);
    else if (strcmp(newNode->name, root->name) < 0)
        root->left = insertCityTree(root->left, newNode);
    else
        root->right = insertCityTree(root->right, newNode);

    return root;
}

CityTree* readCitiesFromFile(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    CityTree* root = NULL;
    char city[50];
    int pop;

    while (fscanf(file, " %49[^,], %d", city, &pop) == 2) {
        CityTree* node = createCityNode(city, pop);
        root = insertCityTree(root, node);
    }

    fclose(file);
    return root;
}

int printCityTree(CityTree* root) {
    if (!root) return;
    printCityTree(root->left);
    printf("  %s (%d)\n", root->name, root->num_citizens);
    printCityTree(root->right);
    return 0;
}

int searchCityTree(CityTree* root, int min_population) {
    if (!root) return;

    if (root->num_citizens > min_population) {
        searchCityTree(root->left, min_population);
        printf("  %s (%d)\n", root->name, root->num_citizens);
        searchCityTree(root->right, min_population);
    }
    else {
        searchCityTree(root->right, min_population);
    }
    return 0;
}

StateList* findState(StateList* head, char* name) {
    while (head) {
        if (strcmp(head->name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

//stablo drzava lista gradova

StateTree* createStateTreeNode(char* name) {
    StateTree* node = malloc(sizeof(StateTree));
    strcpy(node->name, name);
    node->head_list = NULL;
    node->left = node->right = NULL;
    return node;
}

CityList* createCityListNode(char* name, int population) {
    CityList* node = malloc(sizeof(CityList));
    strcpy(node->name, name);
    node->num_citizens = population;
    node->next = NULL;
    return node;
}

CityList* insertCityList(CityList* head, CityList* newNode) {
    if (!head ||
        newNode->num_citizens < head->num_citizens ||
        (newNode->num_citizens == head->num_citizens &&
            strcmp(newNode->name, head->name) < 0)) {

        newNode->next = head;
        return newNode;
    }

    CityList* curr = head;
    while (curr->next &&
        (curr->next->num_citizens < newNode->num_citizens ||
            (curr->next->num_citizens == newNode->num_citizens &&
                strcmp(curr->next->name, newNode->name) < 0))) {
        curr = curr->next;
    }

    newNode->next = curr->next;
    curr->next = newNode;
    return head;
}

CityList* readCitiesToList(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    CityList* head = NULL;
    char city[50];
    int pop;

    while (fscanf(file, " %49[^,], %d", city, &pop) == 2) {
        CityList* node = createCityListNode(city, pop);
        head = insertCityList(head, node);
    }

    fclose(file);
    return head;
}

StateTree* insertStateTree(StateTree* root, StateTree* newNode) {
    if (!root) return newNode;

    if (strcmp(newNode->name, root->name) < 0)
        root->left = insertStateTree(root->left, newNode);
    else
        root->right = insertStateTree(root->right, newNode);

    return root;
}

StateTree* readFromFileTree(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    StateTree* root = NULL;
    char state[50], city_file[50];

    while (fscanf(file, "%49s %49s", state, city_file) == 2) {
        StateTree* node = createStateTreeNode(state);
        node->head_list = readCitiesToList(city_file);
        root = insertStateTree(root, node);
    }

    fclose(file);
    return root;
}

int printStateTree(StateTree* root) {
    if (!root) return;

    printStateTree(root->left);
    printf("%s\n", root->name);

    CityList* c = root->head_list;
    while (c) {
        printf("  %s (%d)\n", c->name, c->num_citizens);
        c = c->next;
    }

    printStateTree(root->right);
    return 0;
}

 int searchCityList(CityList* head, int min_population) {
     while (head) {
        if (head->num_citizens > min_population)
            printf("  %s (%d)\n", head->name, head->num_citizens);
        head = head->next;
     }
     return 0;
}

StateTree* findStateTree(StateTree* root, char* name) {
    if (!root) return NULL;

    int cmp = strcmp(name, root->name);
    if (cmp == 0) return root;
    if (cmp < 0) return findStateTree(root->left, name);
    return findStateTree(root->right, name);
}










