#define _CRT_SECURE_NO_WARNINGS
#define ERROR_FILE_NOT_FOUND -1
#define ERROR_FILE_FORMAT -2
#define ERROR_MEM_ALL -3
#define SIZE_BUFFER 1024
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node *pok;

typedef struct Node {
  double number;
  pok next;
} Node;

pok createEl(double);

int scanFile(pok);

void static checkError(int err) {
  switch (err) {
    case ERROR_FILE_NOT_FOUND:
      printf("File not found.\n");
      break;
    case ERROR_FILE_FORMAT:
      printf("File format is wrong.\n");
      break;
    case ERROR_MEM_ALL:
      printf("Memory allocation error.\n");
      break;
  }
}

void calcPost(char, pok);

void deleteEl(pok);

void printPost(pok);

int main() {
  Node head = {.next = NULL};

  scanFile(&head);
  printf("Result: ");
  printPost(head.next);
  return EXIT_SUCCESS;
}

int scanFile(pok pos) {
  FILE *fp = NULL;
  pok Q = NULL;
  double temp;
  char *buffer;
  char operation;
  int byteSize, num = 0, op_c = 0;
  fp = fopen("postfix.txt", "r");
  if (fp == NULL) {
    checkError(ERROR_FILE_NOT_FOUND);
    return ERROR_FILE_NOT_FOUND;
  };

  buffer = (char *)malloc(sizeof(char) * SIZE_BUFFER);
  if (buffer == NULL) {
    checkError(ERROR_MEM_ALL);
    return ERROR_MEM_ALL;
  }
  while (!feof(fp)) {
    fgets(buffer, SIZE_BUFFER, fp);
    while (strlen(buffer) > 0) {
      num = sscanf(buffer, "%lf %n", &temp, &byteSize);
      op_c = sscanf(buffer, "%c %n", &operation, &byteSize);
      if (num == 0) {
        calcPost(operation, pos);
      } else if (num == 1) {
        Q = createEl(temp);
        Q->next = pos->next;
        pos->next = Q;
      } else {
        checkError(ERROR_FILE_FORMAT);
        return ERROR_FILE_FORMAT;
      }
      buffer = buffer + byteSize;
    }
  }
  return EXIT_SUCCESS;
}

pok createEl(double data) {
  pok Q = NULL;
  Q = (pok)malloc(sizeof(Node));
  if (Q == NULL) {
    checkError(ERROR_MEM_ALL);
    return NULL;
  }
  Q->number = data;
  Q->next = NULL;
  return Q;
}

void calcPost(char op, pok pos) {
  int count = 1;
  double num1, num2, result;
  num2 = pos->next->number;
  num1 = pos->next->next->number;

  switch (op) {
    case '+':
      result = num1 + num2;
      pos->next->next->number = result;
      deleteEl(pos);
      break;
    case '-':
      result = num1 - num2;
      pos->next->next->number = result;
      deleteEl(pos);
      break;
    case '*':
      result = num1 * num2;
      pos->next->next->number = result;
      deleteEl(pos);
      break;
    case '/':
      result = num1 / num2;
      pos->next->next->number = result;
      deleteEl(pos);
      break;
    default:
      return;
      break;
  }
}

void printPost(pok pos) {
  while (pos != NULL) {
    printf("%lf\n", pos->number);
    pos = pos->next;
  }
}

void deleteEl(pok num1) {
  pok temp;
  temp = num1->next;
  num1->next = temp->next;
  free(temp);
}