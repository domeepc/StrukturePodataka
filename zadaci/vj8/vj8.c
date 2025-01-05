#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct CvorStabla Cvor;
typedef struct CvorStabla* Stablo;
typedef struct CvorStabla* Pozicija;

Stablo StvoriPrazno(Stablo S);
Pozicija Trazi(int el, Stablo S);
Stablo Dodaj(int el, Stablo S);
Stablo Brisi(int el, Stablo S);
Pozicija TraziMin(Stablo S);
Pozicija TraziMax(Stablo S);
void IspisPost(Stablo S);
void IspisPre(Stablo S);
void IspisIn(Stablo S);
int visina(Stablo S);
void IspisTrLevel(Stablo S, int level);
void IspisLevel(Stablo S);

struct CvorStabla {
  int El;
  Stablo Desno;
  Stablo Lijevo;
};

int main() {
  Cvor* root = NULL;
  Cvor* temp = NULL;
  char op;
  int el;
  root = StvoriPrazno(root);
  while (1) {
    printf("\nIzaberi opciju: \n");
    printf(
        "\n1. - Dodaj el u stablo\n2. - Trazi el u stablu\n3. - Brisi el iz "
        "stabla\n4. - Izadi iz programa\n");
    (void)scanf(" %c", &op);
    switch (op) {
      case '1':
        printf("Unesi el za dodati: ");
        (void)scanf("%d", &el);
        root = Dodaj(el, root);
        printf("Ispis preorder: ");
        IspisPre(root);
        printf("\n");
        printf("Ispis inorder: ");
        IspisIn(root);
        printf("\n");
        printf("Ispis postorder: ");
        IspisPost(root);
        printf("\n");
        printf("Ispis levelorder: ");
        IspisLevel(root);
        printf("\n");
        break;
      case '2':
        printf("Unesi el za pronaci: ");
        (void)scanf("%d", &el);
        temp = Trazi(el, root);
        if (temp == NULL) {
          printf("Nema ga.");
          break;
        }
        printf("Adresa elementa je: 0x%p", temp);
        break;
      case '3':
        printf("Unesi el za izbrisati: ");
        (void)scanf("%d", &el);
        root = Brisi(el, root);
        printf("Ispis preorder: ");
        IspisPre(root);
        printf("\n");
        printf("Ispis inorder: ");
        IspisIn(root);
        printf("\n");
        printf("Ispis postorder: ");
        IspisPost(root);
        printf("\n");
        printf("Ispis levelorder: ");
        IspisLevel(root);
        printf("\n");
        break;
      case '4':
        return 0;
    }
  }
  root = StvoriPrazno(root);
  return 0;
}

Stablo StvoriPrazno(Stablo S) {
  if (S != NULL) {
    StvoriPrazno(S->Lijevo);
    StvoriPrazno(S->Desno);
    free(S);
  }
  return NULL;
}

Pozicija Trazi(int el, Stablo S) {
  if (NULL == S) {
    return NULL;
  }
  if (el < S->El) {
    return Trazi(el, S->Lijevo);
  }
  if (el > S->El) {
    return Trazi(el, S->Desno);
  }
  return S;
}

Stablo Dodaj(int el, Stablo S) {
  if (NULL == S) {
    S = (Stablo)malloc(sizeof(Cvor));
    if (S == NULL) {
      return NULL;
    }
    S->El = el;
    S->Lijevo = NULL;
    S->Desno = NULL;
  }
  if (el < S->El) {
    S->Lijevo = Dodaj(el, S->Lijevo);
  }
  if (el > S->El) {
    S->Desno = Dodaj(el, S->Desno);
  }
  return S;
}

Stablo Brisi(int el, Stablo S) {
  Stablo Temp = NULL;
  if (NULL == S) {
    return S;
  }
  if (el < S->El) {
    S->Lijevo = Brisi(el, S->Lijevo);
  } else if (el > S->El) {
    S->Desno = Brisi(el, S->Desno);
  } else {
    if (S->Lijevo != NULL && S->Desno != NULL) {
      Temp = TraziMin(S->Desno);
      S->El = Temp->El;
      S->Desno = Brisi(S->El, S->Desno);
    } else {
      Temp = S;
      if (NULL == S->Lijevo) {
        S = S->Desno;
      } else {
        S = S->Lijevo;
      }
      free(Temp);
    }
  }
  return S;
}

Pozicija TraziMin(Stablo S) {
  if (NULL == S) {
    return NULL;
  }
  if (NULL == S->Lijevo) {
    return S;
  }
  return TraziMin(S->Lijevo);
}

Pozicija TraziMax(Stablo S) {
  if (NULL != S) {
    if (NULL != S->Desno) {
      S = S->Desno;
    }
  }
  return S;
}

void IspisPost(Stablo S) {
  if (S == NULL) {
    return;
  }
  IspisPost(S->Lijevo);
  IspisPost(S->Desno);

  printf("%d ", S->El);
}

void IspisIn(Stablo S) {
  if (S == NULL) {
    return;
  }

  IspisIn(S->Lijevo);
  printf("%d ", S->El);
  IspisIn(S->Desno);
}

void IspisPre(Stablo S) {
  if (S == NULL) {
    return;
  }
  printf("%d ", S->El);
  IspisPre(S->Lijevo);
  IspisPre(S->Desno);
}

int visina(Stablo S) {
  if (S == NULL)
    return 0;
  else {
    int LVis = visina(S->Lijevo);
    int DVis = visina(S->Desno);
    if (LVis > DVis)
      return (LVis + 1);
    else
      return (DVis + 1);
  }
}

void IspisTrLevel(Stablo S, int level) {
  if (S == NULL) return;
  if (level == 1)
    printf("%d ", S->El);
  else if (level > 1) {
    IspisTrLevel(S->Lijevo, level - 1);
    IspisTrLevel(S->Desno, level - 1);
  }
}

void IspisLevel(Stablo S) {
  int h = visina(S);
  int i;
  for (i = 1; i <= h; i++) IspisTrLevel(S, i);
}