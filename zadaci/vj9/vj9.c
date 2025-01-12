#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct TreeNode Node;
typedef struct TreeNode* Tree;
typedef struct TreeNode* Pos;

Tree create(Tree S);
Tree insert(int el, Tree S);
void printIn(Tree S, FILE* fp);
int sum(Tree);

struct TreeNode {
  int El;
  Tree right;
  Tree left;
};

int main() {
  FILE* fp = NULL;
  fp = fopen("stablo.txt", "w");
  if (fp == NULL) {
    return;
  }
  Node* root = NULL;
  srand(time(NULL));
  int niz[10] = {0}, i;
  root = create(root);
  for (i = 0; i < (sizeof(niz) / sizeof(niz[0])); i++) {
    niz[i] = rand() % (90 - 10 + 1) + 10;
    root = insert(niz[i], root);
  }

  printIn(root, fp);

  sum(root);

  printIn(root, fp);

  root = create(root);

  return 0;
}

Tree create(Tree S) {
  if (S != NULL) {
    create(S->left);
    create(S->right);
    free(S);
  }
  return NULL;
}

Tree insert(int el, Tree S) {
  if (NULL == S) {
    S = (Tree)malloc(sizeof(Node));
    if (S == NULL) {
      return NULL;
    }
    S->El = el;
    S->left = NULL;
    S->right = NULL;
  } else if (el >= S->El) {
    S->left = insert(el, S->left);
  } else if (el <= S->El) {
    S->right = insert(el, S->right);
  }
  return S;
}

int sum(Tree S) {
  if (S == NULL) {
    return 0;
  }

  int el_before = S->El;
  S->El = sum(S->left) + sum(S->right);
  return S->El + el_before;
}

void printIn(Tree S, FILE* fp) {
  if (S == NULL) {
    return;
  }
  printIn(S->left, fp);
  fprintf(fp, "%d ", S->El);
  printIn(S->right, fp);
  fprintf(fp, "\n");
}