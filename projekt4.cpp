#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996 6387)

#define VERSION 4

#if (VERSION == 1)
#include "stos_lista.h"
#endif
#if (VERSION == 2)
#include "stos_tablica.h"
#endif
#if (VERSION == 3)
#include "kolejka_lista.h"
#endif
#if (VERSION == 4)
#include "kolejka_tablica.h"
#endif

void print_menu();

int main()
{	
	do {
		print_menu();

		switch (wybor) {
		case dodaj:
			dodaj_element();
			break;

		case pobierz:
			pobierz_element();
			break;

		case wyszukaj:
			wyszukaj_element();
			break;

		case wypisz:
			wypisz_elementy();
			break;

		case licz:
			policz_elementy();
			break;

		case usun:
			usun_elementy();
			break;

		case odczytaj:
			odczytaj_plik();
			break;

		case zapisz:
			zapisz_plik();
			break;

		case zakoncz:
			zakoncz_program();
			break;

		default:
			printf("Podano opcje z poza MENU");
		}
	} while (wybor != 9);

	return 0;
}

void print_menu() {
	printf("====================================\n\t\tMENU\n====================================\n");
	printf("[1] - Dodaj element\n");
	printf("[2] - Pobierz element\n");
	printf("[3] - Wyszukaj element\n");
	printf("[4] - Wypisz wszystkie elementy\n");
	printf("[5] - Policz elementy\n");
	printf("[6] - Usun stos\n");
	printf("[7] - Odczytaj z pliku\n");
	printf("[8] - Zapisz do pliku\n");
	printf("[9] - Zakoncz\n");
	do {
		fgets(bufor, MAX_BUFOR, stdin);
		wybor = atoi(bufor);
		if (wybor == 0) printf("Podano nieprawidlowa opcje\n");
	} while (!wybor);
	system("cls");
	printf("====================================\n");
}
