#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996 6387)
#define MAX_BUFOR 64
#define MAX_SIZE 10

typedef struct stud {
	char* imie;
	char* nazwisko;
	int rok;
	struct stud* n;
} STUDENT;

typedef enum {
	dodaj = 1,
	pobierz,
	wyszukaj,
	wypisz,
	licz,
	usun,
	odczytaj,
	zapisz,
	zakoncz
} MENU;

MENU wybor;
FILE* f;
STUDENT* H = NULL, * T = NULL, * POBRANY = NULL;
char bufor[MAX_BUFOR];
int size_now = 0;

void wypisz_elementy() {
	if (H == NULL) {
		printf("Kolejka pusta\n");
		return;
	}

	for (STUDENT* w = H; w != (STUDENT*)NULL; w = w->n) {
		printf("%12s| %12s| %4d|\n", w->imie, w->nazwisko, w->rok);
	}

	printf("\nWypisano wszystkie elementy kolejki\n");
}

void dodaj_element() {
	if (size_now >= MAX_SIZE) {
		printf("Przekroczono maksymalny rozmiar kolejki\n");
		return;
	}

	unsigned len;
	STUDENT* B = NULL;

	printf("Dodawanie nowego elementu do kolejki\n");
	B = (STUDENT*)malloc(sizeof(STUDENT));

	printf("Podaj imie: ");
	fgets(bufor, MAX_BUFOR, stdin);
	len = (unsigned)strlen(bufor);
	bufor[len - 1] = '\0';
	B->imie = (char*)malloc(len);
	strcpy(B->imie, bufor);

	printf("Podaj nazwisko: ");
	fgets(bufor, MAX_BUFOR, stdin);
	len = (unsigned)strlen(bufor);
	bufor[len - 1] = '\0';
	B->nazwisko = (char*)malloc(len);
	strcpy(B->nazwisko, bufor);

	printf("Podaj rok: ");
	do {
		fgets(bufor, MAX_BUFOR, stdin);
		B->rok = atoi(bufor);
		if (B->rok == 0) printf("Podano nieprawidlowy rok urodzenia sprobuj ponownie\n");
	} while (!B->rok);

	if (T) {
		T->n = B;
		T = B;
		T->n = (STUDENT*)NULL;
	}
	else {
		H = T = B;
		B->n = (STUDENT*)NULL;
	}

	size_now++;
	printf("Poprawnie dodano element\n");
}

void pobierz_element() {
	STUDENT* w = H;

	if (w) {
		H = H->n;
		free(w->imie);
		free(w->nazwisko);
		free(w);
		size_now--;
	}
	else {
		printf("Brak elementow w kolejce\n");
	}
}

void wyszukaj_element() {
	typedef enum {
		imie = 1,
		nazwisko,
		rok
	} WYSZUKAJ_PO;

	WYSZUKAJ_PO element;
	STUDENT* w = H;
	int found = 0, r;

	printf("Po jakim elemencie chcesz wyszukiwac?\n");
	printf("[1] - imie\n");
	printf("[2] - nazwisko\n");
	printf("[3] - rok urodzenia\n");
	do {
		fgets(bufor, MAX_BUFOR, stdin);
		element = atoi(bufor);
		if (element == 0) printf("Podano nieprawidlowy element sprobuj ponownie\n");
	} while (!element);

	switch (element) {
	case imie:
		printf("Podaj imie: ");
		fgets(bufor, MAX_BUFOR, stdin);
		r = (unsigned)strlen(bufor);
		bufor[r - 1] = '\0';
		printf("Wyszukane elementy:\n");
		while (w) {
			if (!strcmp(bufor, w->imie)) {
				found = 1;
				printf("%12s| %12s| %4d|\n", w->imie, w->nazwisko, w->rok);
			}
			w = w->n;
		}
		break;
	case nazwisko:
		printf("Podaj nazwisko: ");
		fgets(bufor, MAX_BUFOR, stdin);
		r = (unsigned)strlen(bufor);
		bufor[r - 1] = '\0';
		printf("Wyszukane elementy:\n");
		while (w) {
			if (!strcmp(bufor, w->nazwisko)) {
				found = 1;
				printf("%12s| %12s| %4d|\n", w->imie, w->nazwisko, w->rok);
			}
			w = w->n;
		}
		break;
	case rok:
		printf("Podaj rok urodzenia: ");
		do {
			fgets(bufor, MAX_BUFOR, stdin);
			r = atoi(bufor);
			if (r == 0) printf("Podano nieprawidlowy rok urodzenia sprobuj ponownie\n");
		} while (!r);
		printf("Wyszukane elementy:\n");
		while (w) {
			if (r == w->rok) {
				found = 1;
				printf("%12s| %12s| %4d|\n", w->imie, w->nazwisko, w->rok);
			}
			w = w->n;
		}
		break;
	default:
		printf("Taki element nie istnieje\n");
		found = 1;
	}

	if (!found)
		printf("Nie znaleziono zadnego elementu o takim kluczu\n");

}

