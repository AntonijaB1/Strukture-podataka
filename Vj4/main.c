#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _poly* position;
typedef struct _poly {
    int coeff;
    int exp;
    position next;
} poly;

/* prototipi funkcija */
position createElement(int coeff, int exp);
int insertSorted(position head, position new_el);
int readFile(const char* filename, position head1, position head2);
position addPolynomials(position head1, position head2);
position multiplyPolynomials(position head1, position head2);
int printPolynomial(position head);
int freePoly(position head);

int main(void) {
    poly head1 = { .coeff = 0, .exp = 0, .next = NULL };
    poly head2 = { .coeff = 0, .exp = 0, .next = NULL };

    if (readFile("polinomi.txt", &head1, &head2) != EXIT_SUCCESS) {
        fprintf(stderr, "Greska pri citanju datoteke.\n");
        return EXIT_FAILURE;
    }

    printf("Polinom 1: ");
    printPolynomial(head1.next);

    printf("Polinom 2: ");
    printPolynomial(head2.next);

    position sum = addPolynomials(head1.next, head2.next);
    printf("Zbroj: ");
    printPolynomial(sum);

    position product = multiplyPolynomials(head1.next, head2.next);
    printf("Umnozak: ");
    printPolynomial(product);

    freePoly(head1.next);
    freePoly(head2.next);
    freePoly(sum);
    freePoly(product);

    return EXIT_SUCCESS;
}

/* funkcija stvara i vraca novi element liste polinoma */
position createElement(int coeff, int exp) {
    position new_el = (position)malloc(sizeof(poly));
    if (new_el == NULL) {
        fprintf(stderr, "Alokacija memorije nije uspjela\n");
        return NULL;
    }
    new_el->coeff = coeff;
    new_el->exp = exp;
    new_el->next = NULL;
    return new_el;
}

/* insertSorted listu sortira silazno po eksponentima; ako postoji element s istim
   eksponentom zbraja koeficijente; element s koeficijentom 0 uklanja */
int insertSorted(position head, position new_el) {
    if (!head || !new_el) return EXIT_FAILURE;

    position temp = head;
    /* traži mjesto (silazno po exp) */
    while (temp->next != NULL && temp->next->exp > new_el->exp) {
        temp = temp->next;
    }

    if (temp->next != NULL && temp->next->exp == new_el->exp) {
        temp->next->coeff += new_el->coeff;
        free(new_el);
        if (temp->next->coeff == 0) {
            position toDelete = temp->next;
            temp->next = temp->next->next;
            free(toDelete);
        }
    }
    else {
        new_el->next = temp->next;
        temp->next = new_el;
    }
    return EXIT_SUCCESS;
}

/* funkcija cita polinome iz datoteke */
int readFile(const char* filename, position head1, position head2) {
    if (!filename || !head1 || !head2) return EXIT_FAILURE;

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Datoteka se ne moze otvoriti: %s\n", filename);
        return EXIT_FAILURE;
    }

    int n1 = 0;
    if (fscanf(file, "%d", &n1) != 1) {
        fprintf(stderr, "Ne mogu procitati broj clanova za prvi polinom.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n1; i++) {
        int coeff, exp;
        if (fscanf(file, "%d %d", &coeff, &exp) != 2) {
            fprintf(stderr, "Pogresan format pri citanju prvog polinoma (ocekivano %d redova).\n", n1);
            fclose(file);
            return EXIT_FAILURE;
        }
        position new_el = createElement(coeff, exp);
        if (new_el == NULL) {
            fclose(file);
            return EXIT_FAILURE;
        }
        insertSorted(head1, new_el);
    }

    int n2 = 0;
    if (fscanf(file, "%d", &n2) != 1) {
        fprintf(stderr, "Ne mogu procitati broj clanova za drugi polinom.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n2; i++) {
        int coeff, exp;
        if (fscanf(file, "%d %d", &coeff, &exp) != 2) {
            fprintf(stderr, "Pogresan format pri citanju drugog polinoma (ocekivano %d redova).\n", n2);
            fclose(file);
            return EXIT_FAILURE;
        }
        position new_el = createElement(coeff, exp);
        if (new_el == NULL) {
            fclose(file);
            return EXIT_FAILURE;
        }
        insertSorted(head2, new_el);
    }

    fclose(file);
    return EXIT_SUCCESS;
}

/* zbraja 2 polinoma predstavljena povezanim listama */
position addPolynomials(position head1, position head2) {
    poly resultHead = { .coeff = 0, .exp = 0, .next = NULL };
    position t = head1;
    while (t) {
        position el = createElement(t->coeff, t->exp);
        if (el == NULL) { freePoly(resultHead.next); return NULL; }
        insertSorted(&resultHead, el);
        t = t->next;
    }
    t = head2;
    while (t) {
        position el = createElement(t->coeff, t->exp);
        if (el == NULL) { freePoly(resultHead.next); return NULL; }
        insertSorted(&resultHead, el);
        t = t->next;
    }
    return resultHead.next;
}

/* svaki clan jednog polinoma se mnozi sa svakim clanom drugog */
position multiplyPolynomials(position head1, position head2) {
    poly resultHead = { .coeff = 0, .exp = 0, .next = NULL };
    if (!head1 || !head2) return NULL;

    for (position t1 = head1; t1 != NULL; t1 = t1->next) {
        for (position t2 = head2; t2 != NULL; t2 = t2->next) {
            int newCoeff = t1->coeff * t2->coeff;
            int newExp = t1->exp + t2->exp;
            position el = createElement(newCoeff, newExp);
            if (el == NULL) { freePoly(resultHead.next); return NULL; }
            insertSorted(&resultHead, el);
        }
    }
    return resultHead.next;
}

/* ispis polinoma s lijepim formatiranjem */
int printPolynomial(position head) {
    if (!head) {
        printf("0\n");
        return EXIT_SUCCESS;
    }

    position temp = head;
    int first = 1;
    while (temp) {
        int c = temp->coeff;
        int e = temp->exp;
        if (!first) {
            if (c >= 0) printf(" + ");
            else printf(" - ");
        }
        else {
            if (c < 0) printf("-");
        }

        int abs_c = (c < 0) ? -c : c;

        /* formatiranje koeficijenta i eksponenta */
        if (e == 0) {
            printf("%d", abs_c);
        }
        else if (e == 1) {
            if (abs_c == 1) printf("x");
            else printf("%d*x", abs_c);
        }
        else {
            if (abs_c == 1) printf("x^%d", e);
            else printf("%d*x^%d", abs_c, e);
        }

        first = 0;
        temp = temp->next;
    }
    printf("\n");
    return EXIT_SUCCESS;
}

/* oslobadjanje memorije */
int freePoly(position head) {
    while (head != NULL) {
        position temp = head;
        head = head->next;
        free(temp);
    }
    return EXIT_SUCCESS;
}
