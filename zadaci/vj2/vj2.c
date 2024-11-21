#define _CRT_SECURE_NO_WARNINGS
#define SIZE_BUFFER 1024
#define ERROR_MEMORY_ALLOCATION (-1)
#define ERROR_PERSON_NOT_FOUND (-2)
#define ERROR_ELEMENT_NOT_FOUND (-3)
#define ERROR_DURING_PRINT (-4)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct osoba* new_osoba;

typedef struct osoba {
  char ime[64];
  char prezime[64];
  int godina;
  new_osoba Next;
} osoba;

int unosP(char ime[64], char prezime[64], int godina, new_osoba P);

int unosK(char ime[64], char prezime[64], int godina, new_osoba P);

int ispis(new_osoba P);

int traziPoPrez(char prezime[64], new_osoba P);

int brisiEl(char ime[64], char prez[64], new_osoba P);

int main(void) {
  new_osoba head = NULL;
  head = (new_osoba)malloc(sizeof(osoba));
  if (head == NULL) {
    printf("Memory allocation error");
    return ERROR_MEMORY_ALLOCATION;
  }
  head->Next = NULL;
  unosP("Pero", "Peric", 2005, head);
  unosP("Ivo", "Ivic", 1985, head);
  unosK("Andro", "Andric", 1991, head);
  unosK("Duje", "Dujic", 1905, head);
  ispis(head->Next);
  traziPoPrez("Andric", head->Next);
  brisiEl("Pero", "Peric", head);
  brisiEl("Andro", "Andric", head);
  ispis(head->Next);
  traziPoPrez("Andric", head->Next);
  return EXIT_SUCCESS;
}

int unosP(char ime[64], char prezime[64], int godina, new_osoba P) {
  new_osoba Q = NULL;
  Q = (new_osoba)malloc(sizeof(osoba));
  if (Q == NULL) {
    printf("Memory allocation error");
    return ERROR_MEMORY_ALLOCATION;
  }
  Q->Next = P->Next;
  P->Next = Q;
  strcpy(Q->ime, ime);
  strcpy(Q->prezime, prezime);
  Q->godina = godina;
  return EXIT_SUCCESS;
}

int unosK(char ime[64], char prezime[64], int godina, new_osoba P) {
  new_osoba Q = NULL;
  Q = (new_osoba)malloc(sizeof(osoba));
  if (Q == NULL) {
    printf("Memory allocation error");
    return ERROR_MEMORY_ALLOCATION;
  }
  while (P->Next != NULL) {
    P = P->Next;
  }
  Q->Next = P->Next;
  P->Next = Q;
  strcpy(Q->ime, ime);
  strcpy(Q->prezime, prezime);
  Q->godina = godina;
  return EXIT_SUCCESS;
}

int ispis(new_osoba P) {
  if (P == NULL) {
    printf("Ispis nije moguc.");
    return ERROR_DURING_PRINT;
  }
  printf("Ispis:\n");
  while (P != NULL) {
    printf("- %s %s %d\n", P->ime, P->prezime, P->godina);
    P = P->Next;
  }
  return EXIT_SUCCESS;
}

int traziPoPrez(char prezime[64], new_osoba P) {
  while (P != NULL) {
    P = P->Next;
    if (strcmp(P->prezime, prezime) == 0) {
      printf("Osoba pronadena po prezimenu %s: %s %s\n", prezime, P->ime,
             P->prezime);
      return EXIT_SUCCESS;
    } else if (strcmp(P->prezime, prezime) != 0 && P->Next == NULL) {
      printf("Osoba po prezimenu %s nije pronadena.\n", prezime);
      return ERROR_PERSON_NOT_FOUND;
    }
  }

  return EXIT_SUCCESS;
}

int brisiEl(char ime[64], char prezime[64], new_osoba P) {
  new_osoba Q = NULL;
  while (P->Next != NULL) {
    Q = P;
    P = P->Next;
    if (strcmp(P->prezime, prezime) == 0 && strcmp(P->ime, ime) == 0) {
      Q->Next = P->Next;
      P->Next = NULL;
      free(P);
      return EXIT_SUCCESS;
    }
  }
  if (P->Next == NULL) {
    printf("Ne postoji element.\n");
    return ERROR_ELEMENT_NOT_FOUND;
  }
  return EXIT_SUCCESS;
}
