#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 11

typedef struct CityTree {
    char name[50];
    int numCitizens;
    struct CityTree* left;
    struct CityTree* right;
} CityTree;

typedef struct StateNode {
    char name[50];
    CityTree* root;
    struct StateNode* next;
} StateNode;



CityTree* createCityNode(char* name, int population);
CityTree* insertCityTree(CityTree* root, CityTree* newNode);
CityTree* readCitiesFromFile(char* filename);
void printCityTree(CityTree* root);
void searchByPopulationTree(CityTree* root, int minPopulation);

int hashFunction(char* name);

StateNode* createStateNode(char* name);
StateNode* insertStateSorted(StateNode* head, StateNode* newNode);
void printHashTable(StateNode* table[]);
StateNode* findState(StateNode* table[], char* name);
void readFromFileHash(char* filename, StateNode* table[]);


int main() {
    StateNode* table[TABLE_SIZE] = { 0 };

    readFromFileHash("drzave.txt", table);

    printf("hash tablica drzava\n");
    printHashTable(table);

    char state[50];
    int minPop;

    printf("\n[PRETRAGA]\n");
    printf("Unesite ime drzave: ");
    scanf("%49s", state);
    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &minPop);

    StateNode* found = findState(table, state);
    if (!found)
        printf("Drzava ne postoji!\n");
    else
        searchByPopulationTree(found->root, minPop);

    return 0;
}



CityTree* createCityNode(char* name, int population) {
    CityTree* node = malloc(sizeof(CityTree));
    if (!node) return NULL;
    strcpy(node->name, name);
    node->numCitizens = population;
    node->left = node->right = NULL;
    return node;
}

CityTree* insertCityTree(CityTree* root, CityTree* newNode) {
    if (!root) return newNode;

    if (newNode->numCitizens < root->numCitizens)
        root->left = insertCityTree(root->left, newNode);
    else if (newNode->numCitizens > root->numCitizens)
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

void printCityTree(CityTree* root) {
    if (!root) return;
    printCityTree(root->left);
    printf("  %s (%d)\n", root->name, root->numCitizens);
    printCityTree(root->right);
}

void searchByPopulationTree(CityTree* root, int minPopulation) {
    if (!root) return;
    searchByPopulationTree(root->left, minPopulation);
    if (root->numCitizens > minPopulation)
        printf("  %s (%d)\n", root->name, root->numCitizens);
    searchByPopulationTree(root->right, minPopulation);
}

int hashFunction(char* name) {
    int sum = 0;
    for (int i = 0; i < 5 && name[i]; i++)
        sum += name[i];
    return sum % TABLE_SIZE;
}

StateNode* createStateNode(char* name) {
    StateNode* node = malloc(sizeof(StateNode));
    if (!node) return NULL;
    strcpy(node->name, name);
    node->root = NULL;
    node->next = NULL;
    return node;
}

StateNode* insertStateSorted(StateNode* head, StateNode* newNode) {
    if (!head || strcmp(newNode->name, head->name) < 0) {
        newNode->next = head;
        return newNode;
    }

    StateNode* curr = head;
    while (curr->next && strcmp(newNode->name, curr->next->name) > 0)
        curr = curr->next;

    newNode->next = curr->next;


