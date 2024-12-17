#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100
#define MAX_CHILDREN 10

typedef struct FileSystemNode {
  char name[MAX_NAME_LEN];
  struct FileSystemNode* children[MAX_CHILDREN];
  struct FileSystemNode* parent;
  int childCount;
  int isDirectory;
} FileSystemNode;

FileSystemNode* root;
FileSystemNode* currentDir;

FileSystemNode* createNode(char* name, int isDirectory,
                           FileSystemNode* parent) {
  FileSystemNode* newNode = (FileSystemNode*)malloc(sizeof(FileSystemNode));
  if (newNode == NULL) {
    return NULL;
  }
  strcpy(newNode->name, name);
  newNode->isDirectory = isDirectory;
  newNode->childCount = 0;
  newNode->parent = parent;
  return newNode;
}

void initFileSystem() {
  root = createNode("/", 1, NULL);
  currentDir = root;
}

void cd(char* dirName) {
  if (strcmp(dirName, "..") == 0) {
    if (currentDir != root) {
      currentDir = currentDir->parent;
      printf("Moved to parent directory\n");
    } else {
      printf("Already at root\n");
    }
    return;
  }

  for (int i = 0; i < currentDir->childCount; i++) {
    if (currentDir->children[i]->isDirectory &&
        strcmp(currentDir->children[i]->name, dirName) == 0) {
      currentDir = currentDir->children[i];
      printf("Moved to directory %s\n", dirName);
      return;
    }
  }

  printf("Directory %s not found\n", dirName);
}

void mkdirCmd(char* dirName) {
  if (currentDir->childCount >= MAX_CHILDREN) {
    printf("Cannot create more directories in this directory\n");
    return;
  }

  FileSystemNode* newDir = createNode(dirName, 1, currentDir);
  currentDir->children[currentDir->childCount++] = newDir;
  printf("Directory %s created\n", dirName);
}

void touch(char* fileName) {
  if (currentDir->childCount >= MAX_CHILDREN) {
    printf("Cannot create more files or directories in this directory\n");
    return;
  }

  FileSystemNode* newFile = createNode(fileName, 0, currentDir);
  currentDir->children[currentDir->childCount++] = newFile;
  printf("File %s created\n", fileName);
}

void rm(char* fileName) {
  for (int i = 0; i < currentDir->childCount; i++) {
    if (currentDir->children[i]->isDirectory == 0 &&
        strcmp(currentDir->children[i]->name, fileName) == 0) {
      free(currentDir->children[i]);
      for (int j = i; j < currentDir->childCount - 1; j++) {
        currentDir->children[j] = currentDir->children[j + 1];
      }
      currentDir->childCount--;
      printf("File %s removed\n", fileName);
      return;
    }
  }
  printf("File %s not found\n", fileName);
}

void rmdirCmd(char* dirName) {
  for (int i = 0; i < currentDir->childCount; i++) {
    if (currentDir->children[i]->isDirectory &&
        strcmp(currentDir->children[i]->name, dirName) == 0) {
      if (currentDir->children[i]->childCount == 0) {
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
  printf("Contents of %s:\n", currentDir->name);
  for (int i = 0; i < currentDir->childCount; i++) {
    printf("%s %s\n", currentDir->children[i]->isDirectory ? "[DIR]" : "[FILE]",
           currentDir->children[i]->name);
  }
}

int main() {
  char command[256], arg[MAX_NAME_LEN];

  initFileSystem();
  printf("Simple File System Initialized. Root is /.\n");

  while (1) {
    printf("Enter command: ");
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
    } else if (strncmp(command, "rm", 2) == 0 &&
               sscanf(command, "rm %s", arg) == 1) {
      rm(arg);
    } else if (strcmp(command, "ls") == 0) {
      ls();
    } else if (strcmp(command, "exit") == 0) {
      printf("Exiting the file system...\n");
      break;
    } else {
      printf("Invalid command\n");
    }
  }

  return 0;
}