#define _CRT_SECURE_NO_WARNINGS
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
  receiptPosition rec = NULL, tempRec = NULL;
  articlePosition art = NULL, tempArt = NULL, tempArt1 = NULL, tempPrev = NULL;
  double price;
  char filename[BUFFER_SIZE], buffer[BUFFER_SIZE], name[BUFFER_SIZE];
  int count, quant, day, month, year;
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
    }
    rec = createReceipt();
    if (rec == NULL) {
      checkError(ERROR_MEM_ALL);
      return ERROR_MEM_ALL;
    }
    fp1 = fopen(filename, "r");
    if (fp1 == NULL) {
      checkError(ERROR_FILE_NFOUND);
      return ERROR_FILE_NFOUND;
    }
    fgets(buffer, BUFFER_SIZE, fp1);
    count = sscanf(buffer, "%d-%d-%d", &day, &month, &year);
    if (count != 3) {
      checkError(ERROR_WFORMAT);
      return (ERROR_WFORMAT);
    }
    sprintf(rec->receiptDate, "%d-%d-%d", year, month, day);
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
      tempArt = &rec->articleHead;
      while (tempArt->next != NULL &&
             strcmp(tempArt->next->name, art->name) < 0)
        tempArt = tempArt->next;

      if (tempArt->next != NULL &&
          strcmp(tempArt->next->name, art->name) == 0) {
        tempArt->next->quantity += art->quantity;
      } else {
        art->next = tempArt->next;
        tempArt->next = art;
      }
    }
    tempRec = recHead;
    while (tempRec->next != NULL &&
           strcmp(tempRec->next->receiptDate, rec->receiptDate) < 0) {
      tempRec = tempRec->next;
    }
    if (tempRec->next != NULL &&
        strcmp(tempRec->next->receiptDate, rec->receiptDate) == 0) {
      tempArt = rec->articleHead.next;
      tempPrev = &rec->articleHead;
      while (tempArt != NULL) {
        tempArt1 = &tempRec->next->articleHead;
        while (tempArt1->next != NULL &&
               strcmp(tempArt1->next->name, tempArt->name) < 0) {
          tempArt1 = tempArt1->next;
        }
        if (tempArt1->next != NULL &&
            strcmp(tempArt1->next->name, tempArt->name) == 0) {
          printf("usao\n");
          tempArt1->next->quantity += tempArt->quantity;

        } else {
          tempPrev->next = tempArt->next;
          tempArt->next = tempArt1->next;
          tempArt1->next = tempArt;
          tempArt = tempPrev->next;
          continue;
        }
        tempArt = tempArt->next;
        tempPrev = tempPrev->next;
      }
    } else {
      rec->next = tempRec->next;
      tempRec->next = rec;
    }
    fclose(fp1);
  }
  fclose(fp);
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
