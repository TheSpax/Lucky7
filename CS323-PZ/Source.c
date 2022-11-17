#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int iznos;
	int bet;
} Igra;

void enterCredit(Igra *igra) {
	printf("Dobrodosli na Lucky 7\n\n");
	while (1) {
		printf("Unesite iznos za igru. Minimalan iznos je 100 dinara!\n");
		scanf("%d", &igra->iznos);
		if (igra->iznos < 100){
			printf("Niste uneli dovoljno novca za igru!\n");
		}
		else {
			break;
		}
	}
}

void checkBet(Igra *igra) {
	while (1) {
		printf("\nBet 10\n");
		printf("Bet 20\n");
		printf("Bet 50\n");
		printf("Bet 100\n");
		printf("Odaberite bet za igru: ");
		scanf("%d", &igra->bet);
		if (igra->bet == 10 || igra->bet == 20 || igra->bet == 50 || igra->bet == 100){
			break;
		}
		else {
			printf("Niste uneli validan bet\n");
		}
	}
}

void delay()
{
	clock_t start_time = clock();
	while (clock() < start_time + 1000);
}

void gamble(int *osvojeno) {
	system("cls");
	char s[20];
	char s1[][7] = { "crveno", "crno" };
	srand(time(NULL));
	int i = rand() % 2;
	printf("Kredit za kockanje: %d\n", *osvojeno);
	printf("Unesite naziv boje: crveno/crno.\n");
	scanf("%s", s);
	s[strlen(s)] = '\0';
	if (strcmp("crveno", s) != 0 && strcmp("crno", s) != 0){
		printf("Niste uneli validan naziv boje!\n");
	}
	else {
		printf("Izvucena boja je: %s\n", s1[i]);
		if (strcmp(s1[i], s) == 0){
			printf("Pogodak!\n");
			*osvojeno *= 2;
			printf("Osvojeni kredit je sada: %d\n", *osvojeno);
		}
		else {
			*osvojeno = 0;
			printf("Niste pogodili!\n\n");
		}
	}
}

void printGame(Igra *igra, char *simboli, int *odabraniSimboli) {
	system("cls");
	printf("Dobrodosli na Lucky 7\n\n");
	printf("3 x 7 -> bet x 20\n");
	printf("3 x $ -> bet x 10\n");
	printf("3 x ? -> bet x 5\n");
	printf("3 x ~ -> bet x 2\n");
	printf("3 x & -> bet x 1\n\n");
	printf("Vas kredit je: %d\n", igra->iznos);
	printf("Odabrani bet je: %d\n", igra->bet);
	srand(time(NULL));
	for (int i = 0; i < 9; i++) {
		odabraniSimboli[i] = rand() % 5;
	}
	printf("|   %c   |   %c   |   %c   |\n", simboli[odabraniSimboli[0]], simboli[odabraniSimboli[1]], simboli[odabraniSimboli[2]]);
	delay();
	printf("|   %c   |   %c   |   %c   |\n", simboli[odabraniSimboli[3]], simboli[odabraniSimboli[4]], simboli[odabraniSimboli[5]]);
	delay();
	printf("|   %c   |   %c   |   %c   |\n\n", simboli[odabraniSimboli[6]], simboli[odabraniSimboli[7]], simboli[odabraniSimboli[8]]);
}

void updateSum(char x, int *osvojeno, Igra *igra) {
	switch (x) {
	case '7':
		*osvojeno += igra->bet * 20;
		break;
	case '$':
		*osvojeno += igra->bet * 10;
		break;
	case '?':
		*osvojeno += igra->bet * 5;
		break;
	case '~':
		*osvojeno += igra->bet * 2;
		break;
	case '&':
		*osvojeno += igra->bet * 1;
	default:
		break;
	}
}

void checkResult(int *odabraniSimboli, char *simboli, Igra *igra) {
	igra->iznos -= igra->bet;
	int osvojeno = 0, i;
	char a = '.', b = '.', c = '.';
	if (odabraniSimboli[0] == odabraniSimboli[1] && odabraniSimboli[1] == odabraniSimboli[2]) {
		a = simboli[odabraniSimboli[0]];
	}
	if (odabraniSimboli[3] == odabraniSimboli[4] && odabraniSimboli[4] == odabraniSimboli[5]){
		b = simboli[odabraniSimboli[3]];
	}
	if(odabraniSimboli[6] == odabraniSimboli[7] && odabraniSimboli[7] == odabraniSimboli[8]){
		c = simboli[odabraniSimboli[6]];
	}
	if (a == '.' && b == '.' && c == '.'){
		printf("Nema pogodaka\n");
	}
	else if (a == b && a == c) {
		osvojeno += igra->bet * 30;
	}
	else {
		updateSum(a, &osvojeno, igra);
		updateSum(b, &osvojeno, igra);
		updateSum(c, &osvojeno, igra);
	}

	printf("Osvojeno: %d\n", osvojeno);
	if (osvojeno > 0) {
		printf("Da li zelite da kockate osvojenu sumu?\n1. Da \n2. Ne\n");
		scanf("%d", &i);
		while (i == 1) {
			gamble(&osvojeno);
			if (osvojeno == 0) {
				break;
			}
			printf("Nastavi sa kockanjem?\n1. Da\n2. Ne\n");
			scanf("%d", &i);
		}
	}
	igra->iznos += osvojeno;
	printf("Vas ukupni kredit je sada: %d\n\n", igra->iznos);
	if (osvojeno == igra->bet * 30){
		printf("Pogodili ste dzekpot!\n");
	}
	
}

void payUp(FILE *f, Igra *igra) {
	f = fopen("isplata.txt", "w");
	fprintf(f, "Osvojena suma je: %d dinara", igra->iznos);
	fclose(f);
}

int main() {

	Igra igra = {0, 0};
	FILE* f = NULL;
	char *simboli = (char*)calloc(5, sizeof(char));
	simboli[0] = '7';
	simboli[1] = '$';
	simboli[2] = '?';
	simboli[3] = '~';
	simboli[4] = '&';
	int *odabraniSimboli = (int*)calloc(9, sizeof(int)), i = 1;

	jump:
	enterCredit(&igra);
	checkBet(&igra);

	while (i == 1) {
		printGame(&igra, simboli, odabraniSimboli);
		checkResult(odabraniSimboli, simboli, &igra);
		if (igra.iznos <= 0)
		{
			printf("Nemate vise kredita.\nPokreni igru ponovo? \n1. Pokreni igru \n2. Zavrsi igru\n");
			scanf("%d", &i);
			if (i == 2) {
				break;
			}else {
				system("cls");
				goto jump;
			}
		}
		printf("Nastavi sa igrom? \n1. Nastavi \n2. Zavrsi igru\n");
		scanf("%d", &i);
	}

	payUp(f, &igra);

	free(simboli);
	free(odabraniSimboli);
	return 0;
}