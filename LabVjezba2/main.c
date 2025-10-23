#define _CRT_SECURE_NO_WARNINGS
/*
2. Definirati strukturu osoba (ime, prezime, godina rodjenja) i napisati program koji:
A. dinamicki dodaje novi element na pocetak liste,
B. ispisuje listu,
C. dinamicki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. brise odredjeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct person;
typedef struct person* position;
typedef struct person {
    char name[50];
    char surname[50];
    int birthYear;
    position next;
} Person;

// Prototipi funkcija
Person* addAtBeginning(Person* head, char* name, char* surname, int birthYear);
Person* addAtEnd(Person* head, char* name, char* surname, int birthYear);
int printList(Person* head);
Person* searchBySurname(Person* head, char* surname);
Person* deleteElement(Person* head, char* surname);
int freeList(Person* head);

int main() {
    Person* head = NULL;
    char name[50], surname[50];
    int birthYear, choice;

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1 - Dodaj osobu na pocetak\n");
        printf("2 - Dodaj osobu na kraj\n");
        printf("3 - Ispisi listu\n");
        printf("4 - Pronadi osobu po prezimenu\n");
        printf("5 - Obrisi osobu po prezimenu\n");
        printf("0 - Izlaz\n");
        printf("Vas izbor: ");
        scanf("%d", &choice);
        getchar(); // ocisti buffer poslije scanf

        switch (choice) {
        case 1:
            printf("Unesite ime: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;

            printf("Unesite prezime: ");
            fgets(surname, sizeof(surname), stdin);
            surname[strcspn(surname, "\n")] = 0;

            printf("Unesite godinu rodjenja: ");
            scanf("%d", &birthYear);
            getchar();

            head = addAtBeginning(head, name, surname, birthYear);
            printf("Uspjesan unos!\n");
            break;

        case 2:
            printf("Unesite ime: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;

            printf("Unesite prezime: ");
            fgets(surname, sizeof(surname), stdin);
            surname[strcspn(surname, "\n")] = 0;

            printf("Unesite godinu rodjenja: ");
            scanf("%d", &birthYear);
            getchar();

            head = addAtEnd(head, name, surname, birthYear);
            printf("Uspjesan unos!\n");
            break;

        case 3:
            printList(head);
            break;

        case 4: {
            printf("Unesite prezime za pretragu: ");
            scanf("%49s", surname);
            getchar();

            Person* wanted = searchBySurname(head, surname);
            if (wanted)
                printf("Pronadena osoba: %s %s (%d)\n", wanted->name, wanted->surname, wanted->birthYear);
            else
                printf("Osoba s prezimenom '%s' nije pronadena.\n", surname);
            break;
        }

        case 5:
            printf("Unesite prezime za brisanje: ");
            scanf("%49s", surname);
            getchar();
            head = deleteElement(head, surname);
            break;

        case 0:
            printf("Izlaz iz programa.\n");
            freeList(head);
            break;

        default:
            printf("Greska, izaberi ponovo!\n");
            break;
        }
    } while (choice != 0);

    return 0;
}

//----------------------- FUNKCIJE -----------------------
//trazi od korisnika unos imena prezimena i godine rodjenja i dodaje na pocetak
Person* addAtBeginning(Person* head, char* name, char* surname, int year) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Alokacija nije uspjela.\n");
        return head;
    }

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = year;
    newPerson->next = head;

    return newPerson; // novi head
}

//dodaje na kraj liste
Person* addAtEnd(Person* head, char* name, char* surname, int year) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Alokacija nije uspjela.\n");
        return head;
    }

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = year;
    newPerson->next = NULL;

    if (head == NULL)
        return newPerson;

    Person* temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newPerson;
    return head;
}

//trazi unos prezimena i nadje elemente liste koje sadrzavaju to prezime
Person* searchBySurname(Person* head, char* surname) {
    Person* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->surname, surname) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

//unese korisnik prezime i izbrise elemente s tim prezimenom
Person* deleteElement(Person* head, char* surname) {
    if (head == NULL) {
        printf("Lista je prazna.\n");
        return NULL;
    }

    Person* temp = head;
    Person* prev = NULL;

    while (temp != NULL && strcmp(temp->surname, surname) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Osoba s prezimenom '%s' nije pronadena.\n", surname);
        return head;
    }

    if (prev == NULL) {
        head = temp->next;
    }
    else {
        prev->next = temp->next;
    }

    free(temp);
    printf("Osoba s prezimenom '%s' je obrisana.\n", surname);
    return head;
}

//koristi in a ne void!!!!
int printList(Person* head) {
    if (head == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    printf("\n--- ISPIS LISTE ---\n");
    Person* temp = head;
    while (temp != NULL) {
        printf("%s %s (%d)\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }
    return 0;
}

int freeList(Person* head) {
    Person* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}
