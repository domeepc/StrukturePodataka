#define _CRT_SECURE_NO_WARNINGS
#define ERROR_OPPENING_FILE -1
#define ERROR_MEMORY_ALLOCATION -2
#define SIZE_BUFFER 1024
#define MAX_POINTS 60
#include <stdio.h>
#include <stdlib.h>

int count_rows(char* filename);

typedef struct {
  char name[SIZE_BUFFER];
  char surname[SIZE_BUFFER];
  int points;
} _student;

int main() {
  _student* s = NULL;
  FILE* fp = NULL;
  char* filename = "students.txt";
  int student_number, i;
  student_number = count_rows(filename);

  s = (_student*)malloc(sizeof(_student) * student_number);

  if (s == NULL) {
    printf("Malloc did not allocate any memory.\n");
    return ERROR_MEMORY_ALLOCATION;
  }

  fp = fopen(filename, "r");

  if (fp == NULL) {
    printf("Can't open a file.\n");
    return ERROR_OPPENING_FILE;
  }

  for (i = 0; i < student_number; i++) {
    (void)fscanf(fp, "%s %s %d", s[i].name, s[i].surname, &s[i].points);
  }

  fclose(fp);

  for (i = 0; i < student_number - 1; i++) {
    printf("%-14s%-14sABS: %-14dREL: %-14.2f\n", s[i].name, s[i].surname,
           s[i].points, (double)s[i].points / MAX_POINTS * 100);
  }

  free(s);

  return EXIT_SUCCESS;
}

int count_rows(char* filename) {
  FILE* fp = NULL;
  int count = 0;
  char buffer[SIZE_BUFFER];
  fp = fopen(filename, "r");

  if (fp == NULL) {
    printf("Can't open a file.\n");
    return ERROR_OPPENING_FILE;
  }

  while (!feof(fp)) {
    fgets(buffer, SIZE_BUFFER, fp);
    ++count;
  }

  fclose(fp);
  return count;
}