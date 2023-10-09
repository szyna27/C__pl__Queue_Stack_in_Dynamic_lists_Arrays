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
STUDENT tab[MAX_SIZE];
char bufor[MAX_BUFOR];
int size_now = 0;

void wypisz_elementy() {
	if (size_now == 0) {
		printf("Stos pusty\n");
		return;
	}

	for (int i = 0; i < size_now; i++) {
		printf("%12s| %12s| %4d|\n", tab[i].imie, tab[i].nazwisko, tab[i].rok);
	}

	printf("\nWypisano wszystkie elementy stosu\n");
}

void dodaj_element() {
	if (size_now >= MAX_SIZE) {
		printf("Przekroczono maksymalny rozmiar stosu\n");
		return;
	}

	unsigned len;

	printf("Dodawanie nowego elementu do stosu\n");

	printf("Podaj imie: ");
	fgets(bufor, MAX_BUFOR, stdin);
	len = (unsigned)strlen(bufor);
	bufor[len - 1] = '\0';
	tab[size_now].imie = (char*)malloc(len);
	strcpy(tab[size_now].imie, bufor);

	printf("Podaj nazwisko: ");
	fgets(bufor, MAX_BUFOR, stdin);
	len = (unsigned)strlen(bufor);
	bufor[len - 1] = '\0';
	tab[size_now].nazwisko = (char*)malloc(len);
	strcpy(tab[size_now].nazwisko, bufor);

	printf("Podaj rok: ");
	do {
		fgets(bufor, MAX_BUFOR, stdin);
		tab[size_now].rok = atoi(bufor);
		if (tab[size_now].rok == 0) printf("Podano nieprawidlowy rok urodzenia sprobuj ponownie\n");
	} while (!tab[size_now].rok);

	size_now++;
	printf("Poprawnie dodano element\n");
}

void pobierz_element() {

	if (size_now) {
		free(tab[size_now].imie);
		free(tab[size_now].nazwisko);
		size_now--;

		printf("Poprawnie pobrano element\n");
	}
	else {
		printf("Brak elementow na stosie\n");
	}
}

void wyszukaj_element() {
	typedef enum {
		imie = 1,
		nazwisko,
		rok
	} WYSZUKAJ_PO;

	WYSZUKAJ_PO element;
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
		for (int i = 0; i < size_now; i++) {
			if (!strcmp(bufor, tab[i].imie)) {
				found = 1;
				printf("%12s| %12s| %4d|\n", tab[i].imie, tab[i].nazwisko, tab[i].rok);
			}
		}
		break;
	case nazwisko:
		printf("Podaj nazwisko: ");
		fgets(bufor, MAX_BUFOR, stdin);
		r = (unsigned)strlen(bufor);
		bufor[r - 1] = '\0';
		printf("Wyszukane elementy:\n");
		for (int i = 0; i < size_now; i++) {
			if (!strcmp(bufor, tab[i].nazwisko)) {
				found = 1;
				printf("%12s| %12s| %4d|\n", tab[i].imie, tab[i].nazwisko, tab[i].rok);
			}
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
		for (int i = 0; i < size_now; i++) {
			if (r == tab[i].rok) {
				found = 1;
				printf("%12s| %12s| %4d|\n", tab[i].imie, tab[i].nazwisko, tab[i].rok);
			}
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
	printf("Liczba elementow jest rowna %d\n", size_now);
}

void usun_elementy() {
	int k = 0;
	for (int i = 0; i < size_now; i++) {
		k++;
		free(tab[i].imie);
		free(tab[i].nazwisko);
	}
	size_now = 0;
	printf("Usunieto %d elementow\n", k);
}

void zakoncz_program() {
	for (int i = 0; i < size_now; i++) {
		free(tab[i].imie);
		free(tab[i].nazwisko);
	}
}

void odczytaj_plik() {
	if (!(f = fopen("dane.bin", "rb"))) {
		printf("Nie udalo sie otworzyc pliku do odczytu\n");
		return;
	}

	int n, k = 0;


	while (fread(&n, sizeof(int), 1, f) && size_now < MAX_SIZE) {

		tab[size_now].imie = (char*)malloc(sizeof(char) * n);
		fread(tab[size_now].imie, sizeof(char), n, f);

		fread(&n, sizeof(int), 1, f);

		tab[size_now].nazwisko = (char*)malloc(sizeof(char) * n);
		fread(tab[size_now].nazwisko, sizeof(char), n, f);

		fread(&tab[size_now].rok, sizeof(int), 1, f);

		size_now++;
		k++;
	}

	fclose(f);
	f = NULL;

	printf("Odczytano %d elementow z pliku\n", k);
	if (size_now >= MAX_SIZE) {
		printf("Przerwano w wyniku osiagniecia maksymalnego rozmiaru stosu\n");
	}
}

void zapisz_plik() {
	if (!(f = fopen("dane.bin", "wb"))) {
		printf("Nie udalo sie otworzyc pliku do zapisu\n");
		return;
	}

	int n, k = 0;

	for(int i = size_now - 1; i >= 0; i--){

		n = strlen(tab[i].imie) + 1;
		fwrite(&n, sizeof(int), 1, f);
		fwrite(tab[i].imie, sizeof(char), n, f);
		free(tab[i].imie);

		n = strlen(tab[i].nazwisko) + 1;
		fwrite(&n, sizeof(int), 1, f);
		fwrite(tab[i].nazwisko, sizeof(char), n, f);
		free(tab[i].nazwisko);

		fwrite(&tab[i].rok, sizeof(int), 1, f);
		k++;
	}
	
	size_now = 0;

	fclose(f);
	f = NULL;

	printf("Zapisano %d elementow do pliku\n", k);
}