void policz_elementy() {
	STUDENT* w = H;
	int k = 0;

	while (w) {
		w = w->n;
		k++;
	}

	printf("Liczba elementow jest rowna %d\n", k);
}

void usun_elementy() {
	STUDENT* w;
	int k = 0;
	while (H) {
		k++;
		w = H;
		H = H->n;
		free(w->imie);
		free(w->nazwisko);
		free(w);
	}
	size_now = 0;
	printf("Usunieto %d elementow\n", k);
}

void zakoncz_program() {
	STUDENT* w;
	while (H) {
		w = H;
		H = H->n;
		free(w->imie);
		free(w->nazwisko);
		free(w);
	}
}

void odczytaj_plik() {
	if (!(f = fopen("dane.bin", "rb"))) {
		printf("Nie udalo sie otworzyc pliku do odczytu\n");
		return;
	}

	STUDENT* w;
	int n, k = 0;

	while (fread(&n, sizeof(int), 1, f) && size_now < MAX_SIZE) {

		w = (STUDENT*)malloc(sizeof(STUDENT));

		w->imie = (char*)malloc(sizeof(char) * n);
		fread(w->imie, sizeof(char), n, f);

		fread(&n, sizeof(int), 1, f);

		w->nazwisko = (char*)malloc(sizeof(char) * n);
		fread(w->nazwisko, sizeof(char), n, f);

		fread(&w->rok, sizeof(int), 1, f);

		if (T) {
			T->n = w;
			T = w;
			T->n = (STUDENT*)NULL;
		}
		else {
			H = T = w;
			w->n = (STUDENT*)NULL;
		}
		size_now++;
		k++;
	}

	fclose(f);
	f = NULL;

	printf("Odczytano %d elementow z pliku\n", k);
	if (size_now >= MAX_SIZE) {
		printf("Przerwano w wyniku osiagniecia maksymalnego rozmiaru kolejki\n");
	}
}

void zapisz_plik() {
	if (!(f = fopen("dane.bin", "wb"))) {
		printf("Nie udalo sie otworzyc pliku do zapisu\n");
		return;
	}

	STUDENT* w = H;
	int n, k = 0;

	while (w) {

		n = strlen(w->imie) + 1;
		fwrite(&n, sizeof(int), 1, f);
		fwrite(w->imie, sizeof(char), n, f);
		free(w->imie);

		n = strlen(w->nazwisko) + 1;
		fwrite(&n, sizeof(int), 1, f);
		fwrite(w->nazwisko, sizeof(char), n, f);
		free(w->nazwisko);

		fwrite(&w->rok, sizeof(int), 1, f);

		H = w->n;
		free(w);
		w = H;

		k++;
	}
	T = (STUDENT*)NULL;
	size_now = 0;

	fclose(f);
	f = NULL;

	printf("Zapisano %d elementow do pliku\n", k);
}