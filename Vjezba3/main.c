#define _CRT_SECURE_NO_WARNINGS

/*Zadatku 2 dodati funkcije :
A.dinamicki dodaje novi element iza odredjenog elementa,
B.dinamicki dodaje novi element ispred odredjenog elementa,
C.sortira listu po prezimenima osoba,
D.upisuje listu u datoteku,
E.cita listu iz datoteke.*/

#include<stdio.h>
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

// Prototipi funkcija(funkcije iz zadatka 2)
Person* addAtBeginning(Person* head, char* name, char* surname, int birthYear);
Person* addAtEnd(Person* head, char* name, char* surname, int birthYear);
int printList(Person* head);
Person* searchBySurname(Person* head, char* surname);
Person* deleteElement(Person* head, char* surname);
int freeList(Person* head);

//prototipi funkcija za zadatak 3
int insertBehind(Person* head, char* targetSurname, char* newName, char* newSurname, int birthYear);
int insertBefore(Person* head, char* targetSurname, char* newName, char* newSurname, int birthYear);
int sortList(Person* head);
int inputFile(Person* head, const char* filename);
int readFile(Person* head, const char* filename);
int deleteAll(Person* head);

int main()
{
    /*head.next pokazuje na prvi element ili NULL*/
    Person head = {
        .name = "",
        .surname = "",
        .birthYear = 0,
        .next = NULL
    };

    int choice;
    char name[50], surname[50];
    int birthYear;

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1 - Dodaj osobu na pocetak\n");
        printf("2 - Dodaj osobu na kraj \n");
        printf("3 - Ispisi listu\n");
        printf("4 - Pronadi osobu po prezimenu\n");
        printf("5 - Obrisi osobu po prezimenu\n");
        printf("6 - Dodaj osobu iza osobe \n");
        printf("7 - Dodaj osobu ispred osobe \n");
        printf("8 - Sortiraj listu po prezimenima\n");
        printf("9 - Upisi listu u datoteku (lista.txt)\n");
        printf("10 - Procitaj listu iz datoteke (lista.txt)\n");
        printf("11 - Obrisi sve elemente\n");
        printf("0 - Izlaz\n");
        printf("Vas izbor: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = -1;
            continue;
        }
        getchar(); // ocisti buffer

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

            head.next = addAtBeginning(head.next, name, surname, birthYear);
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

            head.next = addAtEnd(head.next, name, surname, birthYear);
            printf("Uspjesan unos!\n");
            break;

        case 3:
            printList(head.next);
            break;

        case 4: {
            printf("Unesite prezime za pretragu: ");
            scanf("%49s", surname);
            getchar();

            Person* wanted = searchBySurname(head.next, surname);
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
            head.next = deleteElement(head.next, surname);
            break;

        case 6: {
            printf("Unesite prezime osobe iza koje zelite umetnuti: ");
            char target[50];
            scanf("%49s", target); getchar();

            printf("Unesite ime novo: ");
            fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = '\0';
            printf("Unesite prezime novo: ");
            fgets(surname, sizeof(surname), stdin); surname[strcspn(surname, "\n")] = '\0';
            printf("Unesite godinu rodjenja: ");
            scanf("%d", &birthYear); getchar();

            insertBehind(&head, target, name, surname, birthYear);
            break;
        }

        case 7: {
            printf("Unesite prezime osobe ispred koje zelite umetnuti: ");
            char target[50];
            scanf("%49s", target); getchar();

            printf("Unesite ime novo: ");
            fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = '\0';
            printf("Unesite prezime novo: ");
            fgets(surname, sizeof(surname), stdin); surname[strcspn(surname, "\n")] = '\0';
            printf("Unesite godinu rodjenja: ");
            scanf("%d", &birthYear); getchar();

            insertBefore(&head, target, name, surname, birthYear);
            break;
        }

        case 8:
            sortList(&head);
            printf("Lista je sortirana.\n");
            break;

        case 9:
            inputFile(&head, "lista.txt");
            printf("Upis u datoteku zavrsen.\n");
            break;

        case 10:
            readFile(&head, "lista.txt");
            printf("Citanje iz datoteke zavrseno .\n");
            break;

        case 11:
            deleteAll(&head);
            printf("Svi elementi obrisani.\n");
            break;

        case 0:
            deleteAll(&head);
            printf("Izlaz iz programa.\n");
            break;

        default:
            printf("Greska, izaberi ponovo!\n");
            break;
        }

    } while (choice != 0);

    return 0;
}

