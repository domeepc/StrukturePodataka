#define _CRT_SECURE_NO_WARNINGS
#define SIZE_BUFFER 1024
#define ERROR_MEMORY_ALLOCATION (-1)
#define ERROR_PERSON_NOT_FOUND (-2)
#define ERROR_ELEMENT_NOT_FOUND (-3)
#define ERROR_DURING_PRINT (1)
#define ERROR_WRITING_FILE (2)
#define ERROR_OPPENING_FILE (3)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct osoba *new_osoba;

typedef struct osoba {
  char ime[64];
  char prezime[64];
  int godina;
  new_osoba Next;
} osoba;

int unosIzaEl(char[64], char[64], char[64], int, new_osoba); // a done

int upisiDat(new_osoba); // d done

int unosIspredEl(char[64], char[64], char[64], int, new_osoba); // b done

int citajDat(new_osoba P); // e

int unosP(char[64], char[64], int, new_osoba);

int unosK(char[64], char[64], int, new_osoba);

void sortList(new_osoba);

int ispis(new_osoba);

new_osoba traziPoPrez(char[64], new_osoba);

int brisiEl(char[64], char[64], new_osoba);

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
  upisiDat(head->Next);
  unosIzaEl("Ivic", "Mate", "Matic", 1805, head);
  unosIspredEl("Matic", "Karlo", "Karlic", 1956, head);
  ispis(head->Next);
  citajDat(head);
  ispis(head->Next);
  printf("Sortirana lista:\n");
  sortList(head);
  ispis(head->Next);
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
  printf("----------------------\n");
  return EXIT_SUCCESS;
}

new_osoba traziPoPrez(char prezime[64], new_osoba P) {
  while (P != NULL && strcmp(P->prezime, prezime) != 0) {
    P = P->Next;
  }
  if (P == NULL) {
    printf("Nije pronaden element po prezimenu %s.\n", prezime);
  }

  return P;
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

int upisiDat(new_osoba P) {
  FILE *fp = NULL;
  fp = fopen("osobe.txt", "w");
  if (fp == NULL) {
    printf("Greska prilikom pisanja datoteke.\n");
    return ERROR_WRITING_FILE;
  }
  while (P != NULL) {
    fprintf(fp, "%s %s %d", P->ime, P->prezime, P->godina);
    if (P->Next != NULL) {
      fprintf(fp, "\n");
    }
    P = P->Next;
  }
  fclose(fp);
  printf("Uspjesan upis u datoteku.\n");
  return EXIT_SUCCESS;
}

int citajDat(new_osoba P) {
  FILE *fp = NULL;
  char ime[64], prezime[64];
  int god = 0;
  fp = fopen("osobe_unos.txt", "r");
  if (fp == NULL) {
    printf("Datoteka ne postoji.\n");
    return ERROR_OPPENING_FILE;
  }
  while (!feof(fp)) {
    (void)fscanf(fp, "%s %s %d", ime, prezime, &god);
    unosK(ime, prezime, god, P);
  }
  return EXIT_SUCCESS;
}

int unosIzaEl(char prezime[64], char ime[64], char prez[64], int godina,
              new_osoba P) {
  new_osoba prev = NULL;
  new_osoba Q = NULL;
  prev = traziPoPrez(prezime, P);
  if (prev == NULL) {
    printf("Greska u pronalazenju osobe.");
    return ERROR_ELEMENT_NOT_FOUND;
  }
  Q = (new_osoba)malloc(sizeof(osoba));
  if (Q == NULL) {
    printf("Memory allocation error");
    return ERROR_MEMORY_ALLOCATION;
  }
  Q->Next = prev->Next;
  prev->Next = Q;
  strcpy(Q->ime, ime);
  strcpy(Q->prezime, prez);
  Q->godina = godina;

  return EXIT_SUCCESS;
}

int unosIspredEl(char prezime[64], char ime[64], char prez[64], int godina,
                 new_osoba P) {
  new_osoba prev = NULL;
  new_osoba Q = NULL;
  new_osoba temp = P;
  prev = traziPoPrez(prezime, P);
  if (prev == NULL) {
    printf("Osoba nije pronadena.\n");
    return ERROR_ELEMENT_NOT_FOUND;
  }
  while (temp->Next != prev) {
    temp = temp->Next;
  }
  Q = (new_osoba)malloc(sizeof(osoba));
  if (Q == NULL) {
    printf("Memory allocation error");
    return ERROR_MEMORY_ALLOCATION;
  }
  Q->Next = prev;
  temp->Next = Q;
  strcpy(Q->ime, ime);
  strcpy(Q->prezime, prez);
  Q->godina = godina;
  return EXIT_SUCCESS;
}

void sortList(new_osoba P) {
  new_osoba Q, prev, temp, end;

  end = NULL;
  while (P->Next != end) // P->Next != NULL
  {
    prev = P;
    Q = P->Next;
    while (Q->Next != end) {
      if (strcmp(Q->prezime, Q->Next->prezime) < 0) {
        temp = Q->Next;
        prev->Next = temp;
        Q->Next = temp->Next;
        temp->Next = Q;
        Q = temp;
      }
      prev = Q;
      Q = Q->Next;
    }
    end = Q;
  }
}
