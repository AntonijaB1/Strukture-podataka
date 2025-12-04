#define _CRT_SECURE_NO_WARNINGS
/*6. Napisati program koji cita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini racun. Na pocetku svake datoteke je zapisan datum u kojem vremenu je
raèun izdat u formatu YYYY-MM-DD. Svaki sljedeci red u datoteci predstavlja artikl u formatu
naziv, kolicina, cijena. Potrebno je formirati vezanu listu racuna sortiranu po datumu. Svaki cvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omoguciti upit kojim ce korisnik saznati koliko je novaca sveukupno potroseno na specificni
artikl u odredjenom vremenskom razdoblju i u kojoj je kolicini isti kupljen.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DATE_LENGTH 30
#define BUFFER_SIZE 1024

typedef struct _artikl* ArtiklHead;
typedef struct _artikl {
    char naziv[20];
    int kolicina;
    double cijena;
    ArtiklHead next;
} Artikl;

typedef struct _racun* RacunHead;
typedef struct _racun {
    char datum[DATE_LENGTH];
    ArtiklHead anext; //pokaziva na listu artikla u racunu
    RacunHead rnext; //pokaziva na sljedeci racun u listi
} Racun;


int readRacuni(char* filename, RacunHead head);
int insertArtiklSorted(ArtiklHead* head, ArtiklHead novi);
int insertRacunSorted(RacunHead* head, RacunHead novi);
int compareDates(const char* date1, const char* date2);
int freeAll(RacunHead head);
int searchByDate(RacunHead head, char* naziv, const char* start_date, const char* end_date);
int printArtikl(RacunHead head);

int main() {

    //ovo mi je cvor za listu racuna u datoteci
    Racun racuniHead = { .datum = "", .anext = NULL, .rnext = NULL };

    if (readRacuni("racuni.txt", &racuniHead) != 0) {
        printf("Greska pri citanju racuna!\n");
        return EXIT_FAILURE;
    }

    char naziv[20];
    printf("Unesite naziv artikla:\n\n");
    printArtikl(&racuniHead);

    printf("\n\n: ");
    scanf("%s", naziv);

    char start_date[20];
    char end_date[20];

    printf("\nUnesite vremensko razdoblje (yyyy-mm-dd):\n");
    printf("Pocetni datum: ");
    scanf("%s", start_date);

    printf("Zavrsni datum: ");
    scanf("%s", end_date);

    searchByDate(&racuniHead, naziv, start_date, end_date);

    freeAll(racuniHead.rnext);

    return EXIT_SUCCESS;;
}


int readRacuni(char* filename, RacunHead head) {

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Nemoguce otvoriti datoteku %s\n", filename);
        return ;
    }

    char buffer[BUFFER_SIZE];

    while (fscanf(fp, "%s", buffer) == 1) {

        FILE* racun = fopen(buffer, "r");
        if (!racun) {
            printf("Nemoguce otvoriti racun: %s\n", buffer);
            continue;
        }

        RacunHead novi_racun = malloc(sizeof(Racun));
        if (!novi_racun) {
            printf("Greska pri alokaciji racuna\n");
            fclose(racun);
            continue;
        }
        memset(novi_racun, 0, sizeof(Racun));

        if (fgets(buffer, BUFFER_SIZE, racun)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            strncpy(novi_racun->datum, buffer, DATE_LENGTH);
        }

        while (fgets(buffer, BUFFER_SIZE, racun)) {

            char naziv[30];
            int kolicina = 0;
            double cijena = 0.0;

            if (sscanf(buffer, "%s %d %lf", naziv, &kolicina, &cijena) == 3) {

                ArtiklHead novi_artikl = malloc(sizeof(Artikl));
                if (!novi_artikl) {
                    printf("Greska pri alokaciji artikla\n");
                    continue;
                }

                memset(novi_artikl, 0, sizeof(Artikl));

                strncpy(novi_artikl->naziv, naziv, sizeof(novi_artikl->naziv));
                novi_artikl->kolicina = kolicina;
                novi_artikl->cijena = cijena;

                insertArtiklSorted(&novi_racun->anext, novi_artikl);
            }
        }

        fclose(racun);

        insertRacunSorted(&head->rnext, novi_racun);
    }

    fclose(fp);
    return EXIT_SUCCESS;;
}


int insertArtiklSorted(ArtiklHead* head, ArtiklHead novi) {

    ArtiklHead* curr = head;

    while (*curr && strcmp((*curr)->naziv, novi->naziv) < 0) {
        curr = &(*curr)->next;
    }

    if (*curr && strcmp((*curr)->naziv, novi->naziv) == 0) {
        (*curr)->kolicina += novi->kolicina;
        free(novi);
    }
    else {
        novi->next = *curr;
        *curr = novi;
    }

    return 0;
}


int insertRacunSorted(RacunHead* head, RacunHead novi) {

    RacunHead* curr = head;

    while (*curr && compareDates((*curr)->datum, novi->datum) < 0) {
        curr = &(*curr)->rnext;
    }

    novi->rnext = *curr;
    *curr = novi;

    return 0;
}


int compareDates(const char* d1, const char* d2) {

    int y1, m1, d_1;
    int y2, m2, d_2;

    sscanf(d1, "%d-%d-%d", &y1, &m1, &d_1);
    sscanf(d2, "%d-%d-%d", &y2, &m2, &d_2);

    if (y1 != y2) return y1 - y2;
    if (m1 != m2) return m1 - m2;
    return d_1 - d_2;
}


int freeAll(RacunHead head) {

    while (head) {
        RacunHead rdel = head;
        head = head->rnext;

        ArtiklHead a = rdel->anext;
        while (a) {
            ArtiklHead adel = a;
            a = a->next;
            free(adel);
        }

        free(rdel);
    }

    return EXIT_SUCCESS;;
}


int searchByDate(RacunHead head, char* naziv, const char* start_date, const char* end_date) {

    RacunHead curr = head->rnext;
    int total_quantity = 0;
    double total_cost = 0.0;

    while (curr) {
        if (compareDates(curr->datum, start_date) >= 0 &&
            compareDates(curr->datum, end_date) <= 0) {

            ArtiklHead a = curr->anext;

            while (a) {
                if (strcmp(a->naziv, naziv) == 0) {
                    total_quantity += a->kolicina;
                    total_cost += a->kolicina * a->cijena;
                }
                a = a->next;
            }
        }

        curr = curr->rnext;
    }

    printf("\nArtikl: %s\n", naziv);
    printf("Ukupna kolicina: %d\n", total_quantity);
    printf("Ukupan trosak: %.2lf\n", total_cost);

    return EXIT_SUCCESS;;
}


int printArtikl(RacunHead head) {

    ArtiklHead svi = NULL;

    RacunHead racun = head->rnext;

    while (racun) {
        ArtiklHead a = racun->anext;

        while (a) {
            bool found = false;
            ArtiklHead t = svi;

            while (t) {
                if (strcmp(t->naziv, a->naziv) == 0) {
                    found = true;
                    break;
                }
                t = t->next;
            }

            if (!found) {
                ArtiklHead novi = malloc(sizeof(Artikl));
                if (!novi) {
                    printf("Greska pri alokaciji artikla\n");
                    return -1;
                }
                strcpy(novi->naziv, a->naziv);
                novi->cijena = a->cijena;
                novi->next = svi;
                svi = novi;
            }

            a = a->next;
        }

        racun = racun->rnext;
    }

    printf("Dostupni artikli:\n");
    ArtiklHead temp = svi;
    while (temp) {
        printf("%s\t%.2f\n", temp->naziv, temp->cijena);
        temp = temp->next;
    }

    while (svi) {
        ArtiklHead del = svi;
        svi = svi->next;
        free(del);
    }

    return EXIT_SUCCESS;;
}