/*od korisnika se trazi da unese ime, prezime, godinu rodjenja i dodaje na pocetak liste*/
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

//koristi int a ne void!!!!
int printList(Person* head) {
    if (head == NULL) {
        printf("Lista je prazna.\n");
        return 0;
    }

    printf("\n--- ISPIS LISTE ---\n");
    Person* temp = head;
    while (temp != NULL) {
        printf("%s %s (%d)\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }
    return 0;
}

int insertBehind(Person* head, char* targetSurname, char* newName, char* newSurname, int birthYear)
{
    Person* temp = head->next;
    while (temp != NULL)
    {
        if (strcmp(temp->surname, targetSurname) == 0)
        {
            Person* newPerson = (Person*)malloc(sizeof(Person));
            if (!newPerson)
            {
                printf("Neuspjela alokacija\n");
                return 0;
            }
            strcpy(newPerson->name, newName);
            strcpy(newPerson->surname, newSurname);
            newPerson->birthYear = birthYear;
            newPerson->next = temp->next;
            temp->next = newPerson;
            return 1;
        }
        temp = temp->next;
    }
    printf("osoba s prezimenom %s nije pronadjena.\n", targetSurname);
    return 0;
}

int insertBefore(Person* head, char* targetSurname, char* newName, char* newSurname, int birthYear)
{
    Person* prev = head;
    Person* temp = head->next;

    while (temp != NULL && strcmp(temp->surname, targetSurname) != 0)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("osoba s prezimenom %s nije pronadjena.\n", targetSurname);
        return 0;
    }

    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson)
    {
        printf("Neuspjela alokacija\n");
        return 0;
    }
    strcpy(newPerson->name, newName);
    strcpy(newPerson->surname, newSurname);
    newPerson->birthYear = birthYear;

    newPerson->next = temp;
    prev->next = newPerson;
    return 1;
}

//sortiranje liste
int sortList(Person* head) {
    if (head == NULL || head->next == NULL) return 0;

    Person* end = NULL;
    int swapped;

    do {
        swapped = 0;
        Person* p = head;
        while (p->next != end && p->next->next != NULL) {
            Person* a = p->next;
            Person* b = a->next;
            int cmp = strcmp(a->surname, b->surname);
            if (cmp > 0 || (cmp == 0 && strcmp(a->name, b->name) > 0)) {
                p->next = b;
                a->next = b->next;
                b->next = a;
                swapped = 1;
            }
            p = p->next;
        }
        end = p->next;
    } while (swapped);
    return 0;
}

int inputFile(Person* head, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Ne mogu otvoriti datoteku za pisanje.\n");
        return 1;
    }

    Person* x = head->next;
    while (x != NULL)
    {
        fprintf(file, "%s %s %d\n", x->name, x->surname, x->birthYear);
        x = x->next;
    }

    fclose(file);
    return 0;
}

/* citanje iz datoteke (dodaje elemente na kraj) */
int readFile(Person* head, const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ne mogu otvoriti datoteku za citanje.\n");
        return 1;
    }

    char name[50];
    char surname[50];
    int birthyear;

    Person* last = head;
    while (last->next != NULL) last = last->next;

    while (fscanf(file, "%49s %49s %d", name, surname, &birthyear) == 3) {
        Person* newP = (Person*)malloc(sizeof(Person));
        if (!newP) {
            printf("Greska pri alokaciji.\n");
            fclose(file);
            return 1;
        }
        strcpy(newP->name, name);
        strcpy(newP->surname, surname);
        newP->birthYear = birthyear;
        newP->next = NULL;

        last->next = newP;
        last = newP;
    }

    fclose(file);
    return 0;
}

/* brise sve elemente */
int deleteAll(Person* head) {
    Person* temp = head->next;
    Person* toDelete = NULL;

    while (temp != NULL) {
        toDelete = temp;
        temp = temp->next;
        free(toDelete);
    }

    head->next = NULL;
    return 0;
}
