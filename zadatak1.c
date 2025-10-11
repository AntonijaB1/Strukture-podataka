#define _CRT_SECURE_NO_WARNINGS
/*Napisati program koji prvo procita koliko redaka ima datoteka, tj.koliko ima studenata zapisanih u datoteci.
Nakon toga potebno je dinamicki alocirati prostor za niz struktura studenata(ime,prezime,bodovi)
i ucitati iz datoteke sve zapise. Na ekran ispisati ime, prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrzi ime i prezimw studenta, te broj bodova na kolokviju.
relativan_br_bodova =br_bodova/max_br_bodova*100*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_POINTS 50
#define MAX_LINE 100
#define FILE_NOT_OPENED (-1)
#define MEMORY_ERROR (-2)

//struktura koja opisuje jednog studenta
struct _Student;
typedef struct _Student* Position;
typedef struct _Student
{
	char name[50];
	char surname[50];
	int points;
	Position next;
}Student;

//deklaracija funkcija(prije maina!!)
int numberOfStudents(char* filename);
Student* loadStudents(char* filename, int number);
float* calculateRelativePoints(Student* students, int number, int max_points);
int printStudents(Student* students, float* rel_points, int number);

int main()
{
	//ime datoteke
	char* filename = "studenti.txt";

	int max_points = MAX_POINTS;

	//pozivam funkciju da prebrojim koliko studenata ima u datoteci
	int n = numberOfStudents(filename);
	if (n <= 0)
	{
		printf("Datoteka je prazna ili se ne moze otvoriti.\n");
		return FILE_NOT_OPENED;
	}

	//pozivam funkciju da ucitam studente iz datoteke
	Student* students = loadStudents(filename, n);
	if (students == NULL)
	{
		return MEMORY_ERROR;
	}

	//poziv funkcije za racunanje relativnih bodova
	float* rel_points = calculateRelativePoints(students, n, MAX_POINTS);
	if (rel_points == NULL)
	{
		free(students);
		return MEMORY_ERROR; //ako je greska pri alokaciji memorije
	}
	//ispis podataka
	printStudents(students, rel_points, n);

	//oslobadjanje memorije
	free(students);
	free(rel_points);

	return 0;
}

/*funkcija numberOfStudents otvara datoteku, cita je, prepoznaje \n kao kraj jednog reda,
i kada se prijedje u novi red brojac se poveca, broj redova je broj studenata*/
int numberOfStudents(char* filename)
{
	//otvori datoteku i citaj iz nje
	FILE* file = fopen(filename, "r");
	//provjeri moze li se datoteka otvoriti
	if (file == NULL)
	{
		printf("Datoteka se ne moze otvoriti.\n");
		return FILE_NOT_OPENED;
	}
	char row[MAX_LINE]; //deklaracija niza znakova
	int studentCount = 0; //postavljanje brojaca na 0

	/*citanje datoteke red po red dok ne dodje do EOF, fgets cita jedan red do \n,
	sto znaci kraj reda i kad do njega dodje brojac se povecava*/
	while (fgets(row, sizeof(row), file) != NULL)
	{
		studentCount++;
	}

	//zatvaranje datoteke
	fclose(file);

	return studentCount;
}

/*funkcija loadStudents otvara datoteku, alocira memoriju za broj redova odnosno studenatau datoteci,
zatim iscitava sve podatke o studentima iz datoteke te ju zatvara*/
Student* loadStudents(char* filename, int number)
{
	//otvori datoteku, ako se ne moze otvoriti vrati NULL
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Datoteka se ne moze otvoriti.\n");
		return NULL;
	}

	//provjera je li ispravan broj redaka u datoteci
	if (number <= 0)
	{
		printf("neispravan broj studenata.\n");
		fclose(file);
		return NULL;
	}

	//alokacija memorije te provjera je li alokacija uspjela
	Student* students = (Student*)malloc(number * sizeof(Student));
	if (students == NULL)
	{
		printf("Alokacija memorije nije uspjela.\n");
		fclose(file);
		return NULL;
	}
	//ide kroz retke datoteke i ucitava sve podatke o studentu,ako ima manje od 3 podatka javlja gresku
	for (int i = 0; i < number; i++)
	{
		int itemsRead= fscanf(file, "%49s %49s %d", students[i].name, students[i].surname, &students[i].points);
		if (itemsRead != 3)
		{
			printf("greska pri iscitavanju podataka %d reda\n", i + 1);
			fclose(file);
			return NULL;
		}
	}
	//zatvori datoteku
	fclose(file);

	//vrati pokazivac na niz struktura
	return students;
}

/*funkcija calculateRelativePoints iz datoteke uzima bodove svakog studenta i uzima kolki je max broj bodova. 
Alociram memoriju za relativne bodove(jer ne znam koliko cebodova od koliko studenata biti), u for petlji pomocu formule racunamo rel.brojbodova te ih vracamo*/
float* calculateRelativePoints(Student* students, int number, int max_points)
{
	
	//alokacija memorije
	float* rel_points = (float*)malloc(number * sizeof(float));
	if (rel_points == NULL)
	{
		printf("Alokacija memorije za relativne bodove nije uspjela.\n");
		return NULL;
	}

	//racunanje relativnih bodova
	for (int i = 0; i < number; i++)
	{
		rel_points[i] = ((float)students[i].points / max_points) * 100;
	}
	return rel_points;
}

//funkcija printStudents ispisuje sve podatke o studentu
int printStudents(Student* students, float* rel_points, int number)
{
	printf("Ime\tPrezime\tBodovi\tRelativni_bodovi\n");
	for (int i = 0; i < number; i++)
	{
		printf("%s\t%s\t%d\t%.2f\n", students[i].name, students[i].surname, students[i].points, rel_points[i]);
	}
	return 0;
}
