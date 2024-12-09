#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR_MEM_ALL -1
#define ERROR_FILE_NFOUND -2
#define ERROR_WFORMAT -3
#define BUFFER_SIZE 1024
#define DATE_SIZE 1024

struct _receipt;
struct _article;

typedef struct _receipt *receiptPosition;
typedef struct _article *articlePosition;

void static checkError(int err) {
  switch (err) {
    case ERROR_MEM_ALL:
      printf("Error while alocating memory.\n");
      break;
    case ERROR_FILE_NFOUND:
      printf("Error file not found.\n");
      break;
    case ERROR_WFORMAT:
      printf("Error wrong format.\n");
      break;
    default:
      break;
  }
}

typedef struct _article {
  char name[BUFFER_SIZE];
  int quantity;
  double price;
  articlePosition next;
} Article;

typedef struct _receipt {
  char receiptDate[DATE_SIZE];
  receiptPosition next;
  Article articleHead;
} Receipt;

articlePosition createArticle(char *, int, double);
receiptPosition createReceipt();

int scanFile(receiptPosition);

void print_receipts(receiptPosition);

int main() {
  Receipt receipts;
  receipts.next = NULL;
  scanFile(&receipts);
  print_receipts(receipts.next);
  return EXIT_SUCCESS;
}

articlePosition createArticle(char *name, int quant, double price) {
  articlePosition art = NULL;
  art = (articlePosition)malloc(sizeof(Article));
  if (art == NULL) {
    checkError(ERROR_MEM_ALL);
    return NULL;
  }
  strcpy(art->name, name);
  art->price = price;
  art->quantity = quant;
  art->next = NULL;
  return art;
}

receiptPosition createReceipt() {
  receiptPosition rec = NULL;
  rec = (receiptPosition)malloc(sizeof(Receipt));
  if (rec == NULL) {
    checkError(ERROR_MEM_ALL);
    return NULL;
  }
  strcpy(rec->receiptDate, " ");
  rec->next = NULL;
  rec->articleHead.next = NULL;
  return rec;
}

int scanFile(receiptPosition recHead) {
  FILE *fp = NULL, *fp1 = NULL;
  receiptPosition rec = NULL;
  articlePosition art = NULL;
  double price;
  char filename[BUFFER_SIZE], buffer[BUFFER_SIZE], name[BUFFER_SIZE];
  int count, quant, counter = 0, day, month, year;
  fp = fopen("racuni.txt", "r");
  if (fp == NULL) {
    checkError(ERROR_FILE_NFOUND);
    return ERROR_FILE_NFOUND;
  }
  while (!feof(fp)) {
    fgets(buffer, BUFFER_SIZE, fp);
    count = sscanf(buffer, "%[^\n]", filename);
    if (count != 1) {
      checkError(ERROR_WFORMAT);
      return ERROR_WFORMAT;
    } else {
      rec = createReceipt();
      if (rec == NULL) {
        checkError(ERROR_MEM_ALL);
        return ERROR_MEM_ALL;
      }
      rec->next = recHead->next;
      recHead->next = rec;
      fp1 = fopen(filename, "r");
      if (fp1 == NULL) {
        checkError(ERROR_FILE_NFOUND);
        return ERROR_FILE_NFOUND;
      }
      counter = 0;
      while (counter < 1) {
        fgets(buffer, BUFFER_SIZE, fp1);
        count = sscanf(buffer, "%d-%d-%d", &day, &month, &year);
        if (count != 3) {
          checkError(ERROR_WFORMAT);
          return (ERROR_WFORMAT);
        }
        sprintf(rec->receiptDate, "%d-%d-%d", year, month, day);
        counter++;
      } 
      while (!feof(fp1)) {
        fgets(buffer, BUFFER_SIZE, fp1);
        count = sscanf(buffer, "%s %d %lf", name, &quant, &price);
        if (count != 3) {
          checkError(ERROR_WFORMAT);
          return ERROR_WFORMAT;
        }
        art = createArticle(name, quant, price);
        if (art == NULL) {
          checkError(ERROR_MEM_ALL);
          return ERROR_MEM_ALL;
        }
        art->next = rec->articleHead.next;
        rec->articleHead.next = art;
      }
    }
  }
  return EXIT_SUCCESS;
}

void print_receipts(receiptPosition rec) {
  while (rec != NULL) {
    printf("----------\n%s\n", rec->receiptDate);
    while (rec->articleHead.next != NULL) {
      printf("%s %d %.2lf\n", rec->articleHead.next->name,
             rec->articleHead.next->quantity, rec->articleHead.next->price);
      rec->articleHead.next = rec->articleHead.next->next;
    }
    rec = rec->next;
  }
}