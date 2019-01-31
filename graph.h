#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

#include "ST.h"
#include "list.h"

#define INITIAL_SIZE 1024

typedef struct graph {
  ST st;
  int V;
  int capacity;
  List *ladj;
} * Graph;

Graph GRAPHInit() {
  Graph G = (Graph)malloc(sizeof(struct graph));

  G->st = STInit();
  G->V = 0;
  G->capacity = INITIAL_SIZE;
  G->ladj = (List *)malloc(INITIAL_SIZE * sizeof(List));
  for (int i = 0; i < INITIAL_SIZE; ++i) {
    G->ladj[i] = LISTInit();
  }

  return G;
}

void GRAPHInsertE(Graph G, int v1, int v2) {
  int max = (v1 > v2) ? v1 : v2;
  if (G->capacity <= max) {
    G->capacity *= 2;
    G->ladj = (List *)realloc(G->ladj, G->capacity);
  }
  G->ladj[v1] = LISTInsertHead(G->ladj[v1], v2);
}

#endif
