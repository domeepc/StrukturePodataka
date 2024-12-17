#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100
#define MIN_CHILDREN 10

typedef struct FileSystemNode {
  char name[MAX_NAME_LEN];
  struct FileSystemNode **children;
  struct FileSystemNode *parent;
  int childCount;
  int size;
  int isDirectory;
} FileSystemNode;

FileSystemNode *root;
FileSystemNode *currentDir;

FileSystemNode *createNode(char *name, int isDirectory, FileSystemNode *parent);
void initFileSystem();
void cd(char *dirName);
void mkdirCmd(char *dirName);
void touch(char *fileName);
void rm(char *fileName);
void rmrf(char *dirName);
void rmdirCmd(char *dirName);
void ls();
void freeRec(FileSystemNode *node);

int main() {
  char command[256], arg[MAX_NAME_LEN];

  initFileSystem();
  while (1) {
    printf("%s> ", currentDir->name);
    fgets(command, sizeof(command), stdin);
    command[strcspn(command, "\n")] = '\0';

    if (sscanf(command, "cd %s", arg) == 1) {
      cd(arg);
    } else if (strncmp(command, "mkdir", 5) == 0 &&
               sscanf(command, "mkdir %s", arg) == 1) {
      mkdirCmd(arg);
    } else if (strncmp(command, "touch", 5) == 0 &&
               sscanf(command, "touch %s", arg) == 1) {
      touch(arg);
    } else if (strncmp(command, "rmdir", 5) == 0 &&
               sscanf(command, "rmdir %s", arg) == 1) {
      rmdirCmd(arg);
    } else if (strncmp(command, "rm", 2) == 0) {

      if (strncmp(command, "rm -rf", 6) == 0 &&
          sscanf(command, "rm -rf %s", arg) == 1) {
        rmrf(arg);
      } else if (sscanf(command, "rm %s", arg) == 1) {
        rm(arg);
      }
    } else if (strcmp(command, "ls") == 0) {
      ls();
    } else if (strcmp(command, "exit") == 0) {
      printf("Exiting the file system...\n");
      break;
    } else {
      printf("Invalid command\n");
    }
  }

  freeRec(root);

  return 0;
}

FileSystemNode *createNode(char *name, int isDirectory,
                           FileSystemNode *parent) {
  FileSystemNode *newNode = (FileSystemNode *)malloc(sizeof(FileSystemNode));
  if (newNode == NULL) {
    return NULL;
  }
  newNode->children =
      (FileSystemNode **)malloc(MIN_CHILDREN * sizeof(FileSystemNode *));
  if (newNode->children == NULL) {
    return NULL;
  }
  strcpy(newNode->name, name);
  newNode->size = MIN_CHILDREN;
  newNode->isDirectory = isDirectory;
  newNode->childCount = 2;
  newNode->parent = parent;
  return newNode;
}

void initFileSystem() {
  root = createNode("/", 1, NULL);
  root->parent = root;
  currentDir = root;
}

void cd(char *dirName) {
  if (strcmp(dirName, "..") == 0) {
    currentDir = currentDir->parent;
    return;
  }

  if (strcmp(dirName, ".") == 0) {
    return;
  }

  for (int i = 2; i < currentDir->childCount; i++) {
    if (currentDir->children[i]->isDirectory &&
        strcmp(currentDir->children[i]->name, dirName) == 0) {
      currentDir = currentDir->children[i];
      return;
    }
  }

  printf("Directory %s not found\n", dirName);
}

void mkdirCmd(char *dirName) {
  if (strcmp(dirName, ".") == 0 || strcmp(dirName, "..") == 0) {
    printf("Directory already created.\n");
    return;
  }

  if (currentDir->childCount >= currentDir->size - 2) {
    currentDir->children = (FileSystemNode **)realloc(
        currentDir->children, 2 * currentDir->size * sizeof(FileSystemNode *));
    currentDir->size *= 2;
    if (currentDir->children == NULL) {
      printf("Memory allocation failed\n");
      return;
    }
  }

  FileSystemNode *newDir = createNode(dirName, 1, currentDir);
  currentDir->children[currentDir->childCount++] = newDir;
}

void touch(char *fileName) {
  if (currentDir->childCount >= currentDir->size - 2) {
    currentDir->children = (FileSystemNode **)realloc(
        currentDir->children, 2 * currentDir->size * sizeof(FileSystemNode *));
    currentDir->size *= 2;
    if (currentDir->children == NULL) {
      printf("Memory allocation failed\n");
      return;
    }
  }

  FileSystemNode *newFile = createNode(fileName, 0, currentDir);
  currentDir->children[currentDir->childCount++] = newFile;
}

void rm(char *fileName) {

  for (int i = 2; i < currentDir->childCount; i++) {
    if (currentDir->children[i]->isDirectory == 0 &&
        strcmp(currentDir->children[i]->name, fileName) == 0) {
      free(currentDir->children[i]);
      for (int j = i; j < currentDir->childCount - 1; j++) {
        currentDir->children[j] = currentDir->children[j + 1];
      }
      currentDir->childCount--;
      return;
    }
  }
  printf("File %s not found\n", fileName);
}

void rmdirCmd(char *dirName) {
  if (strcmp(dirName, ".") == 0 || strcmp(dirName, "..") == 0) {
    printf("Cannot remove current directory\n");
    return;
  }
  for (int i = 2; i < currentDir->childCount; i++) {
    if (currentDir->children[i]->isDirectory &&
        strcmp(currentDir->children[i]->name, dirName) == 0) {
      if (currentDir->children[i]->childCount == 2) {
        free(currentDir->children[i]);
        for (int j = i; j < currentDir->childCount - 1; j++) {
          currentDir->children[j] = currentDir->children[j + 1];
        }
        currentDir->childCount--;
        printf("Directory %s removed\n", dirName);
      } else {
        printf("Directory %s is not empty, cannot remove\n", dirName);
      }
      return;
    }
  }
  printf("Directory %s not found\n", dirName);
}

void ls() {
  printf("[DIR] .\n");
  printf("[DIR] ..\n");
  for (int i = 2; i < currentDir->childCount; i++) {
    printf("%s %s\n", currentDir->children[i]->isDirectory ? "[DIR]" : "[FILE]",
           currentDir->children[i]->name);
  }
}

void rmrf(char *dirName) {
  if (strcmp(dirName, ".") == 0 || strcmp(dirName, "..") == 0) {
    printf("Cannot remove current directory\n");
    return;
  }

  for (int i = 2; i < currentDir->childCount; i++) {
    if (strcmp(currentDir->children[i]->name, dirName) == 0) {
      freeRec(currentDir->children[i]);
      for (int j = i; j < currentDir->childCount - 1; j++) {
        currentDir->children[j] = currentDir->children[j + 1];
      }
      currentDir->childCount--;
      return;
    }
  }
  printf("File %s not found\n", dirName);
}

void freeRec(FileSystemNode *node) {
  if (node->childCount != 2) {
    for (int i = 2; i < node->childCount; i++) {
      freeRec(node->children[i]);
    }
  }
  free(node->children);
  free(node);
}
