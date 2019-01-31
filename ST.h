#ifndef ST_H
#define ST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 1024

typedef struct St {
  int len;
  int capacity;
  char **from;
  int *to;
} * ST;

ST STInit() {
  ST st = (ST)malloc(sizeof(struct St));

  st->len = 0;
  st->capacity = INITIAL_SIZE;
  st->from = (char **)calloc(INITIAL_SIZE, sizeof(char *));
  st->to = (int *)calloc(INITIAL_SIZE, sizeof(int));

  return st;
}

int STSearch(ST st, char *key) {
  for (int i = 0; i < st->len; ++i) {
    if (strcmp(key, st->from[i]) == 0)
      return st->to[i];
  }
  return -1;
}

void STInsert(ST st, char *key, int value) {
  if (st->len == st->capacity) {
    st->capacity *= 2;
    st->from = (char **)realloc(st->from, st->capacity);
    st->to = (int *)realloc(st->to, st->capacity);
  }

  st->from[st->len] = strdup(key);
  st->to[st->len] = value;
  st->len++;
}

char *STGetIndex(ST st, int index) { return strdup(st->from[index]); }

#endif
