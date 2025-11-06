#define _CRT_SECURE_NO_WARNINGS
/*. Napisati program koji iz datoteke cita postfiks izraz i zatim koristenjem stoga racuna
rezultat. Stog je potrebno realizirati preko vezane liste.*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_BUFFER 100

//definicija struktue za cvor stoga
typedef struct _postfix* Position;
typedef struct _postfix {
	double number;
	Position next;
}postfix;

//deklaracija funkcija
int push(Position head, double number);
double pop(Position head);
int freeStack(Position head);
double computePostfix(const char* expr, Position head);
int readFile(const char* filename, Position head);
int isOperatorToken(const char* token, char* op);

int main()
{
	postfix head = { .number = 0.0, .next = NULL }; //glava stoga
	readFile("postfix.txt", &head);
	freeStack(&head);
	return 0;
}

//funkcija push stvara novi dvor i dodaje ga na vrh stoga
int push(Position head, double number) {
	Position q = (Position)malloc(sizeof(postfix));
	if (!q)
	{
		printf("Neuspjela alokacija memorije!");
		exit(EXIT_FAILURE);
	}

	q->number = number;
	q->next = head->next;
	head->next = q;
	return 0;
}

/*funkcija pop skida vrh stoga i vracanjegovu vrijednost,a ako je stog prazan vraca gresku*/
double pop(Position head) {
	if (head->next == NULL) {
		printf("Stog je prazan, postfix izraz nije ispravan");
		freeStack(head);
		exit(EXIT_FAILURE);
	}
	Position tmp = head->next;
	double val = tmp->number;
	head->next = tmp->next;
	free(tmp);
	return val;
}

int freeStack(Position head)
{
	Position cur = head->next;
	while (cur != NULL)
	{
		Position next = cur->next;
		free(cur);
		cur = next;
	}
	head->next = NULL;
	return 0;
}

//funkcija koja provjerava je li token operator +,-,*,/
int isOperatorToken(const char* token, char* op)
{
	if (token == NULL) return 0;
	if (strlen(token) == 1 && (strchr("+-*/", token[0]) != NULL))
	{
		*op = token[0];
		return 1;
	}
	return 0;
}

//funkcija racuna rezultat danog postfiksa koristeci stog
double computePostfix(const char* expr, Position head) {
    freeStack(head); // ocisti stog prije novog izraza

    char buffer[MAX_BUFFER];
    strncpy(buffer, expr, MAX_BUFFER);
    buffer[MAX_BUFFER - 1] = '\0'; 

    // ukloni trailing newline kako ne bi javilo da je token neispraan i nepoznat
    size_t ln = strlen(buffer);
    while (ln > 0 && (buffer[ln - 1] == '\n' || buffer[ln - 1] == '\r'))
        buffer[--ln] = '\0';
    if (ln == 0) return 0; // prazna linija -> vrati 0

    // tokenizacija
    char* token = strtok(buffer, " \t");
    while (token != NULL) {
        char* endptr = NULL;
        //konverzija u broj, pretvrara string u double
        double num = strtod(token, &endptr);
        if (endptr != token && *endptr == '\0') {
            push(head, num);
            token = strtok(NULL, " \t");
            continue;
        }

        // provjera operatora
        char op;
        if (isOperatorToken(token, &op)) {
            double b = pop(head);
            double a = pop(head);
            double res = 0.0;
            switch (op) {
            case '+': res = a + b; break;
            case '-': res = a - b; break;
            case '*': res = a * b; break;
            case '/':
                if (b == 0.0) {
                    printf("Greska: dijeljenje s nulom!\n");
                    freeStack(head);
                    exit(EXIT_FAILURE);
                }
                res = a / b;
                break;
            }
            push(head, res);
            token = strtok(NULL, " \t");
            continue;
        }

        // nepoznat token
        printf("Greska: Nepoznat ili neispravan token '%s'\n", token);
        freeStack(head);
        exit(EXIT_FAILURE);
    }

    // nakon obrade vraca samo jedan rezultat
    if (head->next == NULL) {
        fprintf(stderr, "Greska: postfix izraz nije ispravan (nema rezultata)\n");
        exit(EXIT_FAILURE);
    }
    double finalVal = pop(head);

    if (head->next != NULL) {
        fprintf(stderr, "Greska: postfix izraz nije ispravan (preostali elementi na stogu)\n");
        freeStack(head);
        exit(EXIT_FAILURE);
    }

    return finalVal;
}

//otvaranje i citanje datoteke
int readFile(const char* filename, Position head) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Datoteka se ne moze otvoriti\n");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER];
    while (fgets(buffer, MAX_BUFFER, fp)) {
        double result = computePostfix(buffer, head);
        printf("Rezultat: %.2f\n", result);
    }

    fclose(fp);
    return 0;
}