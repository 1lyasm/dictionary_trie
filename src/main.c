#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHILD_COUNT 10

struct TrieNode {
  int isLast;
  struct TrieNode *childs[CHILD_COUNT];
  char *word;
};

struct Trie {
  struct TrieNode *root;
};

struct TrieNode *buildTrieNode() {
  int i = -1;
  struct TrieNode *node = malloc(sizeof(struct TrieNode));
  node->isLast = 0;
  node->word = 0;
  for (i = 0; i < CHILD_COUNT; ++i)
    node->childs[i] = 0;
  return node;
}

void freeTrieNode(struct TrieNode *node) {
  free(node->word);
  free(node);
  node = 0;
}

struct Trie *buildTrie() {
  struct Trie *trie = malloc(sizeof(struct Trie));
  trie->root = buildTrieNode();
  return trie;
}

void freeTrie(struct TrieNode *root) {
  int i = -1;
  /* printf("freeTrie: freeing subtrie with root: %s\n", root->word); */
  for (i = 0; i < CHILD_COUNT; ++i) {
    if (root->childs[i] != 0) {
      freeTrie(root->childs[i]);
    }
  }
  freeTrieNode(root);
}

int getAsNumber(int character) {
  int value = -1;
  int characterUpper = toupper((char)character);
  switch (characterUpper) {
  case '#':
    value = 0;
    break;
  case '(':
  case ')':
  case '<':
  case '>':
    value = 1;
    break;
  case 'A':
  case 'B':
  case 'C':
    value = 2;
    break;
  case 'D':
  case 'E':
  case 'F':
    value = 3;
    break;
  case 'G':
  case 'H':
  case 'I':
    value = 4;
    break;
  case 'J':
  case 'K':
  case 'L':
    value = 5;
    break;
  case 'M':
  case 'N':
  case 'O':
    value = 6;
    break;
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
    value = 7;
    break;
  case 'T':
  case 'U':
  case 'V':
    value = 8;
    break;
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
    value = 9;
    break;
  default:
    printf("\nUnknown character in file\n");
    exit(EXIT_FAILURE);
  }
  return value;
}

int checkIfIsTerminal(struct TrieNode *node) {
  int i = -1;
  int isTerminal = 1;
  for (i = 0; i < CHILD_COUNT && isTerminal; ++i)
    if (node->childs[i] != 0)
      isTerminal = 0;
  return isTerminal;
}

void insertString(struct Trie *trie, char *str) {
  int i = -1;
  int currentChar = str[0];
  int value = -1;
  struct TrieNode *node = trie->root;
  int isNew = 0;
  for (i = 1; currentChar != 0; ++i) {
    value = getAsNumber(currentChar);
    if (node->childs[value] == 0) {
      node->childs[value] = buildTrieNode();
      if (str[i] == 0) {
        node->childs[value]->word = malloc((i + 1) * sizeof(char));
        strcpy(node->childs[value]->word, str);
        node->childs[value]->isLast = 1;
      }
      isNew = 1;
    }
    node = node->childs[value];
    currentChar = str[i];
  }
  if (checkIfIsTerminal(node) && !isNew) {
    node->childs[0] = buildTrieNode();
    node->childs[0]->word = malloc((i + 2) * sizeof(char));
    strcpy(node->childs[0]->word, str);
    node->childs[0]->isLast = 1;
  }
  return;
}

void fillTrie(FILE *file, struct Trie *trie) {
  char *tempString = malloc(256 * sizeof(char));
  while (!feof(file)) {
    fscanf(file, " %s\n", tempString);
    insertString(trie, tempString);
  }
  fclose(file);
  free(tempString);
}

void traverseTrie(struct Trie *trie, char *numberString) {
  struct TrieNode *node = trie->root;
  int value = -1;
  int len = strlen(numberString);
  int i = -1;
  for (i = 0; i < len && node != 0; ++i) {
    value = numberString[i] - '0';
    node = node->childs[value];
  }
  if (node == 0 || !node->isLast) {
    printf("There is not a word in the dictionary that matches this number.");
  } else {
    printf("%s", node->word);
    while (node->childs[0] != 0) {
      node = node->childs[0];
      printf("\n%s", node->word);
    }
  }
}

void runMainLoop(struct Trie *trie) {
  char *response = malloc(256 * sizeof(char));
  printf("\n> Enter 'q' to exit.");
  do {
    printf("\n> ");
    scanf(" %s", response);
    if (response[0] != 'q')
      traverseTrie(trie, response);
  } while (response[0] != 'q');
  free(response);
}

void printTrie(struct Trie *trie) {
  int queueSize = 1024;
  struct TrieNode **queue = calloc(queueSize, sizeof(struct TrieNode *));
  int start = 0;
  int end = 0;
  int i;
  int levelsSize = 256;
  int levelsLen = 0;
  int *levels = calloc(levelsSize, sizeof(int));
  int level = 0;
  int isLBrace = 1;
  queue[0] = trie->root;
  levels[levelsLen++] = 0;
  printf("Trie: ");
  while (start <= end) {
    int canPrintRBrace = 0;
    struct TrieNode *visited = queue[start];
    start = (start + 1) % queueSize;

    printf("levels: ");
    for (i = 0; i < levelsLen; ++i) {
        printf("%d ", levels[i]);
    }

    if (level == 0 || levels[start] != level) {
        if (isLBrace == 1) {
            printf("{ ");
        } else {
            canPrintRBrace = 1;
        }
        isLBrace = !isLBrace;
        level = levels[start];
    }

    if (visited->word != NULL) {
      printf("'%s' ", visited->word);
    }

    if (canPrintRBrace == 1) {
        printf(" }\n");
        canPrintRBrace = 0;
    }

    for (i = 0; i < CHILD_COUNT; ++i) {
      if (visited->childs[i] != NULL) {
        end = (end + 1) % queueSize;

        queue[end] = visited->childs[i];

        levels[levelsLen] = level;
        printf("levels[%d]: %d\n", levelsLen, level);
        ++levelsLen;
      }
    }
    ++level;
    printf("level: %d\n", level);
  }

  free(levels);
  free(queue);
}

int main() {
  char *fileName = malloc(64 * sizeof(char));
  FILE *file;
  struct Trie *trie = buildTrie();
  printf("Input file name: ");
  scanf(" %s", fileName);
  file = fopen(fileName, "r");
  free(fileName);
  if (!file) {
    printf("\nCould not open the file\n");
    exit(EXIT_FAILURE);
  }
  fillTrie(file, trie);
  printTrie(trie);
  runMainLoop(trie);
  freeTrie(trie->root);
  free(trie);

  printf("\n");
  return 0;
}
