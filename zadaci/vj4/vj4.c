#define _CRT_SECURE_NO_WARNINGS
#define ERROR_MEMORY_ALLOCATION -1
#define ERROR_READING_FILE -2
#define ERROR_READING_POL -3
#define BUFFER_SIZE 1024
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct polinom* pol;

typedef struct polinom {
	int exp, coef;
	pol next;
}polinom;

pol createEl(int, int);

int scanFile(pol, pol);

int sumPol(pol, pol, pol);

int mulPol(pol, pol, pol);

int ispis(pol);

int delete(pol);

int sortedEntry(pol, int, int);

int main() {
	polinom head1, head2, head3;
	head1.next = NULL;
	head2.next = NULL;
	head3.next = NULL;

	scanFile(&head1, &head2);

	printf("Prvi polinom: ");
	ispis(head1.next);
	printf("Prvi polinom: ");
	ispis(head2.next);
	printf("Umnozak polinoma: ");
	mulPol(head1.next, head2.next, &head3);
	ispis(head3.next);
	delete(&head3);
	printf("Suma polinoma: ");
	sumPol(head1.next, head2.next, &head3);
	ispis(head3.next);

	return 0;
}

pol createEl(int coef, int exp) {
	pol Q = (pol)malloc(sizeof(polinom));
	if (Q == NULL) {
		printf("Greska u alociranju memorije.\n");
		return NULL;
	}
	Q->coef = coef;
	Q->exp = exp;
	Q->next = NULL;
	return Q;
}

int scanFile(pol First, pol Second) {
	FILE* fp = NULL;
	char* buffer;
	int coef, exp, byteSize, count, red = 0;
	fp = fopen("polinomi.txt", "r");
	if (fp == NULL) {
		printf("Greska u citanju filea.\n");
		return ERROR_READING_FILE;
	}

	buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	if (buffer == NULL) {
		printf("Greska u alociranju memorije.\n");
		return ERROR_MEMORY_ALLOCATION;
	}

	while (!feof(fp)) {
		red++;
		fgets(buffer, BUFFER_SIZE, fp);
		while (strlen(buffer) > 0) {
		count = sscanf(buffer, "%d %d %n", &coef, &exp, &byteSize);
			if (count != 2) {
				printf("Greska u citanju polinoma.\n");
				return ERROR_READING_POL;
			}
			if (red == 1) {
				sortedEntry(First, coef, exp);
			}
			else {
				sortedEntry(Second, coef, exp);
			}

			buffer = buffer + byteSize;
		}
	}
	
	return EXIT_SUCCESS;
}

int ispis(pol Q) {
	while (Q != NULL) {
		printf("%dx^%d", Q->coef, Q->exp);
		if (Q->next != NULL) {
			printf(" + ");
		}
		Q = Q->next;
	}
	printf("\n");
	return EXIT_SUCCESS;
}

int sortedEntry(pol Q, int c, int e) {
	pol temp = createEl(c, e);
	if (temp == NULL) {
		printf("Greska u alociranju memorije.\n");
		return ERROR_MEMORY_ALLOCATION;
	}

	while (Q->next != NULL && Q->next->exp > temp->exp) {
		Q = Q->next;
	}

	if (Q->next == NULL) {
		temp->next = Q->next;
		Q->next = temp;
	}

	else if (Q->next != NULL && Q->next->exp < temp->exp) {
		temp->next = Q->next;
		Q->next = temp;
	}

	else if (Q->next != NULL && Q->next->exp == temp->exp) {
		Q->next->coef += temp->coef;
	}
		
	return EXIT_SUCCESS;
}

int sumPol(pol p1, pol p2, pol p3) {
	while (p1 != NULL && p2 != NULL) {
		if (p1->exp == p2->exp) {
			sortedEntry(p3, p1->coef + p2->coef, p1->exp);
			p1 = p1->next;
			p2 = p2->next;
		}
		else if (p1->exp > p2->exp) {
			sortedEntry(p3, p1->coef, p1->exp);
			p1 = p1->next;
		}
		else if (p1->exp < p2->exp) {
			sortedEntry(p3, p2->coef, p2->exp);
			p2 = p2->next;
		}
		if (p2 == NULL) {
			while (p1 != NULL) {
				sortedEntry(p3, p1->coef, p1->exp);
				p1 = p1->next;
			}
		}
		else if (p1 == NULL) {
			while (p2 != NULL) {
				sortedEntry(p3, p2->coef, p2->exp);
				p2 = p2->next;
			}
		}
	}
	return EXIT_SUCCESS;
}

int mulPol(pol p1, pol p2, pol p3) {
	pol temp = p2;
	while (p1 != NULL) {
		while (temp != NULL) {
			sortedEntry(p3, p1->coef * temp->coef, p1->exp + temp->exp);
			temp = temp->next;
		}	
		p1 = p1->next;
		temp = p2;
	}
	return EXIT_SUCCESS;
}

int delete(pol p) {
	pol temp = p->next;
	pol deleteEl = NULL;
	while (temp != NULL) {
		deleteEl = temp;
		temp = temp->next;
		free(deleteEl);
	}
	p->next = NULL;

	return EXIT_SUCCESS;
}
